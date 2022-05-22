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
    attribute vec4 aColour;
    uniform mat4 uModel;
    uniform mat4 uProjection;
    varying vec4 vColour;
#endif

void main() {
    gl_Position =  uProjection * uModel * aPosition;
    vColour = aColour;
}
