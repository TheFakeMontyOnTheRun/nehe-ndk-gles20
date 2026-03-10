attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aVertexNormal;
uniform mat4 uModel;
uniform mat4 uProjection;
varying vec2 vTextureCoords;
uniform vec4 uDiffuseLightDirection;
varying vec4 vNormal;
varying vec4 vTransformedVertex;

void main() {
    mat4 mvpMatrix = uProjection * uModel;
    vNormal = mvpMatrix * aVertexNormal;
    vTransformedVertex = mvpMatrix * aPosition;
    gl_Position =  vTransformedVertex;
    vTextureCoords = aTexCoord;
}
