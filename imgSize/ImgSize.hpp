//
//  ImgSize.hpp
//  imgSize
//
//  Created by tridiak on 21/04/22.
//

#ifndef ImgSize_hpp
#define ImgSize_hpp

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "ImgSizeStruct.h"

ImgSize PngImageSize(const char* file);
ImgSize GifImageSize(const char* file);
std::vector<ImgSize> JpgImageSize(const char* file);
void PrintImgStruct(std::ostream& io, const ImgSize& is);

#endif /* ImgSize_hpp */
