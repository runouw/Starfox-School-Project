
# include <iostream>
# include "app_window.h"
#include <rtools\keys.h>
#include <rtools\ai\shaders.h>

#include <game\engine.h>
#include <game\engine_debugcam.h>
#include <game\engine_starfox.h>
#include <game\particle\particlebank.h>

//==========================================================================
// Main routine
//==========================================================================
int main(int argc, char** argv ) {
	printf("%s\n", argv[0]);

	// Init freeglut library:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextProfile(GLUT_CORE_PROFILE );

	new Keys();
	new Textures();
	new Shaders();

	ParticleBank::allocate();

	// Now create the window of your application:
	AppWindow* w = new AppWindow("OOP Final Project", 100, 100, 800, 600);

	w->setTargetFramerate(60);

	w->setActivity(new Engine_Starfox());

	// Loops forever
	w->run();
}

