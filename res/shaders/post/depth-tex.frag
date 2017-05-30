# version 150

uniform sampler2D m_depth;

in vec2 uvs;

void main() { 
	gl_FragDepth = texture2D(m_depth, uvs).r;
} 
