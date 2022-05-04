//
//  ImgSizeStruct.h
//  imgSize
//
//  Created by tridiak on 21/04/22.
//

#ifndef ImgSizeStruct_h
#define ImgSizeStruct_h

enum ImgType {
	imgTypePNG = 0,
	imgTypeGIF = 1,
	imgTypeJPG = 2,
	imgTypeUnk = 999,
};

typedef struct ImgSize {
	uint32_t w;
	uint32_t h;
} ImgSize;

typedef struct ImgSizeList {
	ImgSize first;
	uint16_t count;
	ImgSize* others;
} ImgSizeList;

void ImgSizeFree(ImgSizeList* islPtr);

#endif /* ImgSizeStruct_h */
