# include <gltools/gltools.h>


GLTools::GLGeometrybuffer::GLGeometrybuffer(int32_t w, int32_t h, int numBuffers, GLenum* internalFormats, bool createDepthBuffer){
	this->hasDBuff = createDepthBuffer;
	this->w = w;
	this->h = h;

	this->numBuffers = numBuffers;

	GLuint* texIds = new GLuint[numBuffers];
	textures = new GLTexture[numBuffers];

	glGenFramebuffers(1, &frameBufId);
	glGenTextures(numBuffers, texIds);

	//glEnable(GL_MULTISAMPLE);

	printf("Start loading geometry buffer!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufId);

	for (int i = 0; i < numBuffers; i++){
		textures[i] = GLTexture(texIds[i]);
		glBindTexture(GL_TEXTURE_2D, texIds[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormats[i], w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // GL_RGB32F
		//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA8, w, h, false);

		int status = glGetError();
		if (status != GL_NO_ERROR){
			printf("Warning: error! (%d): %s\n", status, gluErrorString(status));
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texIds[i], 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, texIds[i], 0);


	}
	    
	if (hasDBuff) {
		GLuint texId_depth;
		glGenTextures(1, &texId_depth);
		depthTexture = new GLTexture(texId_depth);

		//glGenRenderbuffers(1, &frameBufId_depth);
		// If you want to sample from the depth buffer, don't make a buffer buffer for it (render buffers are not meant to be read)
		//glBindRenderbuffer(GL_RENDERBUFFER, frameBufId_depth);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBufId_depth);

		glBindTexture(GL_TEXTURE_2D, texId_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		
		int status = glGetError();
		if (status != GL_NO_ERROR){
			printf("Warning: error! (%d): %s\n", status, gluErrorString(status));
		}

		// 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		// GL_DRAW_FRAMEBUFFER
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId_depth, 0);
	}
	
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Warning: error! (%d): %s\n", status, gluErrorString(status));
	}

	fflush(stdout);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
bool GLTools::GLGeometrybuffer::hasDepthBuffer(){
	return this->hasDBuff;
}
GLTools::GLGeometrybuffer::~GLGeometrybuffer(){
	//this->clear();
}
void GLTools::GLGeometrybuffer::bindTexture(int i, GLenum target){
	if (this->textures == NULL)	return;

	this->textures[target].bind(target);
}
void GLTools::GLGeometrybuffer::bindDepthTexture(GLenum target){
	if (this->depthTexture == NULL)	return;

	this->depthTexture->bind(target);
}
void GLTools::GLGeometrybuffer::resize(int32_t w, int32_t h){
	if (this->w == w && this->h == h) return;
	this->w = w;
	this->h = h;

	printf("GLGeometrybuffer resize: (%d, %d)\n", w, h);
	
	//glBindRenderbuffer(GL_RENDERBUFFER, frameBufId);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, w, h);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, frameBufId);

	for (int i = 0; i < numBuffers; i++){
		glBindTexture(GL_TEXTURE_2D, textures[i].texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}

	if (hasDBuff){
		//glBindRenderbuffer(GL_RENDERBUFFER, frameBufId_depth);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

		glBindTexture(GL_TEXTURE_2D, depthTexture->texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	/*
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufId);
	for (int i = 0; i < numBuffers; i++){
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i].texId, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/
}
void GLTools::GLGeometrybuffer::bindFramebuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufId);

	//glViewport(0, 0, w, h);

	GLenum* drawBuffers = new GLenum[numBuffers];
	for (int i = 0; i < numBuffers; i++){
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	this->bindFramebuffer(numBuffers, drawBuffers);
	delete drawBuffers;
}
void GLTools::GLGeometrybuffer::bindFramebuffer(int numAttachments, GLenum attachments[]){
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufId);

	glViewport(0, 0, w, h);

	glDrawBuffers(numAttachments, attachments);
}

void GLTools::GLGeometrybuffer::unbindFramebuffer(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLTools::GLTexture* GLTools::GLGeometrybuffer::getTexture(int i){
	return &(textures[i]);
}

GLTools::GLTexture* GLTools::GLGeometrybuffer::getDepthTexture(){
	return depthTexture;
}

void GLTools::GLGeometrybuffer::clear(){
	if (this->depthTexture != NULL){
		delete this->depthTexture;
		this->depthTexture = NULL;
	}

	if (textures != NULL){
		for (int i = 0; i<numBuffers; i++){
			GLTexture tex = textures[i];
			tex.clear();
		}
	}

	if (this->frameBufId > 0){
		glDeleteFramebuffers(1, &frameBufId);
		this->frameBufId = 0;
	}
}