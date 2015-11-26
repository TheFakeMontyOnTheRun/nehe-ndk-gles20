//
// Created by monty on 23/11/15.
//

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include "GLES2Lesson.h"
#include "NdkGlue.h"

GLuint GLES2Lesson::vertexAttributePosition = 0;
GLuint GLES2Lesson::modelMatrixAttributePosition = 0;
GLuint GLES2Lesson::projectionMatrixAttributePosition = 0;
GLuint GLES2Lesson::gProgram = 0;


//Counter Clockwise
float GLES2Lesson::triangleVertices[] = {
//     0
//    / \
//   /   \
// 1/____ \2

        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
};

float triangleColours[] {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
};

unsigned short triangleIndices[] {
    0, 1, 2
};

float squareColours[] {
     1.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 0.0f,
     1.0f, 0.0f, 0.0f
};

float GLES2Lesson::squareVertices[]{
//  1___0
//  |   |
//  |   |
//  3___2
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f
};

unsigned short squareIndices[] {
  1, 3, 0, 2
};

int colourAttributePosition;

//start off as identity - late we will init it with proper values.
glm::mat4 GLES2Lesson::triangleTransformMatrix = glm::mat4( 1.0f );
glm::mat4 GLES2Lesson::squareTransformMatrix = glm::mat4( 1.0f );
glm::mat4 GLES2Lesson::projectionMatrix = glm::mat4( 1.0f );

extern void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

extern void checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
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
    colourAttributePosition = glGetAttribLocation(GLES2Lesson::gProgram, "aColour");

    GLES2Lesson::modelMatrixAttributePosition = glGetUniformLocation(GLES2Lesson::gProgram,
                                                                     "uModel");
    GLES2Lesson::projectionMatrixAttributePosition = glGetUniformLocation(GLES2Lesson::gProgram,
                                                                          "uProjection");
    glEnableVertexAttribArray(vertexAttributePosition);
    glEnableVertexAttribArray(colourAttributePosition);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    //glTranslatef( -1.5f, 0.0f, -6.0f);
    GLES2Lesson::triangleTransformMatrix = glm::translate( glm::mat4(1.0f), glm::vec3( -1.5f, 0.0f, -6.0f ) );

    //glTranslatef( -1.5f, 0.0f, -6.0f);
    //glTranslatef(3.0f, 0.0f, 0.0f );
    //= glTranslate( 1.5f, 0.0, -6.0f );
    GLES2Lesson::squareTransformMatrix = glm::translate( glm::mat4(1.0f), glm::vec3( 1.5f, 0.0f, -6.0f ) );
    GLES2Lesson::projectionMatrix = glm::perspective(45.0f, ((float) w) / ((float) h), 0.1f, 100.0f );

    return true;
}

void GLES2Lesson::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glUniformMatrix4fv(projectionMatrixAttributePosition, 1, false, &GLES2Lesson::projectionMatrix[ 0 ][ 0 ]);

    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false,
                       &GLES2Lesson::triangleTransformMatrix[ 0 ][ 0 ]);
    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 0,
                          GLES2Lesson::triangleVertices);
    glVertexAttribPointer(colourAttributePosition, 3, GL_FLOAT, GL_TRUE, 0,
                          triangleColours);


    glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, triangleIndices );

    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &GLES2Lesson::squareTransformMatrix[ 0 ][ 0 ]);
    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, 0,
                          GLES2Lesson::squareVertices);

    glVertexAttribPointer(colourAttributePosition, 3, GL_FLOAT, GL_TRUE, 0,
                          squareColours);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, squareIndices);
}

void GLES2Lesson::shutdown() {
    LOGI("Shutdown!\n");
}
