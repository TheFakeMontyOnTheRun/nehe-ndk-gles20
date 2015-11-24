//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H


class GLES2Lesson {
public:

    static float triangleVertices[ 9 ];
    static float squareVertices[ 18 ];
    static float triangleTransformMatrix[ 16 ];
    static float squareTransformMatrix[ 16 ];
    static float projectionMatrix[ 16 ];

    static GLuint vertexAttributePosition;
    static GLuint modelMatrixAttributePosition;
    static GLuint projectionMatrixAttributePosition;
    static GLuint gProgram;

    static bool init( float w, float h, const std::string& vertexShader, const std::string& fragmentShader );
    static void render();
    static void shutdown();
};


#endif //LESSON02_GLES2LESSON_H
