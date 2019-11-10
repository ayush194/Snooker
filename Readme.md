==============================================================================================

|''''\  |''\  /''|  /'''| |'''''|  /'''|     /''''/ |'|  |'|    /\    |''''\   |'''''| |''''\ 
| |_| | |   \/   | / /| | | |'| | / /| |    / /'''  | |__| |   /  \   | |'\ \  | |'''  | |_| |
|    /  | |\  /| | \/ | | | | | | \/ | |    \ ''''| |  __  |  / __ \  | |  | | | ''|   |    / 
| |\ \  | | \/ | |    | | | |_| |    | |     '''/ | | |  | | / /  \ \ | |_/ /  | |'    | |\ \ 
|_| \_\ |_|    |_|    |_| |_____|    |_|    \''' /  |_|  |_| \/    \/ |____/   | ''''| |_| \_\

==============================================================================================

This is a simple room shader built for Assignment 2 of CS360 course at IIT Kanpur.
The room has been modelled and uv mapped in Blender. The source code provides the
implementation for loading the object files and the textures (stb_library has been
used for loading images as textures) as well as for rendering the loaded object.
The shaders implement the Blinn-Phong model for Diffuse and Specular shading.

USE the mouse to look around, pan to zoom in and out, and the keys A, S, W, D to move 
left, backwards, forwards and right respectively.

-----------------------------------------------------------------------------------

Build Instructions:

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

2. If you are using OSX/Linux, simply compile using the shell file provided and then 
   run the binary created :
		$ sh compile.sh
		$ ./rm101

3. To rebuild the project, simply repeat step 2

4. If you are using Windows or some other operating system, you will need to change
   the OpenGL headers in the source file and then compile with the proper options. 
   Note that you need a C++ version 11 or later compiler to compile the source code.

-----------------------------------------------------------------------------------


Assignment 2
Built for course CS360 (Intorduction to Computer Graphics)
Name: Ayush Kumar
Roll No: 170195
Desg: B.Tech CSE, IIT Kanpur

Designed and Developed by Ayush Kumar | © Copyrights Reserved


