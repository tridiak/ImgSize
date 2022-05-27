//
//  ImgSize.cpp
//  imgSize
//
//  Created by tridiak on 21/04/22.
//

#include "ImgSize.hpp"

#include <vector>
#include <sys/stat.h>
#include <exception>
#include <bit>

static const uint8_t pngMagic[8] = {137, 80, 78, 71, 13, 10, 26, 10};
static const uint8_t gifAMagic[6] = {0x47, 0x49, 0x46, 0x38, 0x37, 0x61};
static const uint8_t gifBMagic[6] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};

extern "C" int PngImageSizeC(const char* file, ImgSizeList* islPtr);
extern "C" int GifImageSizeC(const char* file, ImgSizeList* islPtr);
extern "C" int JpgImageSizeC(const char* file, ImgSizeList* islPtr);

void PrintImgStruct(std::ostream& io, const ImgSize& is) {
	io << "{" << std::to_string(is.w) << "," << std::to_string(is.h) << "}";
}

// Return: 0 - png, 1 - gif, 2 - jpg, -1 error
int FileType(const char* file) {
	struct stat info;
	int res = stat(file, &info);
	
	if (res != 0) {
		throw std::invalid_argument(strerror(errno));
	}
	
	if ((info.st_mode & S_IFMT) != S_IFREG) {
		throw std::invalid_argument("Not a file");
	}
	
	std::ifstream ins(file, std::ios_base::in | std::ios_base::binary);
	ins.exceptions(std::ifstream::failbit);
	
	uint8_t buffer[32]; bzero(buffer, 32);
	
	ins.read((char*)buffer, 32);
	
	if (memcmp(buffer, pngMagic, 8) == 0) {
		return 0;
	}
	
	if (memcmp(buffer, gifAMagic, 6) == 0 || memcmp(buffer, gifBMagic, 6) == 0) {
		return 1;
	}
	
	if (buffer[0] == 0xFF && buffer[1] == 0xD8) {
		return 2;
	}
	
	return -1;
}

extern "C" int GetImageSizeC(const char* file, ImgSizeList* islPtr) {
	if (!file) { return -1; }
	
	int res = FileType(file);
	if (res < 0) { return -1; }
	if (!islPtr) { return res; }
	
	switch (res) {
		case ImgType::imgTypeGIF:
			return GifImageSizeC(file, islPtr) < 0 ? -1 : ImgType::imgTypeGIF;
		case ImgType::imgTypePNG:
			return PngImageSizeC(file, islPtr) < 0 ? -1 : ImgType::imgTypePNG;
		case ImgType::imgTypeJPG:
			return JpgImageSizeC(file, islPtr) < 0 ? -1 : ImgType::imgTypeJPG;
		default: return ImgType::imgTypeUnk;
	}
}

// JPEG can have multiple sizes. One for thumbnail. One for Image.
std::vector<ImgSize> JpgImageSize(const char* file) {
	struct stat info;
	int res = stat(file, &info);
	
	if (res != 0) {
		throw std::invalid_argument(strerror(errno));
	}
	
	if ((info.st_mode & S_IFMT) != S_IFREG) {
		throw std::invalid_argument("Not a file");
	}
	
	std::vector<ImgSize> sizes; sizes.clear();
	
	size_t size = info.st_size;
	if (size < 6) {
		throw std::invalid_argument("Not a (valid) jpeg file");
	}
	
	std::ifstream ins(file, std::ios_base::in | std::ios_base::binary);
	ins.exceptions(std::ifstream::failbit);
	
	try {
		uint8_t buffer[32]; bzero(buffer, 32);
		
		ins.read((char*)buffer, 2);
		if (buffer[0] != 0xFF || buffer[1] != 0xD8) {
			throw std::invalid_argument("Not a valid jpeg file");
		}
		
		ins.seekg(-2, std::__1::ios_base::end);
		ins.read((char*)buffer, 2);
		if (buffer[0] != 0xFF || buffer[1] != 0xD9) {
			throw std::invalid_argument("Not a valid jpeg file");
		}
		
		size_t pos = 2;
		while (pos < size) {
			//printf("pos=0x%zX", pos);
			ins.seekg(pos);
			ins.read((char*)buffer, 4);
			if (buffer[0] != 0xFF) {
				throw std::invalid_argument("Not a valid jpeg file");
				//printf("X");
			}
			if (buffer[1] == 0xDA) { break; }
			
			uint32_t jump = 0;
			jump = buffer[3] + (buffer[2] << 8) + 2;
			//printf("; buffer[1]=0x%X; length=0x%X\n", buffer[1], jump);
			// FF C0 XX XX XX WW WW HH HH
			if (buffer[1] == 0xC0 || buffer[1] == 0xC2) {
				ins.seekg(pos);
				ins.read((char*)buffer, 12);
				uint32_t h = (buffer[5] << 8) + buffer[6];
				uint32_t w = (buffer[7] << 8) + buffer[8];
				sizes.push_back({w, h});
			}
			
			pos += jump;
		}
		
	}
	catch (std::exception& ex) {
		ins.close();
		throw;
	}
	
	return sizes;
}

extern "C" int JpgImageSizeC(const char* file, ImgSizeList* islPtr) {
	if (!file || !islPtr) { return -1; }
	try {
		auto ary = JpgImageSize(file);
		if (ary.size() > 0) {
			islPtr->first = ary[0];
			uint16_t ct = ary.size() - 1;
			if (ct > 0) {
				islPtr->others = (ImgSize*)malloc(sizeof(ImgSize) * ct);
				islPtr->count = ct;
				for (int t = 0; t < ct; t++) {
					islPtr->others[t] = ary[t];
				}
			}
			else {
				islPtr->count = 0;
				islPtr->others = NULL;
			}
			
			return ct + 1;
		}
		
		return (int)ary.size();
	}
	catch (...) {
		return -1;
	}
}

ImgSize GifImageSize(const char* file) {
	struct stat info;
	int res = stat(file, &info);
	
	if (res != 0) {
		throw std::invalid_argument(strerror(res));
	}
	
	if ((info.st_mode & S_IFMT) != S_IFREG) {
		throw std::invalid_argument("Not a file");
	}
	
	if (info.st_size < 10) {
		throw std::invalid_argument("Not a (valid) gif file");
	}
	
	uint8_t buffer[10]; bzero(buffer, 10);
	
	std::ifstream ins(file, std::ios_base::in | std::ios_base::binary);
	ins.exceptions(std::ifstream::failbit);
	
	uint32_t w = 0;
	uint32_t h = 0;
	try {
		ins.read((char*)buffer, 10);
		ins.close();
		
		if (memcmp(buffer, gifAMagic, 6) != 0 && memcmp(buffer, gifBMagic, 6) != 0) {
			throw std::invalid_argument("Not a valid gif file");
		}
		
		w = (buffer[7] << 8) + buffer[6];
		h = (buffer[9] << 8) + buffer[8];
	}
	catch (std::exception& ex) {
		ins.close();
		throw;
	}
	
	return ImgSize{w, h};
}

extern "C" int GifImageSizeC(const char* file, ImgSizeList* islPtr) {
	if (!file || !islPtr) { return -1; }
	try {
		islPtr->first = GifImageSize(file);
		islPtr->count = 0;
		islPtr->others = NULL;
		return 1;
	}
	catch (...) {
		return -1;
	}
}

ImgSize PngImageSize(const char* file) {
	struct stat info;
	int res = stat(file, &info);
	
	if (res != 0) {
		throw std::invalid_argument(strerror(res));
	}
	
	if ((info.st_mode & S_IFMT) != S_IFREG) {
		throw std::invalid_argument("Not a file");
	}
	
	if (info.st_size < 24) {
		throw std::invalid_argument("Not a (valid) png file");
	}
	
	uint8_t buffer[128]; bzero(buffer, 32);
	
	std::ifstream ins(file, std::ios_base::in | std::ios_base::binary);
	ins.exceptions(std::ifstream::failbit);
	
	const char* ihdrChunk = "IHDR";
	
	uint32_t w = 0;
	uint32_t h = 0;
	size_t pos = 8;
	size_t chunkID = 0;
	bool ihdrFound = false;
	try {
		ins.read((char*)buffer, 8);
		if (memcmp(buffer, pngMagic, 8) != 0) {
			throw std::invalid_argument("Not a valid png file");
		}
		
		while (true) {
			ins.seekg(pos, ins.beg);
			ins.read((char*)buffer, 32);
			chunkID = pos + 4;
			if (memcmp(ihdrChunk, &buffer[4], 4) != 0) {
				uint32_t chunkSize = ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) | ((uint32_t)buffer[2] << 8) | ((uint32_t)buffer[3]);
				pos += 12 + chunkSize;
			}
			else {
				break;
			}
			if (pos >= info.st_size) {
				throw std::invalid_argument("Not a valid png file");
			}
		}
		std::streamsize ct = ins.gcount();
		ins.close();
		
		//----------
		
		
		
		int w1 = buffer[8];
		int w2 = buffer[9];
		int w3 = buffer[10];
		int w4 = buffer[11];
		
		w = (w1 << 24) | (w2 << 16) | (w3 << 8) | w4;
		
		int h1 = buffer[12];
		int h2 = buffer[13];
		int h3 = buffer[14];
		int h4 = buffer[15];
		
		h = (h1 << 24) | (h2 << 16) | (h3 << 8) | h4;
	}
	catch (std::exception& ex) {
		ins.close();
		throw;
	}
	
	return ImgSize{w, h};
}

extern "C" int PngImageSizeC(const char* file, ImgSizeList* islPtr) {
	if (!file || !islPtr) { return -1; }
	try {
		islPtr->first = PngImageSize(file);
		islPtr->count = 0;
		islPtr->others = NULL;
		return 1;
	}
	catch (...) {
		return -1;
	}
	
}
