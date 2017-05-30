/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef OGL_TOOLS_H
# define OGL_TOOLS_H

/** \file ogl_tools.h 
 * Graphsim's wrapper and extensions for OpenGL version 4
 * For detailed OpenGL specification per version see http://www.opengl.org/registry/
 */

# include <gsim/gs.h>
# include <gsim/gs_color.h>
# include <gsim/gs_mat.h>

# ifdef GS_WINDOWS
  # include <windows.h>
  # include <GL/glew.h>
# endif

//====================== GlShader =====================

class GlShader
 { public :
    GLuint id;
   public :
    GlShader ();
   ~GlShader ();
    bool load_and_compile ( GLenum type, const char* filename );
   private :
    bool load ( GLenum type, const char* filename );
    void set ( GLenum type, const char* source );
    bool compile ();
 };

//====================== GlProgram =====================

class GlProgram
 { public :
    GLuint id;
   public :
    static GLint uniform_location ( GLuint pid, const char* varname );
   public :
    GlProgram ();
   ~GlProgram ();
    bool init_and_link ( const GlShader& sh1, const GlShader& sh2 );
    GLint uniform_location ( const char* varname ) const { return uniform_location(id,varname); }
   private :
    void attach ( GLuint shid ) { glAttachShader(id,shid); }
    void attach ( const GlShader& sh ) { glAttachShader(id,sh.id); }
    bool link ();
 };

//====================== GlLight =====================

class GlLight
 { public :
    GsVec pos;
    float ambient[4];  //!< Default is black
    float diffuse[4];  //!< Default is white
    float specular[4]; //!< Default is black
   public :
    GlLight () 
     { GsColor::black.get(ambient); 
       GsColor::white.get(diffuse); 
       GsColor::black.get(specular); 
     }
 };

//====================== GlMaterial =====================

class GlMaterial
 { public :
    float ambient[4];
    float diffuse[4];
    float specular[4];
	float shininess;
   public :
    GlMaterial ()
     { GsColor::black.get(ambient); 
       GsColor::white.get(diffuse); 
       GsColor::black.get(specular); 
     }
 };

//====================== GlObjects =====================

class GlObjects // Lightweight class for holding indices to the shader objects of a progam
 { public :
    GLuint prog;
    GLuint *va, *buf;
    GLint *uniloc;
    int na, nb, nu;
    bool changed;

   public :
    GlObjects () { prog=0; va=0; buf=0; uniloc=0; na=nb=nu=0; changed=true; }

   ~GlObjects () { delete_objects(); }

    void delete_objects ();

    void set_program ( const GlProgram& p )
     { if ( p.id>0 ) delete_objects();
       prog = p.id;
     }

    void gen_vertex_arrays ( GLsizei n )
     { delete [] va;
       va = new GLuint[n];
       glGenVertexArrays ( n, va );
       na = (gsbyte)n;
     }

    void gen_buffers ( GLsizei n )
     { delete [] buf;
       buf = new GLuint[n];
       glGenBuffers ( n, buf );
       nb = (gsbyte)n;
     }

    void uniform_locations ( int n )
     { delete [] uniloc;
       uniloc = new GLint[n];
       nu = (gsbyte)n;
     }

    void uniform_location ( int loc, const char* varname ) { uniloc[loc]=GlProgram::uniform_location(prog,varname); }
 };

//=================== utilities =====================

void glClearColor ( const GsColor& c );

//====================== info =======================

bool glChkError ( const char* msg=0, bool printmsg=true );

/*! Prints the vendor, renderer, OpenGL version and GLSL version using output out, or gsout if out is null (the default) */
void glPrintInfo ( GsOutput* out=0 );

/*! Returns the OpenGL version as a 3 digits integer, or -1 if it was not yet determined */
int glVersion ();

/*! Returns the GLSL version as a 3 digits integer, or -1 if it was not yet determined */
int glslVersion ();

//=================== End of File ======================

# endif // OGL_TOOLS_H
