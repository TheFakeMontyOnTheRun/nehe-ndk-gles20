//
// Created by monty on 2015/02/06
//

#include <GLES2/gl2.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>
#include <vector>
#include <random>
#include <android/log.h>

#include "Trig.h"
#include "GLES2Lesson.h"
#include "NdkGlue.h"

namespace odb {
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
        GLuint shader = glCreateShader(shaderType);
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
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
        if (!vertexShader) {
            return 0;
        }

        GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (!pixelShader) {
            return 0;
        }

        GLuint program = glCreateProgram();
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


        glActiveTexture(GL_TEXTURE0);
        textureId = uploadTextureData(textureData, textureWidth, textureHeight);

        glFrontFace(GL_CW);
        glDepthMask(true);

        return true;
    }

    void GLES2Lesson::resetTransformMatrices() {
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.25f, 0.0f), glm::vec3(0.0f, 0.25f, 1.0f),
                                 glm::vec3(0.0, 1.0f, 0.0f));
        glUniformMatrix4fv(viewMatrixAttributePosition, 1, false, &viewMatrix[0][0]);
    }

    void GLES2Lesson::fetchShaderLocations() {

        vertexAttributePosition = glGetAttribLocation(gProgram, "aPosition");
        modelMatrixAttributePosition = glGetUniformLocation(gProgram, "uModel");
        viewMatrixAttributePosition = glGetUniformLocation(gProgram, "uView");
        projectionMatrixAttributePosition = glGetUniformLocation(gProgram, "uProjection");
        samplerUniformPosition = glGetUniformLocation(gProgram, "sTexture");
        textureCoordinatesAttributePosition = glGetAttribLocation(gProgram, "aTexCoord");
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

        glUniform1i(samplerUniformPosition, 0);

        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void GLES2Lesson::render() {
        clearBuffers();
        prepareShaderProgram();
        setPerspective();
        resetTransformMatrices();

        glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &glm::mat4(1.0f)[0][0]);

        for (auto &trig : mTrigs) {
            drawTrig(trig, glm::mat4(1.0f));
        }
    }

    void GLES2Lesson::setTexture(int *bitmapData, int width, int height, int format) {
        textureData = bitmapData;
        textureWidth = width;
        textureHeight = height;
    }

    void GLES2Lesson::tick() {
    }

    void GLES2Lesson::shutdown() {
        delete textureData;
        LOGI("Shutdown!\n");
    }

    void GLES2Lesson::reset() {
    }

    void GLES2Lesson::addTrigs(std::vector<Trig> newTrigs) {
        mTrigs.insert(mTrigs.end(), newTrigs.begin(), newTrigs.end());
    }

    void GLES2Lesson::drawTrig(Trig &trig, const glm::mat4 &transform) {
        glEnableVertexAttribArray(vertexAttributePosition);
        glEnableVertexAttribArray(textureCoordinatesAttributePosition);

        checkGlError("enable attribute");
        glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[0][0]);
        checkGlError("upload model matrix");
        glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_TRUE, 0,
                              trig.getVertexData());
        checkGlError("upload vertex data");
        glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_TRUE,
                              0, trig.getUVData());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        checkGlError("draw");
        glDisableVertexAttribArray(vertexAttributePosition);
        checkGlError("disable attribute");
        glDisableVertexAttribArray(textureCoordinatesAttributePosition);
    }
}