#version 330 core
#define MAX_POINT_LIGHTS 10

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
uniform PointLight pointlight;
uniform Material material;





vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = 0.2f;// pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
	return (diffuse+ambient); //+ specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir,vec3 fragpos)
{
	vec3 lightDir = normalize(light.position - fragpos);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float diff  = max(dot(normal,lightDir),0.0);
	float distance = length(light.position - fragpos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (diffuse+ambient+specular);
}
void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight,norm,viewDir);
	result = result + CalcPointLight(pointlight,norm,viewDir,FragPos);
	fragColor = vec4(result, 1.0);
}




