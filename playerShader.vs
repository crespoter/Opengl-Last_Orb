#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;


out VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 texCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.texCoords = aTexCoords;
	vs_out.Normal = aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos,1.0));
	gl_Position = projection * view * model * vec4(aPos,1.0);
}