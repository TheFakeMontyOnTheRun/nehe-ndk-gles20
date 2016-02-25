//
// Created by monty on 21/02/16.
//
#include <algorithm>
#include <glm/glm.hpp>
#include "Trig.h"

Trig::Trig() {
    cachedVertexData = nullptr;
    cachedUVData = nullptr;
}
const float* Trig::getUVData() {
    if ( cachedUVData == nullptr ) {
        cachedUVData = new float[ 3 * 2 ];

        cachedUVData[ 0 ] = t0.x;
        cachedUVData[ 1 ] = t0.y;

        cachedUVData[ 2 ] = t1.x;
        cachedUVData[ 3 ] = t1.y;

        cachedUVData[ 4 ] = t2.x;
        cachedUVData[ 5 ] = t2.y;
    }

    return cachedUVData;
}


const float* Trig::getVertexData() {
    if ( cachedVertexData == nullptr ) {
        cachedVertexData = new float[ 3 * 3 ];

        cachedVertexData[ 0 ] = p0.x;
        cachedVertexData[ 1 ] = p0.y;
        cachedVertexData[ 2 ] = p0.z;

        cachedVertexData[ 3 ] = p1.x;
        cachedVertexData[ 4 ] = p1.y;
        cachedVertexData[ 5 ] = p1.z;

        cachedVertexData[ 6 ] = p2.x;
        cachedVertexData[ 7 ] = p2.y;
        cachedVertexData[ 8 ] = p2.z;
    }

    return cachedVertexData;
}

Trig::~Trig() {
    delete cachedUVData;
    delete cachedVertexData;
}
