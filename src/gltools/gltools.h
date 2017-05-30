# ifndef GLTOOLS_H
# define GLTOOLS_H

# include <gsim/gs.h>
# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_array.h>
# include <gsim/gs_image.h>

# include <fstream>
# include <string>

# ifdef GS_WINDOWS
# include <windows.h>
# include <GL/glew.h>
# endif

# ifndef M_PI
# define M_PI 3.14159265358
# endif
# ifndef TO_RAD
# define TO_RAD M_PI / 180
# endif
# ifndef TO_DEG
# define TO_DEG 180 / M_PI
# endif

# include <map>

# define TEXT_DIFFUSE 0
# define TEXT_ENVIRONMENT 1
# define TEXT_NORMAL 1
# define TEXT_DISPLACEMENT 2
namespace GLTools{
	typedef union {
		float e[4];
		struct{
			float r, g, b, a;
		};
	} GLColor;
	
	class GLLight {
	public:
		GLColor ambient;
		GLColor diffuse;
		GLColor specular;
		GsVec position;
		GLLight();
	};

	class GLMaterial {
	public:
		GLColor ambient;
		GLColor diffuse;
		GLColor specular;
		float shininess;
		GLMaterial();
	};

	class GLShader {
	private:
		GLuint shader;
		GLenum type;
	public:
		GLShader();
		~GLShader();
		bool load(GLenum, const char*);
		bool compile();
		bool loadAndCompile(GLenum, const char*);
		void attach(GLuint id);
	};

	class GLProgram {
	private:
		GLint id;		
		std::map<std::string, GLint> attribs;
		std::map<std::string, GLint> uniforms;
		GLuint aCount, uCount;
	public:
		GLProgram();
		~GLProgram();
		void setUniform1i(const std::string&, const GLint);
		void setUniform1f(const std::string&, const GLfloat);
		void setUniform2f(const std::string&, const GLfloat*);
		void setUniform3f(const std::string&, const GLfloat*);
		void setUniform4f(const std::string&, const GLfloat*);
		void setUniformMat4f(const std::string&, const GLfloat*);
		void setBuffer(const std::string& loc, GLint buffer, GLuint size,
			GLenum mode = GL_FLOAT, GLboolean normalize = GL_FALSE, GLint stride = 0, GLvoid* ptr = 0);
		GLint getAttribLoc(const char*);
		GLint getUniformLoc(const char*);

		void checkUniformLoc(const std::string&);
		void getAttribLoc(const uint32_t argc, ...);
		void getUniformLoc(const uint32_t argc, ...);

		void bind();
		void unbind() const;
		bool initAndLink(GLShader&, GLShader&);
		bool initAndLink(GLShader&, GLShader&, GLShader&);
		bool initAndLink(GLShader&, GLShader&, GLShader&, GLShader&);
	};

	class GLObject {
	protected:
		GLProgram program;
		GLuint* buf;
		GLuint bCount;
		void setProgram(const GLProgram& prog);
		void genBuffers(GLuint n);
	public:
		bool changed;
		GLObject();
		~GLObject();
		virtual void init(const GLProgram& prog){}
		virtual void draw(GsMat&, GsMat&){}
	};
	class GLTexture {
	public:		
		GLuint texId;
		GLTexture(GLuint texId = 0);
		~GLTexture();
		virtual void clear();
		void load(GsImage& img, bool mipmap = true);
		void bind(GLenum target);
		bool ready();
	};
	class GLFramebuffer : public GLTexture{
	private:
		int32_t w, h;		
		GLuint frameBufId;
		GLTexture* depthBuffer;
		bool initialized;
		bool hasDBuff;		
	public:		
		GLFramebuffer(int32_t, int32_t, bool = false);
		~GLFramebuffer();
		void init(int32_t = -1, int32_t = -1, bool = true);
		void resize(int32_t, int32_t);
		bool hasDepthBuffer();
		void bindFramebuffer();
		void unbindFramebuffer();	
		void bindDepthBuffer(GLenum target);
		void clear();
		GLTexture* getDepthBuffer();
	};	

	class GLGeometrybuffer{
	private:
		
	public:
		int32_t w, h;
		int numBuffers;
		GLuint frameBufId;
		//GLuint frameBufId_depth;

		GLTexture* textures;
		GLTexture* depthTexture;
		bool initialized;
		bool hasDBuff;

		GLGeometrybuffer(int32_t w, int32_t h, int numBuffers, GLenum* internalFormats, bool = false);
		~GLGeometrybuffer();
		void resize(int32_t, int32_t);
		bool hasDepthBuffer();
		void bindFramebuffer();
		void bindFramebuffer(int numAttachments, GLenum attachments[]);
		void unbindFramebuffer();

		void bindTexture(int i, GLenum target);
		void bindDepthTexture(GLenum target);

		GLTexture* getTexture(int i);
		GLTexture* getDepthTexture();

		void clear();
	};
}
# endif