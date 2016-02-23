//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H

namespace odb {

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

        glm::mat4 projectionMatrix;

        GLuint vertexAttributePosition;
        GLuint modelMatrixAttributePosition;
        GLuint samplerUniformPosition;
        GLuint textureCoordinatesAttributePosition;
        GLuint projectionMatrixAttributePosition;
        GLuint gProgram;

        GLuint textureId;

        //VBO stuff
        GLuint vboCubeVertexDataIndex;
        GLuint vboCubeVertexIndicesIndex;

        int *textureData;
        int textureWidth;
        int textureHeight;
    public:

        void addTriangleAt( const glm::vec4& aP0, const glm::vec4& aP1, glm::vec4& aP2 );

        GLES2Lesson();

        ~GLES2Lesson();

        bool init(float w, float h, const std::string &vertexShader,
                  const std::string &fragmentShader);

        void setTexture(int *bitmapData, int width, int height, int format);

        void render();

        void shutdown();

        void tick();

        void reset();

        void addTrigs(std::vector<Trig> vector);
    };
}
#endif //LESSON02_GLES2LESSON_H
