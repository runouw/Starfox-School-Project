# include <gltools/gltools.h>

GLTools::GLObject::GLObject(){
	this->buf = 0;
	this->changed = true;
	this->bCount = 0;
}
GLTools::GLObject::~GLObject(){
	delete[](this->buf);
}
void GLTools::GLObject::setProgram(const GLProgram& program){
	this->program = program;
}
void GLTools::GLObject::genBuffers(GLuint count){
	delete[](this->buf);

	this->buf = new GLuint[count];
	glGenBuffers(count, (this->buf));
	this->bCount = count;
	this->changed = true;
}