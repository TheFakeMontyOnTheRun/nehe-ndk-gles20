#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec4 aPosition;
    int vec4 vColour;
#else
    varying vec4 vColour;
#endif

void main() {
    gl_FragColor = vColour;
}
