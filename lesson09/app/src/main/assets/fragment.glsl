#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    in vec2 vTextureCoords;
    in sampler2D sTexture;
    in vec4 uTint;
#else
     varying vec2 vTextureCoords;
     uniform sampler2D sTexture;
     uniform vec4 uTint;
#endif

void main() {
    vec4 texel = texture2D( sTexture, vTextureCoords );

    if ( texel.a < 0.1 ) {
        discard;
    }

    gl_FragColor = texel * uTint;
}
