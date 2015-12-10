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
    void createVBOs();
    void deleteVBOs();
    void drawGeometry( const int vertexVbo, const int indexVbo, int vertexCount, const glm::mat4& transform );

    GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
    GLuint loadShader(GLenum shaderType, const char *pSource);

    const static float pyramidVertices[ 18 ];
    const static unsigned short pyramidIndices[ 3 ];

    const static float cubeVertices[ 24 ];
    const static unsigned short cubeIndices[ 4 ];

    glm::mat4 pyramidTransformMatrix;
    glm::mat4 cubeTransformMatrix;
    glm::mat4 projectionMatrix;

    GLuint vertexAttributePosition;
    GLuint colourAttributePosition;
    GLuint modelMatrixAttributePosition;
    GLuint projectionMatrixAttributePosition;
    GLuint gProgram;

    //VBO stuff
    GLuint vboPyramidVertexDataIndex;
    GLuint vboCubeVertexDataIndex;
    GLuint vboPyramidVertexIndicesIndex;
    GLuint vboCubeVertexIndicesIndex;

    float pyramidRotationAngle;
    float cubeRotationAngle;
public:
    GLES2Lesson();
    ~GLES2Lesson();
    bool init( float w, float h, const std::string& vertexShader, const std::string& fragmentShader );
    void render();
    void shutdown();
    void tick();
};

#endif //LESSON02_GLES2LESSON_H
