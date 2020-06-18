//DIFFUSE TEXTURE VERTEX SHADER
#version 330 core // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform sampler2D heightMapTexture; 


in vec3 vertex;
in vec3 normal;
in vec2 uv;


out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ) {

    float heightMapSample = texture(heightMapTexture, uv).r * 6.0f; 
    vec3 newVertex = vec3(vertex.x, heightMapSample, vertex.z); 

    gl_Position = mvpMatrix * vec4(newVertex, 1.0f);
	texCoord = uv;
}
