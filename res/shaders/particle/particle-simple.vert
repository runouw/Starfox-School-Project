# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUV;

layout (location = 2) in vec4 v_quadPos;
layout (location = 3) in vec4 v_quadColor;

uniform mat4 vCam;
uniform mat4 vProj;

out vec3 quadColor;
out vec2 uvs;

void main (){
	vec4 quadPos = v_quadPos;
	
	quadColor = v_quadColor.rgb;
	
	vec4 translatedPos = vec4(quadPos.xyz, 1.);
	
	translatedPos = vCam * translatedPos;
	
	float rotation = v_quadColor.w;
	mat3 rotate = mat3(
		cos(rotation), -sin(rotation), 0, 
		sin(rotation), cos(rotation), 0,
		0, 0, 1
	);

	
	vec4 p = vec4((vPos.xyz * rotate) * quadPos.w + translatedPos.xyz, 1.);
	
	uvs = vUV;
	
	gl_Position = vProj * p;
}
