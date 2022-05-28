#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    in vec4 aPosition;
    in vec2 aTexCoord;
    in mat4 uModel;
    in mat4 uProjection;
    out vec2 vTextureCoords;
#else
    attribute vec4 aPosition;
    attribute vec2 aTexCoord;
    uniform mat4 uModel;
    uniform mat4 uProjection;
    varying vec2 vTextureCoords;
#endif

void main() {
    gl_Position =  uProjection * uModel * aPosition;
    vTextureCoords = aTexCoord;
}
