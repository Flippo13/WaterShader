//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;
varying vec3 position;
varying vec3 worldNormal;
varying vec3 eyeNormal;
uniform vec3 eyePos;
in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
    vec3 eye = normalize(eyePos - position);
    vec3 r = reflect(eye, worldNormal);
	fragment_color = texture(diffuseTexture,texCoord);
    fragment_color.a = 0.5f;
}
