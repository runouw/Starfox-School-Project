# include <gltools/gltools.h>
void GLTools::GLFramebuffer::init(int32_t w, int32_t h, bool createDepthBuffer){
	this->clear();

	w = (w == -1) ? this->w : w;
	h = (h == -1) ? this->h : h;

	createDepthBuffer |= this->hasDBuff;

	this->w = w;
	this->h = h;
	this->hasDBuff = createDepthBuffer;

	glGenFramebuffers(1, &frameBufId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufId);

	// construct the texture	
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
	
	// construct the depth buffer
	if (createDepthBuffer){
		GLuint depthrenderbuffer, depthBufferId;		
		glGenRenderbuffers(1, &depthrenderbuffer);
		glGenTextures(1, &depthBufferId);	this->depthBuffer = new GLTexture(depthBufferId);

		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		glBindTexture(GL_TEXTURE_2D, depthBufferId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferId, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		printf("warning: incomplete texture!\n");
		printf("width: %d height: %d\n", w, h);
	}
	else{		
		this->initialized = true;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
bool GLTools::GLFramebuffer::hasDepthBuffer(){
	return this->hasDBuff;
}
GLTools::GLFramebuffer::GLFramebuffer(int32_t w, int32_t h, bool createDepthBuffer){
	this->hasDBuff = createDepthBuffer;
	this->w = w;
	this->h = h;
	this->initialized = false;
	this->depthBuffer = NULL;
}
GLTools::GLFramebuffer::~GLFramebuffer(){
	this->clear();
}
void GLTools::GLFramebuffer::bindDepthBuffer(GLenum target){
	if (this->depthBuffer == NULL)	return;

	this->depthBuffer->bind(target);
}
void GLTools::GLFramebuffer::bindFramebuffer(){
	if (this->initialized == false){
		this->init();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufId);
}
void GLTools::GLFramebuffer::unbindFramebuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GLTools::GLFramebuffer::resize(int32_t w, int32_t h){
	this->w = w;
	this->h = h;

	if (this->frameBufId > 0){
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufId);

		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		if (this->depthBuffer != NULL){
			glBindTexture(GL_TEXTURE_2D, depthBuffer->texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GLTools::GLFramebuffer::clear(){
	GLTexture::clear();

	if (this->depthBuffer != NULL){
		delete this->depthBuffer;
		this->depthBuffer = NULL;
	}

	if (this->frameBufId > 0){
		glDeleteFramebuffers(1, &frameBufId);
		this->frameBufId = 0;
	}
}
GLTools::GLTexture* GLTools::GLFramebuffer::getDepthBuffer(){
	return this->depthBuffer;
}