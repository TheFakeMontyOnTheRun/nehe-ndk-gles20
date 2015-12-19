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
//x, y, z, r, g, b, u, v
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,    //0
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f,     //1
        1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,   //2
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,   //3
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,   //4
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f,    //5
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f,   //6
        -1.0f, -1.0f, -1.0f, 0.0f, 1.0f   //
};

const unsigned short GLES2Lesson::cubeIndices[] {
  0, 1, 2,
  0, 2, 3,

  5, 4, 7,
  5, 7, 6,

  1, 5, 6,
  1, 6, 2,

  4, 0, 7,
  0, 3, 7,

  4, 5, 1,
  0, 4, 1,

  6, 7, 2,
  2, 7, 3

};

GLuint CreateSimpleTexture2D( )
{
    // Texture object handle
    GLuint textureId = 0;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
            {
                    255,   0,   0, // Red
                    0, 255,   0, // Green
                    0,   0, 255, // Blue
                    255, 255,   0  // Yellow
            };

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &textureId );

    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );

    // Load the texture
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

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
//start off as identity - late we will init it with proper values.
    cubeTransformMatrix = glm::mat4( 1.0f );
    projectionMatrix = glm::mat4( 1.0f );

    vertexAttributePosition = 0;
    modelMatrixAttributePosition = 0;
    projectionMatrixAttributePosition = 0;
    gProgram = 0;
    cubeRotationAngleYZ = 0.0f;
    cubeRotationAngleXZ = 0.0f;
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
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glFrontFace( GL_CW );
    glDepthMask( true );
    return true;
}

void GLES2Lesson::resetTransformMatrices() {
    glm::mat4 identity = glm::mat4( 1.0f );
    glm::vec3 translate = glm::vec3( 0.0f, 0.0f, -6.0f );
    glm::vec3 xAxis = glm::vec3( 1.0f, 0.0f, 0.0f );
    glm::vec3 yAxis = glm::vec3( 0.0f, 1.0f, 0.0f );
    glm::mat4 translated = glm::translate( identity, translate );
    glm::mat4 rotatedAroundXAxis = glm::rotate( translated, cubeRotationAngleYZ, xAxis );
    glm::mat4 rotatedAroundYAxis = glm::rotate( rotatedAroundXAxis, cubeRotationAngleXZ, yAxis );
    cubeTransformMatrix = rotatedAroundYAxis;
}

void GLES2Lesson::fetchShaderLocations() {

    vertexAttributePosition = glGetAttribLocation( gProgram, "aPosition");
    modelMatrixAttributePosition = glGetUniformLocation( gProgram, "uModel");
    projectionMatrixAttributePosition = glGetUniformLocation( gProgram, "uProjection");
    samplerUniformPosition = glGetUniformLocation( gProgram, "sTexture" );
    textureCoordinatesAttributePosition = glGetAttribLocation( gProgram, "aTexCoord" );
}

void GLES2Lesson::drawGeometry( const int vertexVbo, const int indexVbo, int vertexCount, const glm::mat4& transform ) {

    glBindBuffer( GL_ARRAY_BUFFER, vertexVbo );

    glEnableVertexAttribArray(vertexAttributePosition);
    glEnableVertexAttribArray(textureCoordinatesAttributePosition);

    glUniform1i( samplerUniformPosition, 0 );
    glUniformMatrix4fv(modelMatrixAttributePosition, 1, false, &transform[ 0 ][ 0 ]);
    glVertexAttribPointer(vertexAttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, 0 );
    glVertexAttribPointer(textureCoordinatesAttributePosition, 2, GL_FLOAT, GL_TRUE, sizeof( float ) * 5, (void*)(sizeof(float ) * 3) );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexVbo );
    glDrawElements( GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, 0 );

    glDisableVertexAttribArray(vertexAttributePosition);
    glDisableVertexAttribArray(textureCoordinatesAttributePosition);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GLES2Lesson::deleteVBOs() {
    glDeleteBuffers( 1, &vboCubeVertexDataIndex );
    glDeleteBuffers( 1, &vboCubeVertexIndicesIndex );
}

void GLES2Lesson::createVBOs() {
    glGenBuffers( 1, &vboCubeVertexDataIndex );
    glBindBuffer( GL_ARRAY_BUFFER, vboCubeVertexDataIndex );
    glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( float ) * 5, cubeVertices, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    textureId = CreateSimpleTexture2D();

    glGenBuffers( 1, &vboCubeVertexIndicesIndex );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboCubeVertexIndicesIndex );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof( GLushort ), cubeIndices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GLES2Lesson::clearBuffers() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepthf( 1.0f );
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

    drawGeometry( vboCubeVertexDataIndex,
                  vboCubeVertexIndicesIndex,
                  36,
                  cubeTransformMatrix
    );
}

void GLES2Lesson::setTexture( void **bitmapData, int size ) {
}

void GLES2Lesson::tick() {
    cubeRotationAngleYZ += 0.0f;
    cubeRotationAngleXZ += 1.0f;
}

void GLES2Lesson::shutdown() {
    LOGI("Shutdown!\n");
}