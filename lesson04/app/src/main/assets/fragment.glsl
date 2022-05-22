#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    int vec4 vColour;
#else
    varying vec4 vColour;
#endif

void main() {
    gl_FragColor = vColour;
}
