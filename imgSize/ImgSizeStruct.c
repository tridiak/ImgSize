//
//  ImgSizeStruct.c
//  imgSize
//
//  Created by tridiak on 22/04/22.
//

#include <stdio.h>
#include "ImgSizeStruct.h"
#include <stdlib.h>

void ImgSizeFree(ImgSizeList* islPtr) {
	if (islPtr && islPtr->others) {
		free(islPtr->others);
		islPtr->others = NULL;
	}
}
