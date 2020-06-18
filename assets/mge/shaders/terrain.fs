//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 core // for glsl version (12 is for older versions , say opengl 2.1


//textures
uniform sampler2D splatMapTexture; 
uniform sampler2D diffuseTexture; //water 
uniform sampler2D diffuseTexture2; 
uniform sampler2D diffuseTexture3; 
uniform sampler2D diffuseTexture4; 

//for waterAnimation
uniform float time; 

in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
	vec4 splatColor = texture(splatMapTexture, texCoord); 
	vec2 waterCoord = vec2(texCoord.x, texCoord.y); 

	float sinX =  sin(time / 28000.0f);  
	float cosY =  cos(time / 28000.0f);

	mat2 rotMat = mat2(cosY, -sinX, sinX, cosY);

	vec2 waterAnimation = rotMat * waterCoord; 
	vec4 tex0 = texture2D(diffuseTexture, waterAnimation) * splatColor.y;
	vec4 tex1 = texture2D(diffuseTexture2, texCoord) * splatColor.x; 
	vec4 tex2 = texture2D(diffuseTexture3, texCoord) * splatColor.z; 
	vec4 tex3 = texture2D(diffuseTexture4, texCoord) * splatColor.w; 
	

	vec3 result = (tex0 + tex1 + tex2+ tex3).xyz; 


	fragment_color = vec4(result,1);
}
