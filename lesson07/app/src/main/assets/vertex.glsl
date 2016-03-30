attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aVertexNormal;
attribute vec3 aTangent;

varying vec3 vObjectSpaceTangent;
varying vec3 vObjectSpaceNormal;

uniform mat4 uModel;
uniform mat4 uProjection;
varying vec2 vTextureCoords;
uniform vec4 uDiffuseLightPosition;
varying vec4 vTransformedVertex;

void main() {
    vTextureCoords = aTexCoord;

    mat4 mvpMatrix = uProjection * uModel;

    vObjectSpaceNormal = aVertexNormal.xyz;
    vObjectSpaceTangent = aTangent;

    vTransformedVertex = mvpMatrix * aPosition;

    gl_Position =  vTransformedVertex;

}
