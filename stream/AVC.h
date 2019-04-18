#pragma once

#include "Buffer.h"
#include <string>

namespace base {


enum AvcNaluType
{
    AvcNaluTypeReserved = 0,
   
    AvcNaluTypeNonIDR = 1,
 
    AvcNaluTypeDataPartitionA = 2,
    AvcNaluTypeDataPartitionB = 3,

    AvcNaluTypeDataPartitionC = 4,

    AvcNaluTypeIDR = 5,
   
    AvcNaluTypeSEI = 6,
  
    AvcNaluTypeSPS = 7,
  
    AvcNaluTypePPS = 8,
    
    AvcNaluTypeAccessUnitDelimiter = 9,
   
    AvcNaluTypeEOSequence = 10,
   
    AvcNaluTypeEOStream = 11,
   
    AvcNaluTypeFilterData = 12,
    
    AvcNaluTypeSPSExt = 13,

    AvcNaluTypePrefixNALU = 14,
    
    AvcNaluTypeSubsetSPS = 15,
   
    AvcNaluTypeLayerWithoutPartition = 19,
   
    AvcNaluTypeCodedSliceExt = 20,
};

enum CodecVideoAVCType
{  
	CodecVideoAVCTypeSequenceHeader      = 0,
    CodecVideoAVCTypeNALU                = 1,
    CodecVideoAVCTypeSequenceHeaderEOF   = 2,
	CodecVideoAVCTypeReserved           = 3,
};

	class AVC
	{
	public:
		AVC() {}
		~AVC() {}
		static unsigned int ReadVideoConfig(const unsigned char* data, unsigned int size, std::string& sps, std::string& pps);
	private:

	};

}