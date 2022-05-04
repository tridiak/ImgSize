//
//  imgSizeFWIOS.h
//  imgSizeFWIOS
//
//  Created by tridiak on 29/04/22.
//

#import <Foundation/Foundation.h>

#import "ImgSizeStruct.h"

// Framework for iOS

//! Project version number for ImgSizeFW.
FOUNDATION_EXPORT double ImgSizeFWVersionNumber;

//! Project version string for ImgSizeFW.
FOUNDATION_EXPORT const unsigned char ImgSizeFWVersionString[];

/// Get image dimensions for passed file.
///
/// @Parameter islPtr. Call ImgSizeFree() when done. Pass nil to just retrieve the image type,
/// @Return ImgType value or -1 for error.
FOUNDATION_EXPORT int GetImageSizeC(const char* file, ImgSizeList* islPtr);

/// Gets image dimensions for passed file. Returns -1 for any error. 1 otherwise.
///
/// @Parameter islPtr. Call ImgSizeFree() when done.
FOUNDATION_EXPORT int PngImageSizeC(const char* file, ImgSizeList* islPtr);

/// Gets image dimensions for passed file. Returns -1 for any error. 1 otherwise.
///
/// @Parameter islPtr. Call ImgSizeFree() when done.
FOUNDATION_EXPORT int GifImageSizeC(const char* file, ImgSizeList* islPtr);

/// Gets image dimensions for passed file. Returns -1 for any error.
///
/// @Parameter islPtr. Call ImgSizeFree() when done.
FOUNDATION_EXPORT int JpgImageSizeC(const char* file, ImgSizeList* islPtr);
