//
// Created by monty on 25/02/16.
//
#include <glm/glm.hpp>
#include <GLES2/gl2.h>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_set>

#include <map>
#include "Trig.h"

#include "TrigBatch.h"

void TrigBatch::draw(GLuint vertexAttributePosition, GLuint textureCoordinatesAttributePosition) {

    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), vertex);
    glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( float ), uv);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

TrigBatch::TrigBatch(const std::vector<Trig>& triangles) {
    int size = triangles.size();

    vertex = new float[3 * 3 * size];
    uv = new float[3 * 2 * size];
    int vertexIndex = 0;
    int uvIndex = 0;
    for (auto &trig : triangles) {
        vertex[vertexIndex++] = trig.p0.x;
        vertex[vertexIndex++] = trig.p0.y;
        vertex[vertexIndex++] = trig.p0.z;
        uv[uvIndex++] = trig.t0.x;
        uv[uvIndex++] = trig.t0.y;
        ++vertexCount;

        vertex[vertexIndex++] = trig.p1.x;
        vertex[vertexIndex++] = trig.p1.y;
        vertex[vertexIndex++] = trig.p1.z;
        uv[uvIndex++] = trig.t1.x;
        uv[uvIndex++] = trig.t1.y;
        ++vertexCount;

        vertex[vertexIndex++] = trig.p2.x;
        vertex[vertexIndex++] = trig.p2.y;
        vertex[vertexIndex++] = trig.p2.z;
        uv[uvIndex++] = trig.t2.x;
        uv[uvIndex++] = trig.t2.y;
        ++vertexCount;
    }
}
