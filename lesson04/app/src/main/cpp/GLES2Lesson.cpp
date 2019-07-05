//
// Created by monty on 23/11/15.
//

#include <GLES2/gl2.h>
#include <android/log.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include "GLES2Lesson.h"
#include "NdkGlue.h"

//Counter Clockwise
const float GLES2Lesson::triangleVertices[] {
//     0
//    / \
//   /   \
// 1/____ \2
//x, y, z, r, g, b
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

const unsigned short GLES2Lesson::triangleIndices[] {
    0, 1, 2
};

const float GLES2Lesson::squareVertices[]{
//  1___0
//  |   |
//  |   |
//  3___2
//x, y, z, r, g, b
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
};

const unsigned short GLES2Lesson::squareIndices[] {
  1, 3, 0, 2
};

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
    triangleTransformMatrix = glm::mat4( 1.0f );
    squareTransformMatrix = glm::mat4( 1.0f );
    projectionMatrix = glm::mat4( 1.0f );

    vertexAttributePosition = 0;
    colourAttributePosition = 0;
    modelMatrixAttributePosition = 0;
    projectionMatrixAttributePosition = 0;
    gProgram = 0;
    triangleRotationAngle = 0.0f;
    squareRotationAngle = 0.0f;
}

GLES2Lesson::~GLES2Lesson() {
    deleteVBOs();
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

    projectionMatrix = glm::perspective(45.0f, w / h, 0.1f, 100.0f );

    createVBOs();

    return true;
}

void GLES2Lesson::resetTransformMatrices() {
    float rads = ((M_PI) / 180.0f);
    triangleTransformMatrix = glm::rotate( glm::translate( glm::mat4( 1.0f ), glm::vec3( -1.5f, 0.0f, -6.0f ) ), triangleRotationAngle * rads, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    squareTransformMatrix = glm::rotate( glm::translate( glm::mat4( 1.0f ), glm::vec3( 1.5f, 0.0f, -6.0f ) ), squareRotationAngle * rads, glm::vec3( 1.0f, 0.0f, 0.0f ) );
}

void GLES2Lesson::fetchShaderLocations() {

    vertexAttributePosition = glGetAttribLocation( gProgram, "aPosition");
    colourAttributePosition = glGetAttribLocation( gProgram, "aColour");
    modelMatrixAttributePosition = glGetUniformLocation( gProgram, "uModel");
    projectionMatrixAttributePosition = glGetUniformLocation( gProgram, "uProjection");
}

void GLES2Lesson::drawGeometry( const int vertexVbo, const int indexVbo, int vertexCount, const glm::mat4& transform ) {

    glBindBuffer( GL_ARRAY_BUFFER, vertexVbo );

    glEnableVertexAttribArray(vertexAttributePosition);
    glEnableVertexAttribArray(colourAttributePosition);

    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[ 0 ][ 0 ]);
    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
    glVertexAttribPointer(colourAttributePosition, 3, GL_FLOAT, GL_TRUE, sizeof( float ) * 6, (void*)(sizeof(float ) * 3) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexVbo );
    glDrawElements( GL_TRIANGLE_STRIP, vertexCount, GL_UNSIGNED_SHORT, 0 );

    glDisableVertexAttribArray(vertexAttributePosition);
    glDisableVertexAttribArray(colourAttributePosition);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GLES2Lesson::deleteVBOs() {
    glDeleteBuffers( 1, &vboTriangleVertexDataIndex );
    glDeleteBuffers( 1, &vboTriangleVertexIndicesIndex );
    glDeleteBuffers( 1, &vboSquareVertexDataIndex );
    glDeleteBuffers( 1, &vboSquareVertexIndicesIndex );
}

void GLES2Lesson::createVBOs() {
    glGenBuffers( 1, &vboTriangleVertexDataIndex );
    glBindBuffer( GL_ARRAY_BUFFER, vboTriangleVertexDataIndex );
    glBufferData( GL_ARRAY_BUFFER, 3 * sizeof( float ) * 6, triangleVertices, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glGenBuffers( 1, &vboTriangleVertexIndicesIndex );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboTriangleVertexIndicesIndex );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof( GLushort ), triangleIndices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    glGenBuffers( 1, &vboSquareVertexDataIndex );
    glBindBuffer( GL_ARRAY_BUFFER, vboSquareVertexDataIndex );
    glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( float ) * 6, squareVertices, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glGenBuffers( 1, &vboSquareVertexIndicesIndex );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboSquareVertexIndicesIndex );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof( GLushort ), squareIndices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GLES2Lesson::clearBuffers() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");
}

void GLES2Lesson::setPerspective() {
    glUniformMatrix4fv(projectionMatrixAttributePosition, 1, false, &projectionMatrix[ 0 ][ 0 ]);
}

void GLES2Lesson::prepareShaderProgram() {
    glUseProgram(gProgram);
    checkGlError("glUseProgram");
}

void GLES2Lesson::render() {

    clearBuffers();
    prepareShaderProgram();
    setPerspective();
    resetTransformMatrices();

    drawGeometry( vboTriangleVertexDataIndex,
                  vboTriangleVertexIndicesIndex,
                  3,
                  triangleTransformMatrix
    );

    drawGeometry( vboSquareVertexDataIndex,
                  vboSquareVertexIndicesIndex,
                  4,
                  squareTransformMatrix
    );

}

void GLES2Lesson::tick() {
    triangleRotationAngle += 1.0f;
    squareRotationAngle += 1.0f;
}

void GLES2Lesson::shutdown() {
    LOGI("Shutdown!\n");
}