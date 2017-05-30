# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

layout (location = 2) in vec4 v_lightPos;
layout (location = 3) in vec4 v_lightColor;

uniform mat4 vProj;

out vec4 screenPos;

out vec4 lightPos;
out vec4 lightColor;

void main (){
	lightPos = v_lightPos;
	lightColor = v_lightColor;

	vec4 p = vec4 ( vPos.xyz * lightPos.w + lightPos.xyz, 1.);
	
	gl_Position = vProj * p;

	screenPos = gl_Position;
}
