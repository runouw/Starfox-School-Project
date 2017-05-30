# include <gltools\gltools.h>

GLTools::GLTexture::GLTexture(GLuint texId){
	this->texId = texId;
}
GLTools::GLTexture::~GLTexture(){
	//clear();
}
bool GLTools::GLTexture::ready(){
	return (this->texId > 0);
}
void GLTools::GLTexture::clear(){
	if (this->texId > 0){
		glDeleteTextures(1, &(this->texId));
	}	
}
void GLTools::GLTexture::bind(GLenum target){
	//static GLint BOUND_TEXTURES[3];

	// TODO: optimize bind by doing check.
	// REMOVED for now because of errors (no unbind function exists!!)
	// Deleting a framebuffer deletes the textures and the BOUND_TEXTURES flag is not turned off!!
	// reduce number of binds
	//if (BOUND_TEXTURES[target - GL_TEXTURE0] != this->texId){
		glActiveTexture(target);		
		glBindTexture(GL_TEXTURE_2D, this->texId);
		//BOUND_TEXTURES[target - GL_TEXTURE0] = this->texId;
	//}	
}
void GLTools::GLTexture::load(GsImage& img, bool buildMipMaps){
	glGenTextures(1, &this->texId);

	glBindTexture(GL_TEXTURE_2D, this->texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w(), img.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
	if (buildMipMaps){
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (buildMipMaps){
		glGenerateMipmap(GL_TEXTURE_2D);

		// TODO: IF ANISOTRPIC FILTERING
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}