//DIFFUSE COLOR FRAGMENT SHADER

#version 400 core // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform mat4 projectionReflection;
uniform float waveFactor;

varying vec3 interpolatedVertexObject;
in vec4 clipSpace;
in vec2 texCoord;
out vec4 fragment_color;

const float waveStrenght = 0.05f;

void main( void ) {

	vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, 1.0-ndc.y);

	vec2 distortion1 = (texture(dudvMap, vec2(texCoord.x + waveFactor / 28000.0f, texCoord.y +waveFactor / 28000.0f)).rg * 2.0 - 1) * waveStrenght; 
	vec2 distortion2 = (texture(dudvMap, vec2(texCoord.x, -texCoord.y + waveFactor / 28000.0f)).rg * 2.0 - 1) * waveStrenght; 
	
	vec2 totalDistortion = distortion1 + distortion2;

	refractTexCoords += totalDistortion; 
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	reflectTexCoords += totalDistortion;
	//reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);

	//reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	//vec4 clipReflection = projectionReflection * vec4(interpolatedVertexObject.xy, 0.0, 1.0);
	//vec2 deviceReflection = clipReflection.st / clipReflection.q;
	//vec2 textureReflection = vec2(0.5, 0.5) + vec2(0.5,0.5) * clipSpace ;
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	//vec4 reflectionTextureColor = texture2D (reflectionTexture, textureReflection);

	//reflectionTextureColor.a = 1.0;

	fragment_color = mix(reflectColor,refractColor,0.5);
	fragment_color = mix(fragment_color, vec4(0.2,0.2,0.7,1.0),0.3);
}
