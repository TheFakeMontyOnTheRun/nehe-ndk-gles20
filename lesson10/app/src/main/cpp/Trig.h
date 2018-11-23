//
// Created by monty on 21/02/16.
//

#ifndef LESSON10_TRIG_H
#define LESSON10_TRIG_H


class Trig {
    float *cachedVertexData;
    float *cachedUVData;
public:
    glm::vec3 p0;
    glm::vec2 t0;

    glm::vec3 p1;
    glm::vec2 t1;

    glm::vec3 p2;
    glm::vec2 t2;

    Trig();

    const float *getVertexData();

    const float *getUVData();

    virtual ~Trig();
};


#endif //LESSON10_TRIG_H
