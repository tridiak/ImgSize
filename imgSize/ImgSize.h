//
//  ImgSize.h
//  imgSize
//
//  Created by tridiak on 21/04/22.
//

#ifndef ImgSize_h
#define ImgSize_h

#ifdef __cplusplus
extern "C" {
#endif

#include "ImgSizeStruct.h"

/// Get image dimensions for passed file.
/// Buffer will be zero'd before use. buffercount is number of ImgSize items the buffer can hold , not the byte count.
///
/// @Return Returns enum ImgType value or -1 for error.
int GetImageSizeC(const char* file, ImgSizeList* islPtr);

/// Gets image dimensions for passed file. Returns -1 for any error. 1 otherwise.
int PngImageSizeC(const char* file, ImgSizeList* islPtr);

/// Gets image dimensions for passed file. Returns -1 for any error. 1 otherwise.
int GifImageSizeC(const char* file, ImgSizeList* islPtr);

/// Gets image dimensions for passed file. Returns -1 for any error.
///
/// @Parameter islPtr. Call ImgSizeFree() when done.
int JpgImageSizeC(const char* file, ImgSizeList* islPtr);

#ifdef __cplusplus
}
#endif


#endif /* ImgSize_h */
