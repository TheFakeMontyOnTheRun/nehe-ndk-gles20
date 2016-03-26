attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aVertexNormal;
attribute vec3 axisX;
varying vec3 vAxisX;
varying vec3 vObjectSpaceAxisX;

uniform mat4 uModel;
uniform mat4 uProjection;
varying vec2 vTextureCoords;
uniform vec4 uDiffuseLightDirection;
varying vec4 vNormal;
varying vec4 vTransformedVertex;
varying vec3 vObjectSpaceNormal;

void main() {
    mat4 mvpMatrix = uProjection * uModel;
    vNormal = uModel * aVertexNormal;
    vObjectSpaceNormal = aVertexNormal.xyz;
    vAxisX = ( uModel * vec4( axisX, 0.0 ) ).xyz;
    vObjectSpaceAxisX = axisX;
    vTransformedVertex = mvpMatrix * aPosition;
    gl_Position =  vTransformedVertex;
    vTextureCoords = aTexCoord;
}
