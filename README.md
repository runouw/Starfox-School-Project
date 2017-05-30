# Starfox-School-Project
![Starfox OOP demo](https://github.com/runouw/Starfox-School-Project/blob/master/demo_pic/demo%203.jpg)

[Demo on YouTube](https://www.youtube.com/watch?v=ZhU7qlP5E5w)

This was a school project to demonstrate OOP principles in C++. I am uploading the source so others can benefit from it.

Created with Visual Studio 2013 using freeglut and glew for OpenGL. Models loaded using [Open Asset Import Library](http://assimp.sourceforge.net/). There was code provided by the school that we had to use which is located in the "gsim" directory as well parts of "app_window.h" and "app_window.cpp".

# Features

## Starfox gameplay
Gameplay similar to how Starfox 64 behaves. There's an aiming reticule that draws over the scene and can be controlled by the keyboard arrow keys.

### Controls
 * WASD - Movement
 * QE - Tilt 
 
## Collision Detection
When a laser is fired, a collision check is performed by testing ray intersection with the triangles in the scene. Enemy collision is determined by colliding the ray with a bounding sphere.

## Particles
There are particle trails are emitted for the player as well as explosion particles that are emitted when shooting down an enemy.

## Lighting
The lighting is achieved by applying a common technique called deferred lighting, in which the scene is rendered to a geometry buffer with the channels "color", "normals", and "position". After the render pass, a pass to calculate the lighting data is performed. This is done by approximating the lights as spheres (with backface rendering), and calculating the effect that each light would have on the scene and drawing it to the final backbuffer.

The player's lasers and the enemy ship's laser attacks emit light which can be seen by this effect.

# Contributors
* **Robert Hewitt** - *Programming, Design, Gameplay* - [Runouw](https://github.com/runouw)

## License

This project is licensed under the Apache 2.0 License - see the [LICENSE.md](LICENSE.md) file for details
