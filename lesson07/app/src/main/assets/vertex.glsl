#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
    in vec4 aPosition;
    in vec2 aTexCoord;
    in vec4 aVertexNormal;
    in mat4 uModel;
    in mat4 uProjection;
    out vec2 vTextureCoords;
    out float vLightIntensity;
    int vec4 uDiffuseLightDirection;
#else
    attribute vec4 aPosition;
    attribute vec2 aTexCoord;
    attribute vec4 aVertexNormal;
    uniform mat4 uModel;
    uniform mat4 uProjection;
    varying vec2 vTextureCoords;
    varying float vLightIntensity;
    uniform vec4 uDiffuseLightDirection;
#endif

void main() {
    mat4 mvpMatrix = uProjection * uModel;
    vec4 vNormal = mvpMatrix * aVertexNormal;
    gl_Position =  mvpMatrix * aPosition;
    vTextureCoords = aTexCoord;
    vLightIntensity =  max( dot( uDiffuseLightDirection.xyz, vNormal.xyz), 0.0 );
}
