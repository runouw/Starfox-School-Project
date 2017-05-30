# include <gltools/gltools.h>

GLTools::GLProgram::GLProgram(){
	this->aCount = 0;
	this->uCount = 0;
	this->id = 0;
}
GLTools::GLProgram::~GLProgram(){
	if (this->id > 0){
		glDeleteProgram(this->id);
	}
}
bool GLTools::GLProgram::initAndLink(GLShader& sh0, GLShader& sh1){
	if (this->id > 0){
		glDeleteProgram(this->id);
	}
	this->id = glCreateProgram();
	sh0.attach(this->id);
	sh1.attach(this->id);
	glLinkProgram(this->id);
	GLint linked;
	glGetProgramiv(this->id, GL_LINK_STATUS, &linked);
	if (!linked){
		GLint size;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);
		std::string msg; msg.resize(size);
		glGetProgramInfoLog(id, size, NULL, &msg[0]);
		printf("%s\n", msg);
		return false;
	}
	return true;
}
bool GLTools::GLProgram::initAndLink(GLShader& sh0, GLShader& sh1, GLShader& sh2){
	if (this->id > 0){
		glDeleteProgram(this->id);
	}
	this->id = glCreateProgram();
	sh0.attach(this->id);
	sh1.attach(this->id);
	sh2.attach(this->id);

	glLinkProgram(this->id);
	GLint linked;
	glGetProgramiv(this->id, GL_LINK_STATUS, &linked);
	if (!linked){
		GLint size;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);
		std::string msg; msg.resize(size);
		glGetProgramInfoLog(id, size, NULL, &msg[0]);
		printf("%s\n", msg);
		return false;
	}
	return true;
}
bool GLTools::GLProgram::initAndLink(GLShader& sh0, GLShader& sh1, GLShader& sh2, GLShader& sh3){
	if (this->id > 0){
		glDeleteProgram(this->id);
	}
	this->id = glCreateProgram();
	sh0.attach(this->id);
	sh1.attach(this->id);
	sh2.attach(this->id);
	sh3.attach(this->id);

	glLinkProgram(this->id);
	GLint linked;
	glGetProgramiv(this->id, GL_LINK_STATUS, &linked);
	if (!linked){
		GLint size;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);
		std::string msg; msg.resize(size);
		glGetProgramInfoLog(id, size, NULL, &msg[0]);
		printf("%s\n", msg);
		return false;
	}
	return true;
}
void GLTools::GLProgram::checkUniformLoc(const std::string& loc){
	if (this->uniforms.find(loc) == this->uniforms.end()){
		getUniformLoc(loc.c_str());
	}
}

void GLTools::GLProgram::setUniform1i(const std::string& loc, const GLint v){
	checkUniformLoc(loc);
	glUniform1i(this->uniforms[loc], v);	
}
void GLTools::GLProgram::setUniform1f(const std::string& loc, const GLfloat v){
	checkUniformLoc(loc);
	glUniform1f(this->uniforms[loc], v);
}
void GLTools::GLProgram::setUniform2f(const std::string& loc, const GLfloat* v){
	checkUniformLoc(loc);
	glUniform2fv(this->uniforms[loc], 1, v);
}
void GLTools::GLProgram::setUniform3f(const std::string& loc, const GLfloat* v){
	checkUniformLoc(loc);
	glUniform3fv(this->uniforms[loc], 1, v);
}
void GLTools::GLProgram::setUniform4f(const std::string& loc, const GLfloat* v){
	checkUniformLoc(loc);
	glUniform4fv(this->uniforms[loc], 1, v);
}
void GLTools::GLProgram::setUniformMat4f(const std::string& loc, const GLfloat* v){
	checkUniformLoc(loc);
	glUniformMatrix4fv(this->uniforms[loc], 1, false, v);
}
void GLTools::GLProgram::setBuffer(const std::string& loc, GLint buffer, GLuint size, GLenum type, GLboolean normalize, GLint stride, GLvoid* ptr){
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(this->attribs[loc], size, type, normalize, stride, ptr);
}
GLint GLTools::GLProgram::getAttribLoc(const char* a){
	GLint attrib = glGetAttribLocation(this->id, a);

	this->attribs[a] = attrib;
	this->aCount++;

	return attrib;
}
GLint GLTools::GLProgram::getUniformLoc(const char* u){
	GLint uniform = glGetUniformLocation(this->id, u);	

	this->uniforms[u] = uniform;
	this->uCount++;

	return uniform;
}
void GLTools::GLProgram::getUniformLoc(const uint32_t argc, ...){
	va_list ap;
	va_start(ap, argc);

	for (uint32_t i = 0; i < argc; i++){
		getUniformLoc(va_arg(ap, char*));
	}
	va_end(ap);
}
void GLTools::GLProgram::getAttribLoc(const uint32_t argc, ...){
	va_list ap;
	va_start(ap, argc);

	for (uint32_t i = 0; i < argc; i++){
		getAttribLoc(va_arg(ap, char*));
	}
	va_end(ap);
}
void GLTools::GLProgram::bind(){
	static GLProgram* LAST_PROGRAM;

	if (LAST_PROGRAM == this){
		return;
	}
	if (LAST_PROGRAM != NULL){
		((GLProgram*)LAST_PROGRAM)->unbind();
	}

	glUseProgram(this->id);

	for (GLuint i = 0; i < this->aCount; i++){
		glEnableVertexAttribArray(i);
	}

	LAST_PROGRAM = this;
}
void GLTools::GLProgram::unbind() const{
	for (GLuint i = 0; i < this->aCount; i++){
		glDisableVertexAttribArray(i);
	}
}