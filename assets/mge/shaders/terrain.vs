//DIFFUSE TEXTURE VERTEX SHADER
#version 330 core // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform mat4 modelMatrix;

uniform sampler2D heightMapTexture; 
uniform vec4 plane;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ) {
    
    float heightMapSample = texture(heightMapTexture, uv).r * 8.0f; 
    vec3 newVertex = vec3(vertex.x, heightMapSample, vertex.z); 
    
    vec4 worldPosition = modelMatrix * vec4(newVertex, 1.0f);
    gl_ClipDistance[0] = dot(worldPosition,plane);

    
    gl_Position = mvpMatrix * vec4(newVertex, 1.0f);
	texCoord = uv;
}
