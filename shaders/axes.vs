//
//  Created by Ayush Kumar on 6/26/18.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#version 330 core
layout (location = 0) in vec3 gpos;     //global pos
layout (location = 1) in vec3 gcolor; 	//global normal

out vec3 vcolor;

//uniform mat4 model;
//do not perform any model calculations in the vertex shader
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(gpos, 1.0);
    vcolor = gcolor;
}
