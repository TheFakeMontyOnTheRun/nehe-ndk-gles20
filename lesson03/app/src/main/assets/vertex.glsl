#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    in vec4 aPosition;
    in mat4 uModel;
    in mat4 uProjection;
    in vec4 aColour;
    out vec4 vColour;
#else
    attribute vec4 aPosition;
    uniform mat4 uModel;
    uniform mat4 uProjection;
    attribute vec4 aColour;
    varying vec4 vColour;
#endif

void main() {
    gl_Position =  uProjection * uModel * aPosition;
    vColour = aColour;
}
