#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    in vec2 vTextureCoords;
    in sampler2D sTexture;
    in vec4 uAmbientLightColor;
    in vec4 uDiffuseLightColor;
    in float vLightIntensity;
#else
    varying vec2 vTextureCoords;
    uniform sampler2D sTexture;
    uniform vec4 uAmbientLightColor;
    uniform vec4 uDiffuseLightColor;
    varying float vLightIntensity;
#endif

void main() {
    gl_FragColor = texture2D( sTexture, vTextureCoords ) * ( uAmbientLightColor + ( vLightIntensity * uDiffuseLightColor ) );
}
