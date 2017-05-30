# include <gltools/gltools.h>

GLTools::GLShader::GLShader(){
	this->shader = 0;
	this->type = 0;

}
GLTools::GLShader::~GLShader(){
	if (this->shader > 0){
		glDeleteShader(this->shader);
	}
}
void GLTools::GLShader::attach(GLuint id){
	glAttachShader(id, this->shader);
}
bool GLTools::GLShader::loadAndCompile(GLenum type, const char* filename){
	bool r;

	r = this->load(type, filename);
	r &= this->compile();

	return r;
}
bool GLTools::GLShader::load(GLenum type, const char* filename){
	// from OGLTools.h
	
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in.is_open()) {
		//std::cout << "Could not load shader [" << filename << "] !\n"; 
		printf("Could not load shader [%s]!\n", filename);
		return false;
	}

	std::string s;
	in.seekg(0, std::ios::end);
	s.resize((size_t)in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&s[0], s.size());

	GLchar* script = &s[0];

	if (this->shader > 0){
		glDeleteShader(this->shader);
	}
	this->type = type;
	this->shader = glCreateShader(type);
	glShaderSource(this->shader, 1, (const GLchar**)&script, NULL);
	return true;
}
bool GLTools::GLShader::compile(){
	GLint compiled;
	if (!this->shader) {
		return true;
	}

	glCompileShader(this->shader);
	glGetShaderiv(this->shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled){
		switch (this->type){
		case GL_FRAGMENT_SHADER:	printf("Error in fragment shader!: ");	break;
		case GL_VERTEX_SHADER:		printf("Error in vertex shader!: ");	break;
		}

		printf("compilation error\n");

		GLint size;
		glGetShaderiv(this->shader, GL_INFO_LOG_LENGTH, &size);
		std::string msg; msg.resize(size);
		glGetShaderInfoLog(this->shader, size, NULL, &msg[0]);
		std::cout << msg << std::endl;
		return false;
	}
	return true;
}