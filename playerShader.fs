#version 330 core

in vec2 texCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 fragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_height1;
    float shininess;
}; 

struct DirLight {
    vec3 direction;	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight{
	vec3 position;

	float constant;
    float linear;
    float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

#define NR_POINT_LIGHTS 1



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));

	return(diffuse+ambient);
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight,norm,viewDir);
	fragColor = vec4(result, 1.0);
}




