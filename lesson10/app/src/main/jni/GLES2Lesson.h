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

        GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

        GLuint loadShader(GLenum shaderType, const char *pSource);

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        GLuint vertexAttributePosition;
        GLuint modelMatrixAttributePosition;
        GLuint viewMatrixAttributePosition;
        GLuint samplerUniformPosition;
        GLuint textureCoordinatesAttributePosition;
        GLuint projectionMatrixAttributePosition;
        GLuint gProgram;

        GLuint textureId;


        int *textureData;
        int textureWidth;
        int textureHeight;
        std::vector<Trig> mTrigs;
        glm::vec3 camera;
    public:
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
