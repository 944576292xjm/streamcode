#pragma once

namespace base {

	
#define FLV_HEADER_LENGTH 9



enum Codec { // Aligned values with FLV codec value
	FLV_CODEC_RAW = 0,
	FLV_CODEC_JPEG = 1,
	FLV_CODEC_SORENSON = 2,
	FLV_CODEC_SCREEN1 = 3,
	FLV_CODEC_VP6 = 4,
	FLV_CODEC_VP6_ALPHA = 5,
	FLV_CODEC_SCREEN2 = 6,
	FLV_CODEC_H264 = 7,
	FLV_CODEC_H263 = 8,
	FLV_CODEC_MPEG4_2 = 9,
	FLV_CODEC_HEVC = 12 // Added codec HEVC (not supported by FLV)
};

enum TagType
{
	FLV_TAG_AUDIO = 0x08,
	FLV_TAG_VIDEO = 0x09,
	FLV_TAG_META = 0x12,
};

enum FrameType { // Aligned values with FLV frame type value, and ignore FRAME_GENERATED_KEYFRAME which is redundant with FRAME_KEY for a "key frame" test condition
	FLV_FRAME_UNSPECIFIED = 0,
	FLV_FRAME_KEY = 1,
	FLV_FRAME_INTER = 2, // Used too by H264 for Config sequence
	FLV_FRAME_DISPOSABLE_INTER = 3, // just for H263
	FLV_FRAME_INFO = 5,
	FLV_FRAME_CONFIG = 7
};




}