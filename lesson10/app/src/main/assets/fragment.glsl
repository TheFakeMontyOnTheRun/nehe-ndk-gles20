precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform vec4 uTint;

void main() {
    vec4 texel = texture2D( sTexture, vTextureCoords );

    if ( texel.a < 0.1 ) {
        discard;
    }

    gl_FragColor = texel * uTint;
}
