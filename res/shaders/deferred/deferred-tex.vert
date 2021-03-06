# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vUVs;

uniform mat4 vTransf;
uniform mat4 vProj;

out vec2 uvs;
out vec3 normals;

void main (){
	vec4 p = vTransf * vec4 ( vPos.x, vPos.y, vPos.z, 1.0 );
	p = vProj * p;
	
	uvs = vUVs;
	
	normals = normalize(mat3(vTransf) * vNorm);

	gl_Position = p;
}
