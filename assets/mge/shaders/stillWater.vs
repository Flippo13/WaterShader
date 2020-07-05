#version 400 core

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;

out vec4 clipSpace; 

out vec2 texCoord;

void main( void ){
		
    vec4 worldPos = modelMatrix * vec4(vertex,1.0);
        vec4 posRelativeToCamera = viewMatrix * worldPos;
        clipSpace =  projectionMatrix * posRelativeToCamera;
    
    
    gl_Position =  clipSpace;
    texCoord = vec2 (vertex.x / 2.0 + 0.5, vertex.z/ 2.0 + 0.5) * 6;
		
}
