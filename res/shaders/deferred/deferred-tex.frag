# version 400

uniform sampler2D Texture0;

in vec2 uvs;
in vec3 normals;

layout (location = 0) out vec4 fAmbient; 
layout (location = 1) out vec4 fDiffuse;
layout (location = 2) out vec3 fNormals; 
layout (location = 3) out vec3 fGlow; 

void main() { 
	vec4 color = texture2D(Texture0, uvs);
	
	fAmbient = vec4(color.rgb * .2, color.a);
	fDiffuse = color;
	fNormals = normals;
	fGlow = vec3(0);
} 
