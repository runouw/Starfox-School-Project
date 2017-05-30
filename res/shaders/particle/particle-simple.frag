# version 400

uniform sampler2D Texture0;

in vec2 uvs;

in vec3 quadColor;

out vec4 fColor;

void main() { 
	vec4 color = texture2D(Texture0, uvs);
	color.rgb *= quadColor;
	
	fColor = color;
} 
