attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aVertexNormal;
uniform mat4 uModel;
uniform mat4 uProjection;
varying vec2 vTextureCoords;
varying float vLightIntensity;
uniform vec4 uDiffuseLightDirection;

void main() {
    mat4 mvpMatrix = uProjection * uModel;
    vec4 vNormal = mvpMatrix * aVertexNormal;
    gl_Position =  mvpMatrix * aPosition;
    vTextureCoords = aTexCoord;
    vLightIntensity =  max( dot( uDiffuseLightDirection.xyz, vNormal.xyz), 0.0 );
}
