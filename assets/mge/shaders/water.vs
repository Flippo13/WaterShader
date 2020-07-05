//WATER TEXTURE SHADER
#version 330 core
const float pi = 3.14159;

uniform mat4 mvpMatrix;
//uniform sampler2D waterTexture; 

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec4 clipSpace;
out vec3 toCameraVector; 
out vec3 fromLightVector; 
out vec4 worldPos; 

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;

uniform int numWaves;
uniform vec3 eyePos;

out vec3 eyePosition; 

uniform float time;
uniform float amplitude[8];
uniform float waveLength[8];
uniform float speed[8];
uniform vec2 direction[8];
varying vec3 position;
varying vec3 worldNormal;
varying vec3 eyeNormal;
uniform vec3 lightPosition;

//int float waterHeight; 
out vec2 texCoord; //make sure the texture coord is interpolated
/*
float wave(int i, float x, float y) {
    float frequency = 2*pi/waveLength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float y) {
    float height = 0.0;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/waveLength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/waveLength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}
*/
void main( void ){

    vec4 pos = vec4(vertex,1.0);
   // pos.y = waveHeight(pos.x,pos.z);
    
        worldPos = modelMatrix * vec4(vec3(vertex.x, 0.0, vertex.z),1.0);
		vec4 posRelativeToCamera = viewMatrix * worldPos;
		clipSpace =  projectionMatrix * posRelativeToCamera;

    eyePosition = eyePos;
    position = pos.xyz / pos.w;
  //  worldNormal = waveNormal(pos.x, pos.z);
    eyeNormal = gl_NormalMatrix * worldNormal;
    gl_Position = clipSpace;
	texCoord = vec2 (vertex.x / 2.0 + 0.5, vertex.z/ 2.0 + 0.5) * 2;
    toCameraVector = eyePos - worldPos.xyz;
    fromLightVector = worldPos.xyz - lightPosition;
}