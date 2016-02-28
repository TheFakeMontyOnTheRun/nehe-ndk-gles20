//
// Created by monty on 28/02/16.
//

#include "NativeBitmap.h"

int NativeBitmap::getWidth() {
    return this->mWidth;
}

int NativeBitmap::getHeight() {
    return this->mHeight;
}

int *NativeBitmap::getPixelData() {
    return this->mRawData;
}

NativeBitmap::NativeBitmap(int aWidth, int aHeight, int *aRawData) :
        mWidth(aWidth), mHeight(aHeight), mRawData(aRawData) {
}

void NativeBitmap::releaseTextureData() {
    this->mWidth = 0;
    this->mHeight = 0;
    this->mRawData = nullptr;
}

NativeBitmap::~NativeBitmap() {
    delete mRawData;
}
