//
// Created by monty on 23/11/15.
//

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <string>
#include "GLES2Lesson.h"
#include "NdkGlue.h"

GLuint GLES2Lesson::vertexAttributePosition = 0;
GLuint GLES2Lesson::modelMatrixAttributePosition = 0;
GLuint GLES2Lesson::projectionMatrixAttributePosition = 0;
GLuint GLES2Lesson::gProgram = 0;


//Counter Clockwise
float GLES2Lesson::triangleVertices[] = {
//     1
//    / \
//   /   \
// 2/____ \3

        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
};

float GLES2Lesson::squareVertices[]{
//  2___1
//  |   |
//  |   |
//  |___3
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
//  2____
//  |   |
//  |   |
//  3___1
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f
};

//glTranslatef( -1.5f, 0.0f, -6.0f);
float GLES2Lesson::triangleTransformMatrix[]{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -1.5f, 0.0f, -6.0f, 1.0f
};

//glTranslatef( -1.5f, 0.0f, -6.0f);
//glTranslatef(3.0f, 0.0f, 0.0f );
//= glTranslate( 1.5f, 0.0, -6.0f );
float GLES2Lesson::squareTransformMatrix[]{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.5f, 0.0f, -6.0f, 1.0f
};

//We start off with a identity and later will fill in for the projection space transform..
float GLES2Lesson::projectionMatrix[]{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
};

float radians(float degrees) {
    return degrees * (3.14159f / 180.0f);
}

float cotangent(float angle) {
    return 1.0f / tan(radians(angle));
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


void perspective(float fovy, float aspect, float zNear, float zFar) {

    float A;
    float B;
    float d;

    A = (2.0f * zNear * zFar) / (zFar - zNear);
    B = (zFar + zNear) / (zFar - zNear);
    d = cotangent(fovy / 2.0f);

    GLES2Lesson::projectionMatrix[0] = d / aspect;
    GLES2Lesson::projectionMatrix[5] = d;
    GLES2Lesson::projectionMatrix[10] = -B;
    GLES2Lesson::projectionMatrix[11] = -1.0f;
    GLES2Lesson::projectionMatrix[14] = -A;
}

GLuint loadShader(GLenum shaderType, const char *pSource) {
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

GLuint createProgram(const char *pVertexSource, const char *pFragmentSource) {
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


bool GLES2Lesson::init(float w, float h, const std::string &vertexShader,
                       const std::string &fragmentShader) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    GLES2Lesson::gProgram = createProgram(vertexShader.c_str(), fragmentShader.c_str());
    if (!GLES2Lesson::gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    GLES2Lesson::vertexAttributePosition = glGetAttribLocation(GLES2Lesson::gProgram, "aPosition");
    GLES2Lesson::modelMatrixAttributePosition = glGetUniformLocation(GLES2Lesson::gProgram,
                                                                     "uModel");
    GLES2Lesson::projectionMatrixAttributePosition = glGetUniformLocation(GLES2Lesson::gProgram,
                                                                          "uProjection");

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    perspective(45.0f, ((float) w) / ((float) h), 0.1f, 100.0f);

    return true;
}

void GLES2Lesson::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glUniformMatrix4fv(projectionMatrixAttributePosition, 1, false, GLES2Lesson::projectionMatrix);

    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false,
                       GLES2Lesson::triangleTransformMatrix);
    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 0,
                          GLES2Lesson::triangleVertices);
    glEnableVertexAttribArray(vertexAttributePosition);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, GLES2Lesson::squareTransformMatrix);
    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 0,
                          GLES2Lesson::squareVertices);
    glEnableVertexAttribArray(vertexAttributePosition);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GLES2Lesson::shutdown() {

}