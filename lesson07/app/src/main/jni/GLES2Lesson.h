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

    void drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                      const glm::mat4 &transform);

    GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

    GLuint loadShader(GLenum shaderType, const char *pSource);

    const static float cubeVertices[6 * 4 * 9];
    const static unsigned short cubeIndices[6 * 6];

    const static glm::vec4 ambientLightFullColor;
    const static glm::vec4 ambientLightOffColor;

    glm::mat4 cubeTransformMatrix;
    glm::mat4 projectionMatrix;

    GLuint vertexAttributePosition;
    GLuint modelMatrixAttributePosition;
    GLuint samplerUniformPosition;
    GLuint textureCoordinatesAttributePosition;
    GLuint projectionMatrixAttributePosition;
    GLuint normalAttributePosition;

    GLuint gProgram;

    GLuint textureId;

    //VBO stuff
    GLuint vboCubeVertexDataIndex;
    GLuint vboCubeVertexIndicesIndex;

    int *textureData;
    int textureWidth;
    int textureHeight;
    GLuint currentFilter;
    float cubeRotationAngleYZ;
    float cubeRotationAngleXZ;

    glm::vec4 diffuseLightDirection;
    glm::vec4 diffuseLightColor;
    glm::vec4 ambientLightColor;

    GLuint diffuseLightDirectionShaderLocation;
    GLuint diffuseLightColorShaderLocation;
    GLuint ambientLightColorShaderLocation;

    float rotationXZSpeed;
    float rotationYZSpeed;
public:
    GLES2Lesson();

    ~GLES2Lesson();

    bool init(float w, float h, const std::string &vertexShader, const std::string &fragmentShader);

    void setTexture(int *bitmapData, int width, int height, int format);

    void render();

    void shutdown();

    void tick();

    void toggleFiltering();

    void toggleLightning();

    void speedUpXZ();

    void speedDownXZ();

    void speedUpYZ();

    void speedDownYZ();

    void reset();
};

#endif //LESSON02_GLES2LESSON_H
