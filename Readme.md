# ScenicSnooker                                         
                                        _                              _             
                     ___  ___ ___ _ __ (_) ___   ___ _ __   ___   ___ | | _____ _ __ 
                    / __|/ __/ _ \ '_ \| |/ __| / __| '_ \ / _ \ / _ \| |/ / _ \ '__|
                    \__ \ (_|  __/ | | | | (__  \__ \ | | | (_) | (_) |   <  __/ |   
                    |___/\___\___|_| |_|_|\___| |___/_| |_|\___/ \___/|_|\_\___|_|   
                                        
---------------

This is a simple Snooker game built for the course project of CS360 course at IIT Kanpur. The pooltable has been modelled and UV mapped in Blender. The source code provides the implementation for loading the object files and the textures (stb_library has been used for loading images as textures) as well as for rendering the loaded object. The files also implement the gameplay and rendering algorithm for the game. Cubemaps have been used for adding an environment to the scene. The shaders implement the Blinn-Phong model for Diffuse and Specular shading. More implementation details are mentioned in the report submitted.

![Screenshot](https://raw.githubusercontent.com/ayush194/Snooker/master/screenshot.png?raw=true "Screenshot")

Use the mouse to look around, pan to zoom in and out, and the keys A, S, W, D to move around the pooltable in a fixed trajectory. Use the F key to lock the camera before shooting.
Click and Drag to set the force of hit of the cuestick. Release the mouse press to hit the cue.

---------------

## Build Instructions:

To build the project, a working C++11 compiler is needed.
For linux one can use the preinstalled g++ compiler.
On OSX, the clang compiler comes packaged with XCode. 

## Dependencies
1. [GLFW](https://www.glfw.org/) is a window loading library for OpenGL. It initializes the window for rendering. It also provides event handlers for receiving input and displaying the output.

2. [GLEW](http://glew.sourceforge.net/) is a function loading library for OpenGL. It loads OpenGL function pointers at runtime.

3. [glm](https://glm.g-truc.net/0.9.9/index.html) is a library for performing efficient matrix operations. Matrix operations are frequently performed in OpenGL.

4. [FreeType](https://www.freetype.org/) is library for efficiently producing high-quality glyph images for vector and bitmap fornt formats.

Install these dependencies as they are required for building the project.

### For Linux
```
sudo apt-get update
sudo apt-get install libglfw3-dev libglew-dev libglm-dev
```

### For OSX
The easiest way to install these libraries is using the brew package manager. For a more custom install, build using the source files from the latest development branch.
```
brew install glew glfw glm
```

### For OSX and Linux
To install the FreeType font rendering library, visit their website and download their latest release. At the time of writing, this was [FreeType 2.10.0](https://download.savannah.gnu.org/releases/freetype/freetype-2.10.0.tar.gz). Unzip the tar, and build using make.
```
tar -xvf freetype-2.10.0.tar.gz
cd freetype-2.10.0
sudo make
sudo make install
```

After install, make sure that the include files are located in the standard include directory /usr/local/include/freetype2. If not, explicitly copy the header files into it.
```
sudo cp -r include /usr/local/include/freetype2
```

Removed the tar file as well as the untarred directory as they are no longer needed.
```
cd ..
sudo rm -rf freetype-2.10.0
sudo rm freetype-2.10.0.tar.gz
```

5. [ReactPhysics3D](https://www.reactphysics3d.com/) is a library for performing efficient physics simulation of a rigid body system with arbitrary motion and force parameters.

To install this dependency, make sure you have cmake installed. If not, install them using

### For Linux
```
sudo apt-get install cmake
```

### For OSX
```
brew install cmake
```

To install the reactphysics3d library, first clone the official github repository and then build it using cmake and make. The rest of the installation will remain the same for both Linux and OSX machines.

### For Linux and OSX
```
git clone https://github.com/DanielChappuis/reactphysics3d.git --branch master
cd reactphysics3d
sudo cmake .
sudo make
sudo make install
```

For some reason, the makefiles generated for the project do not copy the include files contained in the directories reactphysics3d/src/containers, reactphysics3d/src/memory, reactphysics3d/src/mathematics, reactphysics3d/src/body, reactphysics3d/src/collision, reactphysics3d/src/engine, reactphysics3d/src/constraint, reactphysics3d/src/utils to the standard include directory where the library's header files are installed. Hence we need to do this manually. Locate the directory where the reactphysics3d library's header files are installed (most likely /usr/local/include/reactphysics3d). Copy the remaining header files into the same directory. Also, the parent directory (/usr/local/include in this case) must be present in the compiler's standard search paths. You can check this using,
```
g++ -xc++ -E -v -
```

Copy the header files,
```
sudo cp -r src/containers /usr/local/include/reactphysics3d/
sudo cp -r src/memory /usr/local/include/reactphysics3d/
sudo cp -r src/mathematics /usr/local/include/reactphysics3d/
sudo cp -r src/body /usr/local/include/reactphysics3d/
sudo cp -r src/collision /usr/local/include/reactphysics3d/
sudo cp -r src/engine /usr/local/include/reactphysics3d/
sudo cp -r src/constraint /usr/local/include/reactphysics3d/
sudo cp -r src/utils /usr/local/include/reactphysics3d/
```

Remove the cloned directory as it is no longer needed,
```
cd ..
sudo rm -rf reactphysics3d
```

Now simply clone the Snooker repository and build using make.
```
git clone https://github.com/ayush194/Snooker
cd Snooker
make
```

# Running the game

Run the game by simply running the binary created.
```
./snooker
```

---------------

## Course Project
Built for course CS360 (Intorduction to Computer Graphics)

Ayush Kumar
Roll No 170195
B.Tech CSE, IIT Kanpur


