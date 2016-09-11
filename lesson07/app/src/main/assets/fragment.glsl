precision mediump float;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform sampler2D sNormalMap;

uniform vec4 uAmbientLightColor;
uniform vec4 uDiffuseLightColor;
uniform vec4 uDiffuseLightPosition;
varying vec4 vTransformedVertex;
varying vec3 vObjectSpaceNormal;
varying vec3 vObjectSpaceTangent;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightColor, const in vec3 normal, const in vec3 halfVec, const in vec4 diffuse, const in vec4 specular, const in float shininess) {

        float nDotL = dot(normal, direction)  ;
        vec4 lambert = diffuse * lightColor * max (nDotL, 0.0) ;

        float nDotH = dot(normal, halfVec) ;
        vec4 phong = specular * lightColor * pow (max(nDotH, 0.0), shininess) ;

        return lambert + phong;
}

//http://chimera.labs.oreilly.com/books/1234000001814/ch08.html#ch08_id35940324
vec3 getNormalFromMapInObjectSpace( const in sampler2D map, const in vec2 uv, const in vec3 normal, const in vec3 tangent ) {
    highp vec3 tangentSpaceNormal = texture2D(map, uv).yzx * 2.0 - 1.0;
    highp vec3 n = normalize(normal);
    highp vec3 t = normalize(tangent);
    highp vec3 crossed = cross(n, t);
    highp vec3 b = normalize(crossed);
    highp mat3 basis = mat3(n, t, b);
    highp vec3 N = basis * tangentSpaceNormal;

    return N;
}


void main() {

    highp vec3 N;
    vec3 halfAngleVector;

    vec4 lightPosition = uDiffuseLightPosition;

    vec4 texel = texture2D( sTexture, vTextureCoords );

    vec3 dehomogenizedPosition = vTransformedVertex.xyz / vTransformedVertex.w;

    vec3 lightDirection;

    if ( lightPosition.w == 1.0 ) {
        lightDirection = normalize ((lightPosition.xyz / lightPosition.w) - dehomogenizedPosition);
   	} else {
   		lightDirection = normalize (lightPosition.xyz );
   	}

    const vec3 origin = vec3(0,0,0);
   	vec3 normalizedEyeDirection = normalize( dehomogenizedPosition - origin);
   	halfAngleVector = normalize (lightDirection + normalizedEyeDirection);

   	float shininess = 2.0;
   	vec4 diffuseHighlight = vec4( 0.5, 0.5, 0.5, 1.0 );
   	vec4 specularHighlight = vec4( 1.0, 1.0, 1.0, 1.0 );

    N = getNormalFromMapInObjectSpace( sNormalMap, vTextureCoords, vObjectSpaceNormal,  vObjectSpaceTangent );

	gl_FragColor = texel * (uAmbientLightColor + ComputeLight (lightDirection, uDiffuseLightColor, normalize( N), halfAngleVector, diffuseHighlight, specularHighlight, shininess));
}
