#pragma once
#include "AVC.h"
#include "BinaryReader.h"


namespace base {



	unsigned int AVC::ReadVideoConfig(const unsigned char* data, unsigned int size, std::string& sps, std::string& pps) {
	BinaryReader reader(data, size);
	unsigned int ret = 0;
	reader.Next(5); // skip avcC version 1 + 3 bytes of profile, compatibility, level + 1 byte xFF

	// SPS and PPS
	ret += 5;
	unsigned char spsCount(reader.Read8() & 0x1F);
	ret += 1;
	for (auto i = 0; i < spsCount; i++)
	{
		unsigned int spsSize = reader.Read16();
		//sps.append(reader.Current(), spsSize);
		 sps.append((const char *)reader.Current(), spsSize);
	//	sps->Append(reader.Current(), spsSize);
		reader.Next(spsSize); 
		ret += (spsSize + 2);
	}

	unsigned char ppsCount(reader.Read8());
	ret += 1;
	for (auto i = 0; i < spsCount; i++)
	{
		unsigned int ppsSize = reader.Read16();
	//	pps.append("111", ppsSize);
		pps.append((const char *)reader.Current(), ppsSize);
	//	pps->Append(reader.Current(), ppsSize);
		reader.Next(ppsSize);
		ret += (ppsSize + 2);
	}

	return ret;
	
	/*bool isPPS(false);
	while (reader.available() >= 2 && count--) {
		size = reader.read16();
		if (size > reader.available())
			size = reader.available();
		writer.write32(size).write(reader.current(), size);
		reader.next(size);
		if (!count) {
			if (isPPS)
				break;
			count = reader.read8(); // PPS now!
			isPPS = true;
		}
	}
	return reader.position();*/
}

}