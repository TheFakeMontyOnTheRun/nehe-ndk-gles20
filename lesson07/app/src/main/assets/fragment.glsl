precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform sampler2D sNormalMap;
uniform vec4 uAmbientLightColor;
uniform vec4 uDiffuseLightColor;
varying float vLightIntensity;

void main() {
    vec4 normal = texture2D( sNormalMap, vTextureCoords );
    vec4 texel = texture2D( sTexture, vTextureCoords );
    gl_FragColor = texel * ( uAmbientLightColor + ( vLightIntensity * uDiffuseLightColor ) );
}
