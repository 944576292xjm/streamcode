#pragma once

#include <memory>

#include "AMF.h"
#include "base/WorkPool.h"
#include <memory>
#include "MediaPacket.h"
#include "File.h"

namespace base {



class FlvReader 
{
public:
	FlvReader(const char *filepath) : file_(filepath){}
	~FlvReader() {}

	typedef std::function<void(MediaPacket)> FlvReaderReadCallback;
	
	bool Open();
	
	size_t DecodeChunkInternal(const unsigned char* data, size_t size);

	void DocodeChunk(const unsigned char*data, size_t size);
	
	


	void SetReadCallback(FlvReaderReadCallback cb)
	{
		//base::AutoLock lock(callbackMutex);
		//callbackQueue_.push_back(cb);
		readCallback_ = std::move(cb);
	}
	
	void ReadHeader()
	{
		int len = file_.Read(temp_buffer_, 200);
		DecodeHead(temp_buffer_, len);
		ReadNextChunk();
	}

	void DecodeHead(const unsigned char*data, size_t size);

	
	void ReadNextChunk()
	{
		WorkPool::SingleInstance()->PostTask(std::bind(&FlvReader::ReadFile, this));
	}

	void ReadFile()
	{
	//	while (1) 
	//	{
			int len = file_.Read(temp_buffer_, 4 * 1024);
			if (len < 4 * 1024)
			{
				printf("tuichu le --\n");
			//	file_.Open();
				printf("end--\n");
				return;
			}
			DocodeChunk(temp_buffer_, len);
//		}
	}
	

	//void SetReadCallback();

private:
	File file_;
	std::vector<unsigned char> data_buffer_;
	std::vector<MediaPacket> packets_;
	unsigned char temp_buffer_[4* 1024];
	std::string filepath_;

private:
	unsigned char version_;
	bool hasVideo_;
	bool hasAudio_;

private:
	FlvReaderReadCallback readCallback_;
};


}