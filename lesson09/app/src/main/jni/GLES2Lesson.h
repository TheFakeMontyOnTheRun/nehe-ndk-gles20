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

        void initStars();

        void drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                          const glm::mat4 &transform);

        GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

        GLuint loadShader(GLenum shaderType, const char *pSource);

        const static std::array <float, 4 * 9> cubeVertices;
        const static std::array <unsigned short, 4> cubeIndices;

        const static int PREFERED_NUMBER_OF_STARS;

        std::vector <std::shared_ptr<Star>> mStars;

        glm::mat4 projectionMatrix;

        GLint vertexAttributePosition;
        GLint modelMatrixAttributePosition;
        GLint samplerUniformPosition;
        GLint textureCoordinatesAttributePosition;
        GLint projectionMatrixAttributePosition;
        GLint fragmentTintPosition;
        GLuint gProgram;

        GLuint textureId;
        GLuint twinkleId;

        //VBO stuff
        GLuint vboCubeVertexDataIndex;
        GLuint vboCubeVertexIndicesIndex;

        int *textureData;
        int *twinkleData;
        int textureWidth;
        int textureHeight;

        float movementPosition;
        float movementDelta;
        float rotationPosition;
        float rotationDelta;
        bool twinkling;
    public:
        GLES2Lesson();

        ~GLES2Lesson();

        bool init(float w, float h, const std::string &vertexShader,
                  const std::string &fragmentShader);

        void setTexture(int *bitmapData, int *detailData, int width, int height, int format);

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
