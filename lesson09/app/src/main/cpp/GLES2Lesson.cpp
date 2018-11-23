//
// Created by monty on 23/11/15.
//

#include <GLES2/gl2.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <random>
#include <android/log.h>

#include "Star.h"
#include "GLES2Lesson.h"
#include "NdkGlue.h"


namespace odb {
    const std::array<float, 4 * 9> GLES2Lesson::cubeVertices{
            -1.0f, 1.0f, -3.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -3.0f, 1.0f, 0.0f,
            1.0f, -1.0f, -3.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -3.0f, 0.0f, 1.0f,
    };


    const std::array<unsigned short, 4> GLES2Lesson::cubeIndices{
            0, 3, 1,
            2
    };

    const int GLES2Lesson::PREFERED_NUMBER_OF_STARS = 50;


    GLuint uploadTextureData(int *textureData, int width, int height) {
        // Texture object handle
        GLuint textureId = 0;

        //Generate texture storage
        glGenTextures(1, &textureId);

        //specify what we want for that texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        //upload the data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     textureData);

        return textureId;
    }


    extern void printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        LOGI("GL %s = %s\n", name, v);
    }

    extern void checkGlError(const char *op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    GLuint GLES2Lesson::loadShader(GLenum shaderType, const char *pSource) {
        auto shader = glCreateShader(shaderType);
        if (shader) {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen) {
                    char *buf = (char *) malloc(infoLen);
                    if (buf) {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }


    GLuint GLES2Lesson::createProgram(const char *pVertexSource, const char *pFragmentSource) {
        auto vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);

        if (!vertexShader) {
            return 0;
        }

        auto pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (!pixelShader) {
            return 0;
        }

        auto program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength) {
                    char *buf = (char *) malloc(bufLength);
                    if (buf) {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        LOGE("Could not link program:\n%s\n", buf);
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }

    void GLES2Lesson::printVerboseDriverInformation() {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);
    }

    GLES2Lesson::GLES2Lesson() {
//start off as identity - later we will init it with proper values.
        projectionMatrix = glm::mat4(1.0f);
        textureData = nullptr;
        vertexAttributePosition = 0;
        modelMatrixAttributePosition = 0;
        projectionMatrixAttributePosition = 0;
        gProgram = 0;
        reset();
    }

    GLES2Lesson::~GLES2Lesson() {
        deleteVBOs();
        glDeleteTextures(1, &textureId);
    }

    bool GLES2Lesson::init(float w, float h, const std::string &vertexShader,
                           const std::string &fragmentShader) {

        printVerboseDriverInformation();

        gProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());

        if (!gProgram) {
            LOGE("Could not create program.");
            return false;
        }

        fetchShaderLocations();

        glViewport(0, 0, w, h);
        checkGlError("glViewport");

        projectionMatrix = glm::perspective(45.0f, w / h, 0.1f, 1024.0f);

        createVBOs();

        glActiveTexture(GL_TEXTURE0);
        textureId = uploadTextureData(textureData, textureWidth, textureHeight);
        glActiveTexture(GL_TEXTURE1);
        twinkleId = uploadTextureData(twinkleData, textureWidth, textureHeight);
        initStars();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glFrontFace(GL_CW);
        glDepthMask(true);

        return true;
    }

    void GLES2Lesson::resetTransformMatrices() {
    }

    void GLES2Lesson::fetchShaderLocations() {

        vertexAttributePosition = glGetAttribLocation(gProgram, "aPosition");
        modelMatrixAttributePosition = glGetUniformLocation(gProgram, "uModel");
        projectionMatrixAttributePosition = glGetUniformLocation(gProgram, "uProjection");
        samplerUniformPosition = glGetUniformLocation(gProgram, "sTexture");
        textureCoordinatesAttributePosition = glGetAttribLocation(gProgram, "aTexCoord");
        fragmentTintPosition = glGetUniformLocation(gProgram, "uTint");
    }

    void GLES2Lesson::drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                                   const glm::mat4 &transform) {

        glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
        glEnableVertexAttribArray(vertexAttributePosition);
        glEnableVertexAttribArray(textureCoordinatesAttributePosition);

        glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[0][0]);

        glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
        glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_TRUE,
                              sizeof(float) * 5, (void *) (sizeof(float) * 3));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
        glDrawElements(GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(vertexAttributePosition);
        glDisableVertexAttribArray(textureCoordinatesAttributePosition);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLES2Lesson::deleteVBOs() {
        glDeleteBuffers(1, &vboCubeVertexDataIndex);
        glDeleteBuffers(1, &vboCubeVertexIndicesIndex);
    }

    void GLES2Lesson::createVBOs() {
        glGenBuffers(1, &vboCubeVertexDataIndex);
        glBindBuffer(GL_ARRAY_BUFFER, vboCubeVertexDataIndex);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 5, cubeVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &vboCubeVertexIndicesIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCubeVertexIndicesIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), cubeIndices.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void GLES2Lesson::clearBuffers() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepthf(1.0f);
        checkGlError("glClearColor");
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        checkGlError("glClear");
    }

    void GLES2Lesson::setPerspective() {
        glUniformMatrix4fv(projectionMatrixAttributePosition, 1, false, &projectionMatrix[0][0]);
    }

    void GLES2Lesson::prepareShaderProgram() {
        glUseProgram(gProgram);
        checkGlError("glUseProgram");

        glUniform1i(samplerUniformPosition, twinkling ? 1 : 0);

        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glActiveTexture(GL_TEXTURE1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

    void GLES2Lesson::render() {
        clearBuffers();
        prepareShaderProgram();
        setPerspective();
        resetTransformMatrices();

        for (auto &star : mStars) {

            glUniform4fv(fragmentTintPosition, 1, &star->mColor[0]);

            drawGeometry(vboCubeVertexDataIndex,
                         vboCubeVertexIndicesIndex,
                         4,
                         star->mTransform
            );
        }
    }

    void GLES2Lesson::setTexture(int *bitmapData, int *detailData, int width, int height) {
        textureData = bitmapData;
        twinkleData = detailData;
        textureWidth = width;
        textureHeight = height;
    }

    void GLES2Lesson::tick() {

        movementPosition += movementDelta;
        rotationPosition += rotationDelta;

        float acc = 0.0f;
        float rads = ( 2.0f * M_PI) / 180.0f;
        glm::mat4 transform(1.0f);

        for (auto &star: mStars) {
            acc += 1.0f;
            transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, acc * rotationPosition * rads,
                                    glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::translate(transform,
                                       glm::vec3(acc, 0.0f,
                                                 -(movementPosition / PREFERED_NUMBER_OF_STARS) *
                                                 (PREFERED_NUMBER_OF_STARS - acc)));
            transform = glm::rotate(transform, acc,
                                    glm::vec3(0.0f, 0.0f, 1.0f));

            star->mTransform = transform;
        }
    }

    void GLES2Lesson::shutdown() {
        delete textureData;
        LOGI("Shutdown!\n");
    }

    void GLES2Lesson::reset() {
        movementPosition = 0.0f;
        movementDelta = 0.1f;
        rotationPosition = 0.0f;
        rotationDelta = 0.1f;
        twinkling = false;
    }

    void GLES2Lesson::initStars() {
        std::shared_ptr<Star> star;
        std::default_random_engine rndEngine;
        std::normal_distribution<float> distribution(0.0f, 1.0f);

        rndEngine.seed(time(nullptr));

        for (int i = 0; i < PREFERED_NUMBER_OF_STARS; ++i) {
            mStars.push_back(std::make_shared<Star>(glm::vec3(distribution(rndEngine), 0.0f, -i),
                                                    glm::vec4(distribution(rndEngine),
                                                              distribution(rndEngine),
                                                              distribution(rndEngine), 1.0f)));
        }
    }

    void GLES2Lesson::toggleTwinkling() {
        LOGI("Toggling twinkling");

        twinkling = !twinkling;
    }

    void GLES2Lesson::zoomIn() {
        LOGI("Zooming in");

        movementDelta -= 0.5f;
    }

    void GLES2Lesson::speedUpTwist() {
        LOGI("Speeding up twisting");

        rotationDelta += 0.5f;
    }

    void GLES2Lesson::zoomOut() {
        LOGI("Zooming out");

        movementDelta += 0.5f;
    }

    void GLES2Lesson::speedDownTwist() {
        LOGI("Speeding down twisting");

        rotationDelta -= 0.5f;
    }
}