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
#include "WalkBouncer.h"
#include "NativeBitmap.h"
#include "Trig.h"
#include "TrigBatch.h"
#include "GLES2Lesson.h"
#include "NdkGlue.h"

namespace odb {

    void buildMipMap(NativeBitmap *pBitmap) {
        NativeBitmap *bitmap = pBitmap;
        NativeBitmap *old;
        int level = 1;
        while (bitmap->getWidth() > 1) {
            old = bitmap;
            bitmap = old->makeBitmapWithHalfDimensions();
            delete old;
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, bitmap->getWidth(), bitmap->getHeight(), 0,
                         GL_RGBA, GL_UNSIGNED_BYTE,
                         bitmap->getPixelData());
            ++level;

        }

        delete bitmap;
    }

    GLuint uploadTextureData(NativeBitmap *texture) {
        // Texture object handle
        GLuint textureId = 0;

        //Generate texture storage
        glGenTextures(1, &textureId);

        //specify what we want for that texture
        glBindTexture(GL_TEXTURE_2D, textureId);

        //upload the data
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->getWidth(), texture->getHeight(), 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     texture->getPixelData());

        buildMipMap(texture);

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
        mTexture = nullptr;
        vertexAttributePosition = 0;
        modelMatrixAttributePosition = 0;
        projectionMatrixAttributePosition = 0;
        mTrigBatch = nullptr;
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

        projectionMatrix = glm::perspective(45.0f, w / h, 0.1f, 100.0f);

        camera = glm::vec3(0.0f, 0.25f, 0.0f);

        glActiveTexture(GL_TEXTURE0);
        textureId = uploadTextureData(mTexture);

        glFrontFace(GL_CW);
        glDepthMask(true);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        angleXzInDegress = 0.0f;
        updateDirectionVector();
        return true;
    }

    void GLES2Lesson::resetTransformMatrices() {
        viewMatrix = glm::lookAt(camera, camera + cameraDirection,
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
    }

    void GLES2Lesson::render() {
        clearBuffers();
        prepareShaderProgram();
        setPerspective();
        resetTransformMatrices();

        glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &glm::mat4(1.0f)[0][0]);
        checkGlError("before drawing");

        if (mTrigBatch != nullptr) {
            glEnableVertexAttribArray(vertexAttributePosition);
            glEnableVertexAttribArray(textureCoordinatesAttributePosition);
            mTrigBatch->draw(vertexAttributePosition, textureCoordinatesAttributePosition);
            glDisableVertexAttribArray(vertexAttributePosition);
            glDisableVertexAttribArray(textureCoordinatesAttributePosition);
        }

        checkGlError("after drawing");
    }

    void GLES2Lesson::setTexture(NativeBitmap *texture) {
        mTexture = texture;
    }

    void GLES2Lesson::tick() {
    }

    void GLES2Lesson::shutdown() {
        delete mTexture;
        LOGI("Shutdown!\n");
    }

    void GLES2Lesson::reset() {
    }

    void GLES2Lesson::addTrigs(std::vector<Trig> newTrigs) {
        mTrigBatch = new TrigBatch(newTrigs);
    }

    void GLES2Lesson::moveForward(float factor) {
        mBouncer.tick(20);
        camera += cameraDirection * factor;
        camera.y = mBouncer.getCurrentStep();
    }

    void GLES2Lesson::moveBackward(float factor) {
        mBouncer.tick(-20);
        camera -= cameraDirection * factor;
        camera.y = mBouncer.getCurrentStep();
    }

    void GLES2Lesson::turnRight(float factor) {
        angleXzInDegress -= factor;
        updateDirectionVector();
    }

    void GLES2Lesson::turnLeft(float factor) {
        angleXzInDegress += factor;
        updateDirectionVector();
    }

    void GLES2Lesson::updateDirectionVector() {

        float angleInRadians = angleXzInDegress * (3.14159f / 180.0f);

        cameraDirection = glm::vec3(sin(angleInRadians), 0.0f, cos(angleInRadians));
    }

    void GLES2Lesson::onNormalizedTouch(float x, float y) {
        if (x < 0.25f) {
            turnLeft(1.5f);
        } else if (x > 0.75f) {
            turnRight(1.5f);
        }

        if (y < 0.25f) {
            moveForward(0.1f);
        } else if (y > 0.75f) {
            moveBackward(0.1f);
        }
    }
}