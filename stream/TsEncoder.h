#ifndef _TS_ENCODER_H
#define _TS_ENCODER_H

#include <stdint.h>
#include "base/DrainableBuffer.h"
#include <memory>

namespace base
{
	
/*struct TsPacketHeader
{
	unsigned char sync_byte:8;
	unsigned char trans_error:1;
	unsigned char start_indicator:1;
	unsigned char trans_priority:1;
	unsigned int  pid:13;
	unsigned char scrambling:2;
	unsigned char adaptation_field:2;
	unsigned char counter:4;
};	*/

struct TsPacketHeader
{
	unsigned char sync_byte;
	unsigned char trans_error;
	unsigned char start_indicator;
	unsigned char trans_priority;
	unsigned int  pid;
	unsigned char scrambling;
	unsigned char adaptation_field;
	unsigned char counter;
};

struct TsPacket
{
	TsPacketHeader header;
	std::shared_ptr<TsAdaptationFiledPacket> adaptation_field;
	std::shared_ptr<TsPesHeaderPacket> pes;

	int Padding(int nb_stuffings)
	{
		if (!adaptation_field)
		{
			adaptation_field = std::make_shared<TsAdaptationFiledPacket>();
			adaptation_field->adaption_field_length = 0; // calc in size.
			adaptation_field->discontinuity_indicator = 0;
			adaptation_field->randon_access_indicator = 0;
			adaptation_field->priority_indicator = 0;
			adaptation_field->pcr_flag = 0;
			adaptation_field->opcr_flag = 0;
			adaptation_field->splicing_point_flag = 0;
			adaptation_field->transport_private_data_flag = 0;
			adaptation_field->adaptation_field_extension_flag = 0;
		}

		adaptation_field->nb_af_reserved = nb_stuffings;
		if (adaption_field_control == SrsTsAdaptationFieldTypePayloadOnly) 
		{
			adaption_field_control = SrsTsAdaptationFieldTypeBoth;
		}
	}

};	

struct TsAdaptationFiledPacket
{
	unsigned char adaption_field_length;
	unsigned char discontinuity_indicator;
	unsigned char randon_access_indicator;
	unsigned char priority_indicator;;
	unsigned char pcr_flag;
	unsigned char opcr_flag;
	unsigned char splicing_point_flag;
	unsigned char transport_private_data_flag;
	unsigned char adaptation_field_extension_flag;
	uint64_t program_clock_reference_base;
	unsigned char pcr_reserved = 0x3f;;
	unsigned short program_clock_reference_extension;
	uint64_t opcr;
	unsigned char splice_countdown;
	unsigned char private_data_len;
	unsigned char* private_data;
	unsigned char adaptation_field_extension_length;
	unsigned char ltw_flag;
	unsigned char piecewise_rate_flag;
	unsigned char seamless_splice_flag;
	unsigned char const1_value1;
	unsigned char ltw_valid_flag;
	unsigned short ltw_offset;
	unsigned int piecewise_rate;
	unsigned char splice_type;
	unsigned char DTS_next_AU0; //3bits
    unsigned char marker_bit0; //1bit
    unsigned short DTS_next_AU1; //15bits
    unsigned char marker_bit1; //1bit
    unsigned short DTS_next_AU2; //15bits
    unsigned char marker_bit2; //1bit
	unsigned int nb_af_ext_reserved;
	unsigned nb_af_reserved;
};

struct TsPesHeaderPacket
{
	unsigned int start_code;
	unsigned char stream_id;
	unsigned short pes_packet_len;
	unsigned char const2bits;
	unsigned char scrambling_control;
	unsigned char priority;
	unsigned char data_alignment_indicator;
	unsigned char copyright;
	unsigned char original_or_copy;
	unsigned char pts_dts_flags;
	unsigned char escr_flag;
	unsigned char es_rate_flag;
	unsigned char dsm_trick_mode_flag;
	unsigned char additional_copy_info_flag;
	unsigned char pes_crc_flag;
	unsigned char pes_extension_flag;
	unsigned char pes_header_len;
	uint64_t pts;
	uint64_t dts;
	uint64_t ESCR_base;
	unsigned short escr_extension;
	unsigned int es_rate;
	unsigned char trick_mode_control;
	unsigned char trick_mode_value;
	unsigned char additional_copy_info;
	unsigned short previous_PES_packet_CRC;
	unsigned char private_data_flag;
	unsigned char pack_header_field_flag;
	unsigned char program_packet_sequence_counter_flag;
	unsigned char std_buffer_flag;
	unsigned char const1_value0;
	unsigned char extension_flag_2;
	unsigned char private_data;
	unsigned char pack_field_len;
	char* pack_field;
	unsigned char program_packet_sequence_counter;
	unsigned char mpeg1_mpeg2_identifier;
	unsigned char original_stuff_length;
	unsigned char std_buffer_scale;
	unsigned short std_buffer_size;
	unsigned char extension_field_length;
	char* extension_field;
	unsigned int nb_stuffings;
	unsigned int nb_bytes;
	unsigned int nb_paddings;
};

void EncodeTsHead(DrainableBuffer& buffer, TsPacketHeader header)
{
	buffer.Write(&header.sync_byte, 1);

	unsigned short pidv = header.pid & 0x1fff;
	pidv |= (header.trans_priority << 13) & 0x2000;
	pidv |= (header.trans_error << 15) & 0x8000;
	pidv |= (header.start_indicator << 14) & 0x4000;
	buffer.Write(&pidv, 2);

	unsigned char ccv = header.counter & 0x0f;
	ccv |= (header.scrambling << 6) & 0xC0;
	ccv |= (header.adaptation_field << 4) & 0x30;
	buffer.Write(&ccv, 1);
}

int EncodeAdaptationField(DrainableBuffer& buffer, TsAdaptationFiledPacket adaptation_field)
{
	buffer.Write(&adaptation_field.adaption_field_length, 1);

	unsigned char tmpv = adaptation_field.adaptation_field_extension_flag & 0x01;
	tmpv |= (adaptation_field.discontinuity_indicator << 7) & 0x80;
	tmpv |= (adaptation_field.randon_access_indicator << 6) & 0x40;
	tmpv |= (adaptation_field.priority_indicator << 5) & 0x20;
	tmpv |= (adaptation_field.pcr_flag << 4) & 0x10;
	tmpv |= (adaptation_field.opcr_flag << 3) & 0x08;
	tmpv |= (adaptation_field.splicing_point_flag << 2) & 0x04;
	tmpv |= (adaptation_field.transport_private_data_flag << 1) & 0;
	buffer.Write(&tmpv, 1);

	if (adaptation_field.pcr_flag)
	{
		uint64_t pcrv = adaptation_field.program_clock_reference_extension & 0x1ff;
		pcrv |= (adaptation_field.pcr_reserved << 9) & 0x7E00;
		pcrv |= (adaptation_field.program_clock_reference_base << 15) & 0xFFFFFFFF8000LL;
	}
}

int EncodDtsPts(DrainableBuffer& buffer, unsigned char flag, uint64_t pts_dts)
{
	unsigned char val = 0;
	val = ((flag << 4) | ((pts_dts >> 29) & 0x0E) | 0x01) & 0xff;
	buffer.Write(&val, 1);
	val = ((((pts_dts >> 14) & 0xfffe) | 0x01) >> 8) & 0xff;
	buffer.Write(&val, 1);
	val = (((pts_dts >> 14) & 0xfffe) | 0x01) & 0xff;
	buffer.Write(&val, 1);
	val = ((((pts_dts << 1) & 0xfffe) | 0x01) >> 8) & 0xff;
	buffer.Write(&val, 1);
	val = (((pts_dts << 1) & 0xfffe) | 0x01) & 0xff;
	buffer.Write(&val, 1);
}

int EncodePesHeader(DrainableBuffer& buffer, TsPesPacket pes)
{
	buffer.Write(&pes.start_code, 3);
	buffer.Write(&pes.stream_id, 1);

	unsigned short  packet_len  = 0;
/*	if (pes.pes_packet_len > 0)
	{
		packet_len = PES_packet_length + 3 + PES_header_data_length;
		packet_len = (packet_len > 0xFFFF) ? 0 : packet_len;
	}.*/
	buffer.Write(&packet_len, 2);

	unsigned char oocv = pes.original_or_copy & 0x01;
	oocv |= (pes.const2bits << 6) & 0xC0;
	oocv |= (pes.scrambling_control << 4) & 0x30;
	oocv |= (pes.priority << 3) & 0x08;
	oocv |= (pes.data_alignment_indicator << 2) & 0x04;
	oocv |= (pes.copyright << 1) & 0x02;
	buffer.Write(&oocv, 1);

	unsigned char pefv = pes.pes_extension_flag & 0x01;
	pefv |= (pes.pts_dts_flags << 6) & 0xC0;
	pefv |= (pes.escr_flag << 5) & 0x20;
	pefv |= (pes.es_rate_flag << 4) & 0x10;
	pefv |= (pes.dsm_trick_mode_flag << 3) & 0x08;
	pefv |= (pes.additional_copy_info_flag << 2) & 0x04;
	pefv |= (pes.pes_crc_flag << 1) & 0x02;
	buffer.Write(&pefv, 1);

	buffer.Write(&pes.pes_header_len, 1);

	if (pes.pts_dts_flags == 0x02)
	{
		EncodDtsPts(buffer, 0x02, pes.pts);
	}
	if (pes.pts_dts_flags == 0x03)
	{
		EncodDtsPts(buffer, 0x03, pes.pts);
		EncodDtsPts(buffer, 0x01, pes.dts);
	}
}

int Encode(DrainableBuffer& buffer)
{
	EncodeHead(buffer, TsPacketHeader);
	if(adaptation_field)
	{
		adaptation_field->EncodeAdaptationField(buffer, adaptation_field);
	}
	if(payload)
	{
		//payload->EncodePes(payload);
		EncodePesHeader();
	}
	
}

enum SrsTsAdaptationFieldType
{
	// Reserved for future use by ISO/IEC
	SrsTsAdaptationFieldTypeReserved = 0x00,
	// No adaptation_field, payload only
	SrsTsAdaptationFieldTypePayloadOnly = 0x01,
	// Adaptation_field only, no payload
	SrsTsAdaptationFieldTypeAdaptionOnly = 0x02,
	// Adaptation_field followed by payload
	SrsTsAdaptationFieldTypeBoth = 0x03,
};




void EncodePes()
	{
		char* start;
		char* end;
		char* p = start;
		while(p < )
		{
			if(first)
			{
				bool write_pcr = msg->write_pcr;
				int64_t pcr = dts;
				pkt = create_pes_first();
				patcket = create_pes_first();
			}
			else
			{
				pkt = CreateFirstPes();
			}
			int pkt_size = pkt.size();
			
			if(pkt.size() < SRS_TS_PACKET_SIZE - header)
			{
				pkt.Padding();
			}
			
			DrainableBuffer buffer(SRS_TS_PACKET_SIZE);
			EncodeTsHeader(buffer);
			int head_size = buffer.Size();
			int es_size = (int)min(msg.shengyu(), SRS_TS_PACKET_SIZE - head_size);
			int nb_strffings = SRS_TS_PACKET_SIZE - head_size - es_size;
			if(nb_strffings > 0)
			{
				pkt.Padding(nb_strffings);
			}
			memcpy(buf + nb_buf, p, left);
			buffer.Write(p.left);
		//	writefile();
			
		}
	}	



TsPacket* CreateFirstPes(unsigned short pid, unsigned short stream_id, unsigned short counter, bool discontinuity,
	uint64_t pcr, uint64_t pts, uint64_t dts)
{
	TsPacket* pkt = new TsPacket();
	//TsPacketHeader pkt = new TsPacketHeader();
	pkt->header.sync_byte = 0x47;
    pkt->header.trans_error = 0;
    pkt->header.start_indicator = 1;
    pkt->header.trans_priority = 0;
    pkt->header.pid = pid;
   // pkt->header.scrambling = SrsTsScrambledDisabled;
	pkt->header.scrambling = 0;
    pkt->header.adaptation_field = SrsTsAdaptationFieldTypePayloadOnly;
    pkt->header.counter = counter;
    pkt->header.adaptation_field = NULL;

	if(pcr >= 0)
	{
	//	TsAdaptationFiledPacket *af = new TsAdaptationFiledPacket();
		pkt->adaptation_field = std::make_shared<TsAdaptationFiledPacket>();
		pkt->adaptation_field->adaption_field_length = 0; // calc in size.
		pkt->adaptation_field->discontinuity_indicator = discontinuity;
		pkt->adaptation_field->randon_access_indicator = 0; 
		pkt->adaptation_field->priority_indicator = 0;
		pkt->adaptation_field->pcr_flag = 1;
		pkt->adaptation_field->opcr_flag = 0;
		pkt->adaptation_field->splicing_point_flag = 0;
		pkt->adaptation_field->transport_private_data_flag = 0;
		pkt->adaptation_field->adaptation_field_extension_flag = 0;
		pkt->adaptation_field->program_clock_reference_base = pcr;
		pkt->adaptation_field->program_clock_reference_extension = 0;
	}

	pkt->pes = std::make_shared<TsPesHeaderPacket>();
	pkt->pes->start_code = 0x01;
	pkt->pes->stream_id = stream_id;
	pkt->pes->pes_packet_len = (size > 0xFFFF)? 0:size;
	pkt->pes->scrambling_control = 0;
	pkt->pes->priority = 0;
	pkt->pes->data_alignment_indicator = 0;
	pkt->pes->copyright = 0;
	pkt->pes->original_or_copy = 0;
	pkt->pes->pts_dts_flags = (dts == pts)? 0x02:0x03;
	pkt->pes->escr_flag = 0;
	pkt->pes->es_rate_flag = 0;
	pkt->pes->dsm_trick_mode_flag = 0;
	pkt->pes->additional_copy_info_flag = 0;
	pkt->pes->pes_crc_flag = 0;
	pkt->pes->pes_extension_flag = 0;
	pkt->pes->pes_header_len = 0; // calc in size.
	pkt->pes->pts = pts;
	pkt->pes->dts = dts;
	
	return pkt;
}	
TsPacket* CreateFirstPes(unsigned short pid, unsigned short stream_id, unsigned char  counter) 
{
	TsPacket* pkt = new TsPacket();
	pkt->header.sync_byte = 0x47;
	pkt->header.trans_error = 0;
	pkt->header.start_indicator = 0;
	pkt->header.trans_priority = 0;
	pkt->header.pid = pid;
	// pkt->header.scrambling = SrsTsScrambledDisabled;
	pkt->header.scrambling = 0;
	pkt->header.adaptation_field = SrsTsAdaptationFieldTypePayloadOnly;
	pkt->header.counter = counter;

	pkt->adaptation_field = NULL;
	pkt->pes = NULL;

	return pkt;
}

	
}
#endif