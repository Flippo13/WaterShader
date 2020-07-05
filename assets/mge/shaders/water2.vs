//DIFFUSE COLOR VERTEX SHADER

#version 400 core // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec4 clipSpace; 

out vec2 texCoord;

varying vec3 interpolatedVertexObject;

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
float textureTiling; 

uniform mat4 cameraRelfect;

void main( void ){
		
		vec4 worldPos = modelMatrix * vec4(vertex,1.0);
			vec4 posRelativeToCamera = viewMatrix * worldPos;
			clipSpace =  projectionMatrix * posRelativeToCamera;
		
		
		textureTiling = 2;
		gl_Position =  clipSpace;
		texCoord = vec2 (vertex.x / 2.0 + 0.5, vertex.z/ 2.0 + 0.5) * 6;
		
}
