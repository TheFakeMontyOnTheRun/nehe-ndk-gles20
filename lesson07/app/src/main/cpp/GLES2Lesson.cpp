//
// Created by monty on 23/11/15.
//

#include <GLES2/gl2.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>

#include <android/log.h>

#include "GLES2Lesson.h"
#include "NdkGlue.h"

namespace odb {
//Counter Clockwise
    const float GLES2Lesson::cubeVertices[]{
//    4________5
//    /|       /|
//   / |      / |
// 0/__|___1_/  |
//  | 7|____|___|6
//  |  /    |  /
//  | /     | /
// 3|/______|/2
//x, y, z, u, v, nx, ny, nz, nw

            //FLOOR
            1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,  //6 - 0
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,  //7 - 1
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  //2 - 2
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //3 - 3
            //CEILING
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  //0 - 4
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   //1 - 5
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //4 - 6
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //5 - 7
            //SOUTH
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //0 - 8
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //1 - 9
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //2 - 10
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //3 - 11
            //NORTH
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, //4 - 12
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, //5 - 13
            1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,  //6 - 14
            -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,  //7 -15
            //EAST
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,   //1 - 16
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //2 - 17
            1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //5 - 18
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //6 - 19
            //WEST
            -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //0 - 20
            -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //3 - 21
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //4 - 22
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f  //7 - 23
    };


    const unsigned short GLES2Lesson::cubeIndices[]{
            20, 21, 23,
            22, 20, 23,
            16, 18, 19,
            16, 19, 17,
            13, 12, 14,
            14, 12, 15,
            8, 9, 10,
            8, 10, 11,
            6, 7, 5,
            6, 5, 4,
            3, 2, 1,
            1, 0, 2
    };


    const glm::vec4 GLES2Lesson::ambientLightFullColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    const glm::vec4 GLES2Lesson::ambientLightOffColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

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
//start off as identity - late we will init it with proper values.
        cubeTransformMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::mat4(1.0f);
        textureData = nullptr;
        vertexAttributePosition = 0;
        modelMatrixAttributePosition = 0;
        projectionMatrixAttributePosition = 0;
        gProgram = 0;
        currentFilter = GL_NEAREST;
        diffuseLightDirection = glm::normalize(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
        diffuseLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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

        projectionMatrix = glm::perspective(45.0f, w / h, 0.1f, 100.0f);

        createVBOs();

        glActiveTexture(GL_TEXTURE0);
        textureId = uploadTextureData(textureData, textureWidth, textureHeight);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glFrontFace(GL_CW);
        glDepthMask(true);
        return true;
    }

    void GLES2Lesson::resetTransformMatrices() {
        glm::mat4 identity = glm::mat4(1.0f);
        glm::vec3 translate = glm::vec3(0.0f, 0.0f, -6.0f);
        glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 translated = glm::translate(identity, translate);
        float rads = (M_PI / 180.0f);
        glm::mat4 rotatedAroundXAxis = glm::rotate(translated, cubeRotationAngleYZ * rads, xAxis);
        glm::mat4 rotatedAroundYAxis = glm::rotate(rotatedAroundXAxis, cubeRotationAngleXZ * rads, yAxis);
        cubeTransformMatrix = rotatedAroundYAxis;
    }

    void GLES2Lesson::fetchShaderLocations() {

        vertexAttributePosition = glGetAttribLocation(gProgram, "aPosition");
        modelMatrixAttributePosition = glGetUniformLocation(gProgram, "uModel");
        projectionMatrixAttributePosition = glGetUniformLocation(gProgram, "uProjection");
        samplerUniformPosition = glGetUniformLocation(gProgram, "sTexture");
        textureCoordinatesAttributePosition = glGetAttribLocation(gProgram, "aTexCoord");

        ambientLightColorShaderLocation = glGetUniformLocation(gProgram, "uAmbientLightColor");
        diffuseLightColorShaderLocation = glGetUniformLocation(gProgram, "uDiffuseLightColor");
        diffuseLightDirectionShaderLocation = glGetUniformLocation(gProgram,
                                                                   "uDiffuseLightDirection");
        normalAttributePosition = glGetAttribLocation(gProgram, "aVertexNormal");
    }

    void GLES2Lesson::drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
                                   const glm::mat4 &transform) {

        glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
        glEnableVertexAttribArray(vertexAttributePosition);
        glEnableVertexAttribArray(textureCoordinatesAttributePosition);
        glEnableVertexAttribArray(normalAttributePosition);

        glUniform4fv(ambientLightColorShaderLocation, 1, mFullLight ? &ambientLightFullColor[0] : &ambientLightOffColor[0]);

        glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[0][0]);

        glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
        glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_TRUE,
                              sizeof(float) * 9, (void *) (sizeof(float) * 3));
        glVertexAttribPointer(normalAttributePosition, 4, GL_FLOAT, GL_TRUE,
                              sizeof(float) * 9, (void *) (sizeof(float) * 5));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
        glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(vertexAttributePosition);
        glDisableVertexAttribArray(textureCoordinatesAttributePosition);
        glDisableVertexAttribArray(normalAttributePosition);

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
        glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float) * 9, cubeVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &vboCubeVertexIndicesIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCubeVertexIndicesIndex);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLushort), cubeIndices, GL_STATIC_DRAW);
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

        glUniform4fv(diffuseLightColorShaderLocation, 1, &diffuseLightColor[0]);
        glUniform4fv(diffuseLightDirectionShaderLocation, 1, &diffuseLightDirection[0]);
        glUniform4fv(ambientLightColorShaderLocation, 1, mFullLight ? &ambientLightFullColor[0] : &ambientLightOffColor[0]);

        glUniform1i(samplerUniformPosition, 0);

        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, currentFilter);
    }

    void GLES2Lesson::render() {
        clearBuffers();
        prepareShaderProgram();
        setPerspective();
        resetTransformMatrices();

        drawGeometry(vboCubeVertexDataIndex,
                     vboCubeVertexIndicesIndex,
                     36,
                     cubeTransformMatrix
        );
    }

    void GLES2Lesson::setTexture(int *bitmapData, int width, int height) {
        textureData = bitmapData;
        textureWidth = width;
        textureHeight = height;
    }

    void GLES2Lesson::tick() {
        cubeRotationAngleYZ += rotationYZSpeed;
        cubeRotationAngleXZ += rotationXZSpeed;
    }

    void GLES2Lesson::shutdown() {
        delete textureData;
        LOGI("Shutdown!\n");
    }

    void GLES2Lesson::toggleFiltering() {
        if (currentFilter == GL_NEAREST) {
            currentFilter = GL_LINEAR;
            LOGI("Using GL_LINEAR\n");
        } else {
            currentFilter = GL_NEAREST;
            LOGI("Using GL_NEAREST\n");
        }
    }

    void GLES2Lesson::toggleLightning() {
        mFullLight = !mFullLight;
    }

    void GLES2Lesson::speedUpXZ() {
        rotationXZSpeed += 0.125f;
    }

    void GLES2Lesson::speedDownXZ() {
        rotationXZSpeed -= 0.125f;
    }

    void GLES2Lesson::speedUpYZ() {
        rotationYZSpeed += 0.125f;
    }

    void GLES2Lesson::speedDownYZ() {
        rotationYZSpeed -= 0.125f;
    }

    void GLES2Lesson::reset() {
        cubeRotationAngleYZ = 0.0f;
        rotationYZSpeed = 0.0f;
        cubeRotationAngleXZ = 0.0f;
        rotationXZSpeed = 0.0f;
    }

    void GLES2Lesson::setSpeeds(const glm::vec2 &velocity) {
        rotationXZSpeed = -velocity.x;
        rotationYZSpeed = -velocity.y;
    }
}