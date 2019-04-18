#ifndef _HLS_MUXER_H
#define _HLS_MUXER_H

#include "base/Util.h"
#include <sstream>
#include "File.h"
#include <vector>
#include "MediaPacket.h"

namespace base {



#define SRS_CONSTS_LF "\r\n"


	class SrsHlsSegment
	{
	public:
		int a;
		// duration in seconds in m3u8.
		double duration;
		// sequence number in m3u8.
		int sequence_no;
		// ts uri in m3u8.
		std::string uri;
		// ts full file to write.
		std::string full_path;
		// the muxer to write ts.
		//	SrsHlsCacheWriter* writer;
		//	SrsTSMuxer* muxer;
		// current segment start dts for m3u8
		int64_t segment_start_dts;
		// whether current segement is sequence header.
		bool is_sequence_header;
	public:
		//SrsHlsSegment(SrsTsContext* c, bool write_cache, bool write_file, SrsCodecAudio ac, SrsCodecVideo vc);
		//virtual ~SrsHlsSegment();
	public:
		/**
		* update the segment duration.
		* @current_frame_dts the dts of frame, in tbn of ts.
		*/
		virtual void update_duration(int64_t current_frame_dts)
		{

		}

		int Open(std::string file)
		{
			file_writer_ = std::make_shared<File>(file.c_str());
			return file_writer_->Open();
		}

	private:
		//File file_writer_;
		std::shared_ptr<File> file_writer_;
	};


	class HlsMuxer
	{
		int FlushVideo(MediaPacket& packet);


		int RefreshM3u8()
		{
			std::stringstream ss;
			ss << "#EXTM3U" << SRS_CONSTS_LF
				<< "#EXT-X-VERSION:3" << SRS_CONSTS_LF
				<< "#EXT-X-ALLOW-CACHE:YES" << SRS_CONSTS_LF;
			auto first = *segments_.begin();
			ss << "#EXT-X-MEDIA-SEQUENCE:" << first->sequence_no << SRS_CONSTS_LF;

			//std::vector<SrsHlsSegment*>::iterator it;
			int target_duration = 0;
			//	for (it = segments.begin(); it != segments.end(); ++it) 
			for (auto it : segments_)
			{
				//	SrsHlsSegment* segment = it;
				//	target_duration = srs_max(target_duration, (int)ceil(segment->duration));
				target_duration = base_max(target_duration, (int)ceil(it->duration));
			}
			//max_td = 20
			target_duration = base_max(target_duration, 20);
			ss << "#EXT-X-TARGETDURATION:" << target_duration << SRS_CONSTS_LF;
			/*for (it = segments.begin(); it != segments.end(); ++it) {
			SrsHlsSegment* segment = *it;
			if (segment->is_sequence_header) {
			// #EXT-X-DISCONTINUITY\n
			ss << "#EXT-X-DISCONTINUITY" << SRS_CONSTS_LF;
			printf("write m3u8 segment discontinuity success.");
			}
			}*/

			//for (it = segments.begin(); it != segments.end(); ++it) {
			for (auto it : segments_)
			{
				//SrsHlsSegment* segment = *it;

				/*	if (it->is_sequence_header) {
				// #EXT-X-DISCONTINUITY\n
				ss << "#EXT-X-DISCONTINUITY" << SRS_CONSTS_LF;
				srs_verbose("write m3u8 segment discontinuity success.");
				}*/

				// "#EXTINF:4294967295.208,\n"
				ss.precision(3);
				ss.setf(std::ios::fixed, std::ios::floatfield);
				ss << "#EXTINF:" << it->duration << ", no desc" << SRS_CONSTS_LF;
				printf("write m3u8 segment info success.");

				// {file name}\n
				ss << it->uri << SRS_CONSTS_LF;
				printf("write m3u8 segment uri success.");
			}


			std::string m3u8 = ss.str();
		}

		bool SegmentOverFlow()
		{
			return current_->duration >= hls_fragment;
		}

		int SegmentOpen(uint64_t segment_start_dts)
		{
			//current = new SrsHlsSegment();
			current_ = std::make_shared<SrsHlsSegment>();
			current_->sequence_no = sequence_no_++;
			current_->segment_start_dts = segment_start_dts;

			std::stringstream ss;
			ss << current_->sequence_no << ".ts";
			current_->uri = ss.str();

			//current_->muxter->Open(file);
			current_->Open(current_->uri);
		}

		int SegmentClose()
		{
			//	std::vector<SrsHlsSegment*>::iterator it;
			//	it = std::find(segments.begin(), segments.end(), current);
			//	srs_assert(it == segments.end());

			std::vector<SrsHlsSegment*> segment_to_remove;

			segments_.push_back(current_);

			// shrink the segments.
			double duration = 0;
			int remove_index = -1;
			for (int i = segments_.size() - 1; i >= 0; i--) {
				std::shared_ptr<SrsHlsSegment> segment = segments_[i];
				duration += segment->duration;
				//hls_window = 30
				if ((int)duration > 30) {
					remove_index = i;
					break;
				}
			}
			for (int i = 0; i < remove_index; i++)
			{
				segments_.erase(segments_.begin());
			}

			int ret = RefreshM3u8();

			/*	for (int i = 0; i < (int)segment_to_remove.size(); i++) {
			SrsHlsSegment* segment = segment_to_remove[i];

			if (hls_cleanup && should_write_file) {
			if (unlink(segment->full_path.c_str()) < 0) {
			srs_warn("cleanup unlink path failed, file=%s.", segment->full_path.c_str());
			}
			}


			}*/
			//segment_to_remove.clear();
		}

	private:
		int sequence_no_;
		int64_t segment_start_dts_;
		std::shared_ptr<SrsHlsSegment> current_;
		std::vector<std::shared_ptr<SrsHlsSegment>> segments_;
		double hls_fragment = 5.0;
	};


}
#endif