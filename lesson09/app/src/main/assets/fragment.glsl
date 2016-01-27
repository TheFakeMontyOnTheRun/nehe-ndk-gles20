precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;

void main() {
    gl_FragColor = texture2D( sTexture, vTextureCoords );
}
