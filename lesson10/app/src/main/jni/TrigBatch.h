//
// Created by monty on 25/02/16.
//

#ifndef LESSON10_TRIGBATCH_H
#define LESSON10_TRIGBATCH_H


#include <bits/stl_bvector.h>

class TrigBatch {
    float *vertex;
    float *uv;
    int vertexCount;
public:
    TrigBatch( std::vector<Trig> triangles );
    void draw(GLuint vertexAttributePosition, GLuint textureCoordinatesAttributePosition);
};


#endif //LESSON10_TRIGBATCH_H
