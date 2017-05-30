# version 400

uniform sampler2D Texture0;

in vec2 uvs;

uniform vec4 u_color;

out vec4 fColor;

void main() { 
	/*
	vec4 color = texture2D(Texture0, uvs);
	
	fColor = vec4(color.r * .3, color.g, color.b * .3, (color.r + color.g + color.b) / 3);
	*/
	
	
	
	vec4 color = texture2D(Texture0, uvs);
	float br = (color.r + color.g + color.b) / 3.0;
	color.a = br;
	
	fColor = color * u_color;
} 
