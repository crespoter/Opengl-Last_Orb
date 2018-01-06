#version 330 core
layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;
in VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 texCoords;
}gs_in[];
	
out VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 texCoords;
}vs_out;


vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a,b));
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = gl_in[0].gl_Position;
	vs_out.texCoords = gs_in[0].texCoords;
	vs_out.FragPos = gs_in[0].FragPos;
	vs_out.Normal = gs_in[0].Normal;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	vs_out.texCoords = gs_in[1].texCoords;
	vs_out.FragPos = gs_in[1].FragPos;
	vs_out.Normal = gs_in[1].Normal;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	vs_out.texCoords = gs_in[2].texCoords;
	vs_out.FragPos = gs_in[2].FragPos;
	vs_out.Normal = gs_in[2].Normal;
	EmitVertex();
	EndPrimitive();
}