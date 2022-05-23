#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    in vec4 vColour;
    in vec2 vTextureCoords;
    in sampler2D sTexture;
#else
    varying vec2 vTextureCoords;
    varying vec4 vColour;
    uniform sampler2D sTexture;
#endif


void main() {
    gl_FragColor = texture2D( sTexture, vTextureCoords );
}
