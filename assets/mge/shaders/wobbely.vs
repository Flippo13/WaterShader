//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform vec2 	offset; 
uniform float timer; 
uniform vec4 position;
in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){

	
	vec3 vert = vertex *( 0.5f * sin(vertex.y * 3 * timer) + 1.f);
	
	
    gl_Position = mvpMatrix * vec4(vert,1.0f);
	texCoord = uv;
}
