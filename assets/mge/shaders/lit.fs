//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

struct Light{
    int lightType; 

    vec3 lightColor;
    vec3 lightPosition; 
    vec3 lightDirection; 

    vec3 ambient; 
    vec3 diffuse; 

    float ambientContribution; 

    float constent; 
    float linear; 
    float quadratic; 
};

struct Material {
    vec3 diffuseColor; 
    vec3 ambientColor; 
};

in vec3 Normal; 
in vec3 Vertex; 
uniform Light light; 
uniform Material material; 

out vec4 fragment_color;

void main( void ) {

    //ambient lighting
    vec3 ambient = light.ambient * material.ambientColor; 
    
    vec3 lightDir; 
    if (light.lightType == 1) {
        lightDir = normalize(light.lightPosition - Vertex);
    }
    else if (light.lightType == 0) {
        lightDir = normalize(light.lightDirection);
    }
          
    //diffuse lighting  
    vec3 norm = normalize(Normal);
    float diff = max(0.0f,dot(norm, lightDir));
    vec3 diffuse = light.diffuse * (diff  * material.diffuseColor);

    float distance = length( Vertex - light.lightPosition );

    float attenuation = 1.0f/(light.constent + light.linear * distance + light.quadratic * (distance * distance));
   //ambient *= attenuation; 
    diffuse *= attenuation;  
    vec3 result = vec3(0,0,0);
    result = result + (ambient + diffuse); 
	fragment_color = vec4 (result,1.0f);
}
