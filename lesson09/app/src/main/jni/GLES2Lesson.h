//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H

#include "Star.h"

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

        void initStars();

        void drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                          const glm::mat4 &transform);

        GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

        GLuint loadShader(GLenum shaderType, const char *pSource);

        const static std::array< float, 4 * 9 > cubeVertices;
        const static std::array<unsigned short, 4> cubeIndices;

        const static int PREFERED_NUMBER_OF_STARS;

        std::vector<std::shared_ptr<Star>> mStars;

        glm::mat4 projectionMatrix;

        GLuint vertexAttributePosition;
        GLuint modelMatrixAttributePosition;
        GLuint samplerUniformPosition;
        GLuint textureCoordinatesAttributePosition;
        GLuint projectionMatrixAttributePosition;
        GLuint fragmentTintPosition;
        GLuint gProgram;

        GLuint textureId;

        //VBO stuff
        GLuint vboCubeVertexDataIndex;
        GLuint vboCubeVertexIndicesIndex;

        int *textureData;
        int textureWidth;
        int textureHeight;

        float movementPosition;
        float movementDelta;
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

        void toggleTwinkling();

        void zoomIn();

        void speedUpTwist();

        void zoomOut();

        void speedDownTwist();
    };
}
#endif //LESSON02_GLES2LESSON_H
