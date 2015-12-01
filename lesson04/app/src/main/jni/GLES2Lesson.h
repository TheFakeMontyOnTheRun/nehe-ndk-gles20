//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H

class GLES2Lesson {

    void fetchShaderLocations();
    void setPerspective();
    void prepareShaderProgram();
    void clearBuffers();
    void resetTransformMatrices();
    void printVerboseDriverInformation();

    GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
    GLuint loadShader(GLenum shaderType, const char *pSource);

    const static float triangleVertices[ 9 ];
    const static float triangleColours[ 9 ];
    const static unsigned short triangleIndices[ 3 ];

    const static float squareVertices[ 12 ];
    const static float squareColours[ 12 ];
    const static unsigned short squareIndices[ 4 ];

    glm::mat4 triangleTransformMatrix;
    glm::mat4 squareTransformMatrix;
    glm::mat4 projectionMatrix;

    GLuint vertexAttributePosition;
    GLuint colourAttributePosition;
    GLuint modelMatrixAttributePosition;
    GLuint projectionMatrixAttributePosition;
    GLuint gProgram;

    float triangleRotationAngle;
    float squareRotationAngle;
public:
    GLES2Lesson();
    ~GLES2Lesson();
    bool init( float w, float h, const std::string& vertexShader, const std::string& fragmentShader );
    void render();
    void shutdown();
    void tick();
    void drawGeometry( const float* vertexData, const float* colorData, const unsigned short* indexData, int vertexCount, const glm::mat4& transform );
};

#endif //LESSON02_GLES2LESSON_H
