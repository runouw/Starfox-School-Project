# version 400

uniform sampler2D m_diffuse;
uniform sampler2D m_normal;
uniform sampler2D m_depth;

in vec4 lightPos;
in vec4 lightColor;
in vec4 screenPos;

uniform mat4 vProjInv;

out vec4 fColor;

void main() {
	//fColor = vec4(1, 1, 1, .1);
	
	vec2 uvs = (screenPos.xy/screenPos.w + vec2(1.0))/2.0;
	
	vec4 color = texture2D(m_diffuse, uvs);
	
	// recronstruction of position from depth buffer:
	float depthAt = texture2D(m_depth, uvs).x;

	//depthAt = depthAt*2.0 - 1.0;
	
	vec4 screenPos = vec4(screenPos.xy/screenPos.w, depthAt*2.0 - 1.0, 1.0);
	vec4 pos = vProjInv * screenPos;
	
	pos.xyz /= pos.w;
	
	
	float radius = lightPos.w;
	
	vec3 d = pos.xyz - lightPos.xyz;
	float dist = length(d);
	//if(dist > radius) discard;

	vec3 dn = normalize(d);

	vec3 norm = texture2D(m_normal, uvs).xyz;

	float str = max(dot(-dn, norm), 0);

	float br = 1 - (dist/radius);
	br = clamp(br, 0, 1);

	br = br*br*str;
	fColor = vec4(color.rbg*br, 1)*lightColor;
} 
