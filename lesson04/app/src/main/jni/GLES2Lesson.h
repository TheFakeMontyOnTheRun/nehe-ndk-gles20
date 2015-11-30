//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H

class GLES2Lesson {

    static void fetchShaderLocations();
    static void setPerspective();
    static void prepareShaderProgram();
    static void clearBuffers();
    static void resetTransformMatrices();
    static void printVerboseDriverInformation();

    static GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
    static GLuint loadShader(GLenum shaderType, const char *pSource);

    static float triangleVertices[ 9 ];
    static float triangleColours[ 9 ];
    static unsigned short triangleIndices[ 3 ];

    static float squareVertices[ 12 ];
    static float squareColours[ 12 ];
    static unsigned short squareIndices[ 4 ];

    static glm::mat4 triangleTransformMatrix;
    static glm::mat4 squareTransformMatrix;
    static glm::mat4 projectionMatrix;

    static GLuint vertexAttributePosition;
    static GLuint colourAttributePosition;
    static GLuint modelMatrixAttributePosition;
    static GLuint projectionMatrixAttributePosition;
    static GLuint gProgram;

    static float triangleRotationAngle;
    static float squareRotationAngle;
public:

    static bool init( float w, float h, const std::string& vertexShader, const std::string& fragmentShader );
    static void render();
    static void shutdown();
    static void tick();
    static void drawGeometry( const float* vertexData, const float* colorData, const unsigned short* indexData, int vertexCount, const glm::mat4& transform );
};

#endif //LESSON02_GLES2LESSON_H
