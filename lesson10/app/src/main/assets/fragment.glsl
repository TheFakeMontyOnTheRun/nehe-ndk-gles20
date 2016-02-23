precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;

void main() {
    vec4 texel = texture2D( sTexture, vTextureCoords );

    gl_FragColor = texel;
}
