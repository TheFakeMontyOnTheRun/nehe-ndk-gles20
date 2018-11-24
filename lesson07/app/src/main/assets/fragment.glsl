precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform vec4 uAmbientLightColor;
uniform vec4 uDiffuseLightColor;
uniform vec4 uDiffuseLightDirection;
uniform mat4 uNormalMatrix;
varying vec4 vNormal;
uniform mat4 uProjection;
varying vec4 vTransformedVertex;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightColor, const in vec3 normal, const in vec3 halfVec, const in vec4 diffuse, const in vec4 specular, const in float shininess) {

        float nDotL = dot(normal, direction)  ;
        vec4 lambert = diffuse * lightColor * max (nDotL, 0.0) ;

        float nDotH = dot(normal, halfVec) ;
        vec4 phong = specular * lightColor * pow (max(nDotH, 0.0), shininess) ;

        return lambert + phong;
}


void main() {
    vec3 direction0 = vec3( 0.0, 0.0, -1.0 );
    vec4 light0posn = vec4( 0.0, 0.0, 0.0, 1.0);
    vec4 light0color = uDiffuseLightColor;

    vec4 texel = texture2D( sTexture, vTextureCoords );

    vec4 interpolatedNormal = vNormal;
    mat4 normalMatrix = uNormalMatrix;

    vec3 dehomogenizedPosition = vTransformedVertex.xyz / vTransformedVertex.w;

    vec3 normal = normalize(interpolatedNormal.xyz);

    if ( light0posn.w == 1.0 ) {
        vec3 position0 = light0posn.xyz / light0posn.w;
        direction0 = normalize (position0 - dehomogenizedPosition);
   	} else {
   		direction0 = normalize (vec3(light0posn.x, light0posn.y, light0posn.z ) );
   	}

    const vec3 origin = vec3(0,0,0);
   	vec3 eyedirn = normalize(origin - dehomogenizedPosition);

   	vec3 half0 = normalize (direction0 + eyedirn) ;
    gl_FragColor = texel * ( uAmbientLightColor + ComputeLight(direction0, light0color, normal, half0, vec4( 0.4, 0.8, 0.4, 1.0), vec4( 0.0, 0.4, 0.4, 1.0 ), 300.0) );
}
