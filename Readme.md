==============================================================================================

----------------------------------------SCENIC SNOOKER----------------------------------------

==============================================================================================

This is a simple Snooker game built for the course project of CS360 course at IIT Kanpur.
The pooltable has been modelled and uv mapped in Blender. The source code provides the
implementation for loading the object files and the textures (stb_library has been
used for loading images as textures) as well as for rendering the loaded object. The files
also implement the gameplay and rendering algorithm for the game. Cubemaps have been used for
adding an environment to the scene. The shaders implement the Blinn-Phong model for Diffuse 
and Specular shading. More implementation details are mentioned in the report submitted.

USE the mouse to look around, pan to zoom in and out, and the keys A, S, W, D to move 
around the pooltable in a fixed trajectory. Use the F key to fix lock the camera before shooting.
Click and Drag to set the force of hit of the cuestick. Release the mouse press to hit the cue.

-----------------------------------------------------------------------------------

Build Instructions:

#working gcc compiler

apt install g++

sudo apt-get update
sudo apt-get install libglfw3-dev
sudo apt-get install libglew-dev
sudo apt-get install libglm-dev

gcc -xc++ -E -v -


#change compiler to gcc

#Make sure you have git installed 
#make utility required

sudo apt-get install cmake
sudo apt-get install git


git clone https://github.com/DanielChappuis/reactphysics3d.git --branch master
cd reactphysics3d
sudo cmake .
sudo make
sudo make install

cp -r src/containers /usr/local/include/reactphysics3d/
cp -r src/memory /usr/local/include/reactphysics3d/
cp -r src/mathematics /usr/local/include/reactphysics3d/
cp -r src/body /usr/local/include/reactphysics3d/
cp -r src/collision /usr/local/include/reactphysics3d/
cp -r src/engine /usr/local/include/reactphysics3d/
cp -r src/constraint /usr/local/include/reactphysics3d/
cp -r src/utils /usr/local/include/reactphysics3d/

1. You need to have the libraries glm (v0.9.8), GLFW and GLEW installed to compile this program.
   Make sure that the .dylib/.a/.so files for these libraries are located in /usr/local/lib
   Also make sure that the include files for these are located in /usr/local/include

   On MacOS, you can simply install GLFW and GLEW using brew package manager
   		$ sudo brew install glm glfw3 glew
   On Linux, you can install GLFW and GLEW using apt package manager
   		$ sudo apt-get install libglfw3-dev libglew-dev

   To install glm, visit their website (links in description) and download version 0.9.8
   and copy the glm folder (containing the include files) into /usr/local/include :
         $ git clone --single-branch --branch 0.9.8 https://github.com/g-truc/glm.git
         $ cd glm
         $ sudo cp -r glm /usr/local/include

   For more information on these libraries or other build methods, visit their websites : 
   [glm](https://glm.g-truc.net/0.9.9/index.html)
   [GLFW](https://www.glfw.org/)
   [GLEW](http://glew.sourceforge.net/)

2. If you are using OSX/Linux, simply compile using the Makefile provided and then 
   run the binary created :
		$ make
		$ ./snooker

3. To rebuild the project, simply repeat step 2

4. If you are using Windows or some other operating system, you will need to change
   the OpenGL headers in the source file and then compile with the proper options. 
   Note that you need a C++ version 11 or later compiler to compile the source code.

-----------------------------------------------------------------------------------


Course Project
Built for course CS360 (Intorduction to Computer Graphics)

Member1: 
Ayush Kumar
Roll No 170195
B.Tech CSE, IIT Kanpur

Member2: 
Atul Kumar Pandey
Roll No 150164
B.Tech CSE, IIT Kanpur


