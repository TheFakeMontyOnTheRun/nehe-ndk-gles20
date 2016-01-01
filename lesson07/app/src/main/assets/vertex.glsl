attribute vec4 aPosition;
attribute vec4 aColour;
attribute vec2 aTexCoord;
uniform mat4 uModel;
uniform mat4 uProjection;
varying vec2 vTextureCoords;
varying vec4 vColour;

void main() {
    gl_Position =  uProjection * uModel * aPosition;
    vColour = aColour;
    vTextureCoords = aTexCoord;
}
