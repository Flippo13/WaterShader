//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap; 
uniform sampler2D normalMap;
uniform sampler2D depthTexture; 
uniform int numLights; 

struct DirectLight {
	vec3 direction; 

	vec3 specular;
};
uniform DirectLight directLight; 

struct PointLight {
	vec3 position; 

	float constant;
	float linear; 
	float quadratic; 

	vec3 specular; 
};
#define MAX_POINT_LIGHTS 1862 
uniform PointLight pointLights[MAX_POINT_LIGHTS]; 

uniform vec3 lightColor; 

uniform vec3 fromLightDirection; 
varying vec3 position;
varying vec3 worldNormal;
varying vec3 eyeNormal;
uniform float waveFactor; 
in vec2 texCoord;
in vec4 clipSpace;
in vec3 toCameraVector;
in vec3 fromLightVector; 
in vec4 worldPos; 
in vec3 eyePos; 

out vec4 fragment_color;

const float waveStrength = 0.03f;
const float shineDamper = 32.0f; 
const float reflectivity = 0.4f;

vec3 CalcDirectLight (DirectLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction); 

	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineDamper);

	vec3 specular = light.specular * spec;

	return(specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shineDamper);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + 
  			     light.quadratic * (dist * dist));

	vec3 specular = light.specular * spec; 

	specular *= attenuation; 

	return (specular);
	
}

void main( void ) {
    vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, 1.0-ndc.y);
	
	float near = 0.1; 
	float far = 1000.0; 
	float depth = texture(depthTexture, refractTexCoords).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.y; 
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = floorDistance - waterDistance; 

	vec2 distortedTexCoord = texture(dudvMap, vec2(texCoord.x + waveFactor, texCoord.y)).rg*0.1;
	distortedTexCoord = texCoord + vec2(distortedTexCoord.x, -distortedTexCoord.y + waveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoord).rg * 2.0 - 1.0) * waveStrength;// * clamp(waterDepth / 50.0,0.0,1.0);

    refractTexCoords += totalDistortion;
	reflectTexCoords += totalDistortion;

    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

    //vec3 eye = normalize(eyePos - position);
    //vec3 r = reflect(eye, worldNormal);
	vec4 normalMapColor = texture(normalMap, distortedTexCoord);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.g * 3.0, normalMapColor.b * 2.0 - 1.0);
	normal = normalize(normal);

	//vec3 viewDir = normalize(eyePos - worldPos.xyz);

	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, 0.5);

	vec3 lightResult = CalcDirectLight(directLight, normal, viewVector); 

	for(int i = 0; i < numLights && i < MAX_POINT_LIGHTS; i++) {
		//lightResult += CalcPointLight(pointLights[i], normal, worldPos.xyz, viewVector);
	}


	//vec3 reflectedLight = reflect(normalize(fromLightVector), normal); 
	//float specular = max(dot(reflectedLight, viewVector), 0.0);
	//specular = pow(specular, shineDamper); 
	vec3 specularHighlights = lightResult * reflectivity;//  * specular * reflectivity;// * clamp(waterDepth / 50.0,0.0,1.0); 

	//fragment_color = mix(reflectColor, refractColor,refractiveFactor);
	fragment_color = vec4(refractColor);
	//fragment_color = mix(fragment_color, vec4(0.0,0.2,0.5, 1.0),0.2) + vec4(specularHighlights, 0.0);

	//fragment_color.a = clamp(waterDepth / 5.0,0.0,1.0); 
	//fragment_color = vec4(waterDepth/50.0);
}
