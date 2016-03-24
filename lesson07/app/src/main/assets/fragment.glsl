precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform sampler2D sNormalMap;
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
    vec4 interpolatedNormal = vNormal;
    vec4 normalFromTexel = vec4( texture2D( sNormalMap, vTextureCoords ).xyz, 0.0f );
    vec4 texel = texture2D( sTexture, vTextureCoords );
    float intensity =  max( dot( uDiffuseLightDirection.xyz, vNormal.xyz), 0.0 );

    mat4 normalMatrix = uNormalMatrix;
    vec4 finalcolor;
    const vec3 origin = vec3(0,0,0);
    vec4 _mypos = vTransformedVertex;
    vec3 mypos = _mypos.xyz / _mypos.w;
    vec3 eyedirn = normalize(origin - mypos);
    vec3 _normal = ( interpolatedNormal ).xyz ;
    vec3 normal = normalize(_normal) ;
    vec3 direction0 = vec3( -1.0, 0.0, 0.0 );
    vec4 light0posn = vec4( 3.0, 0.0 , 0.0, 1.0 );
    vec4 light0color = uDiffuseLightColor;

    if ( light0posn.w == 1.0 ) {
        vec3 position0 = light0posn.xyz / light0posn.w;
        direction0 = normalize (position0 - mypos);
   	} else {
   		direction0 = normalize (vec3(light0posn.x, light0posn.y, light0posn.z ) );
   	}

   	vec3 half0 = normalize (direction0 + eyedirn) ;
    gl_FragColor = texel * ( uAmbientLightColor + ComputeLight(direction0, light0color, normal, half0, vec4( 1.0, 1.0, 1.0, 1.0), vec4( 1.0, 1.0, 1.0, 1.0 ), 0.1) );
//    gl_FragColor = texel * ( uAmbientLightColor + ( intensity * uDiffuseLightColor ) );
}
