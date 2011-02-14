#pragma once
#include "definitions.h"

class cResourceManager;

class cImage{
    friend cResourceManager;
public:
private:
    image_t mData;
};

typedef cImage* tImagePtr;