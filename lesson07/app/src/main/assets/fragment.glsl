precision mediump float;
varying vec3 vAxisX;
varying vec2 vTextureCoords;
uniform sampler2D sTexture;
uniform sampler2D sNormalMap;
uniform vec4 uAmbientLightColor;
uniform vec4 uDiffuseLightColor;
uniform vec4 uDiffuseLightDirection;
varying vec4 vNormal;
varying vec4 vTransformedVertex;
varying vec3 vObjectSpaceNormal;
varying vec3 vObjectSpaceAxisX;
uniform mat4 uModel;

vec4 ComputeLight (const in vec3 direction, const in vec4 lightColor, const in vec3 normal, const in vec3 halfVec, const in vec4 diffuse, const in vec4 specular, const in float shininess) {

        float nDotL = dot(normal, direction)  ;
        vec4 lambert = diffuse * lightColor * max (nDotL, 0.0) ;

        float nDotH = dot(normal, halfVec) ;
        vec4 phong = specular * lightColor * pow (max(nDotH, 0.0), shininess) ;

        return lambert + phong;
}


void main() {

//http://chimera.labs.oreilly.com/books/1234000001814/ch08.html#ch08_id35940324

    highp vec3 tangentSpaceNormal = texture2D(sNormalMap, vTextureCoords).yxz * 2.0 - 1.0;
    highp vec3 n = normalize(vObjectSpaceNormal);
    highp vec3 t = normalize(vObjectSpaceAxisX);
//    highp vec3 n = normalize(vNormal.xyz);
//    highp vec3 t = normalize(vAxisX);
    highp vec3 crossed = cross(n, t);
    highp vec3 b = normalize(crossed);
    highp mat3 basis = mat3(n, t, b);
    highp vec3 N = basis * tangentSpaceNormal;

    vec4 lightPosition = vec4( -0.5, 0.0 , 0.0, 1.0 );

    vec4 texel = texture2D( sTexture, vTextureCoords );

    vec3 dehomogenizedPosition = vTransformedVertex.xyz / vTransformedVertex.w;

    vec3 normal = normalize( ( uModel * vec4( N, 0 ) ).xyz );
//    vec3 normal = normalize( N );
//    vec3 normal = normalize(vNormal.xyz);

    vec3 lightDirection;
    if ( lightPosition.w == 1.0 ) {
        lightDirection = normalize ((lightPosition.xyz / lightPosition.w) - dehomogenizedPosition);
   	} else {
   		lightDirection = normalize (lightPosition.xyz );
   	}

    const vec3 origin = vec3(0,0,0);
   	vec3 normalizedEyeDirection = normalize( dehomogenizedPosition - origin);

   	vec3 halfAngleVector = normalize (lightDirection + normalizedEyeDirection);

   	float shininess = 100.0;
   	vec4 diffuseHighlight = vec4( 0.5, 0.5, 0.5, 1.0 );
   	vec4 specularHighlight = vec4( 1.0, 1.0, 1.0, 1.0 );

    vec4 complemento = 0.0001 * ( uAmbientLightColor + ComputeLight(lightDirection, uDiffuseLightColor, normal, halfAngleVector, diffuseHighlight, specularHighlight, shininess) );
    texel = vec4( 0.5, 0.5, 0.5, 1.0 );
//    gl_FragColor = texel * ( uAmbientLightColor + ( max( dot( lightDirection, normal), 0.0 ) * uDiffuseLightColor ) ) + complemento;
    gl_FragColor = texel * ( uAmbientLightColor + ComputeLight(lightDirection, uDiffuseLightColor, normal, halfAngleVector, diffuseHighlight, specularHighlight, shininess) );
}
