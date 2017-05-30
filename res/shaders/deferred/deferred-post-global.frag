# version 400

uniform vec4 c_ambient;
uniform vec4 c_diffuse;
uniform vec3 dir;

uniform sampler2D m_ambient;
uniform sampler2D m_diffuse;
uniform sampler2D m_normals;

in vec2 uvs;

out vec4 fColor;

void main() { 
	vec4 color = texture2D(m_ambient, uvs) * c_ambient;
	
	vec4 diffuse = texture2D(m_diffuse, uvs) * c_diffuse;
	
	vec3 normals = texture2D(m_normals, uvs).xyz;
	float str = dot(normals, dir);
	str = max(str, 0);
	
	color += diffuse * str;
	
	fColor = color;
} 
