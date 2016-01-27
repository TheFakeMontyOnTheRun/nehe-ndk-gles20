precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform vec4 uTint;

void main() {
    gl_FragColor = texture2D( sTexture, vTextureCoords );
    vec4 texel = texture2D( sTexture, vTextureCoords );


    gl_FragColor = texel * uTint;
}
