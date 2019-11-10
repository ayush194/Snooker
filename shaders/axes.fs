//
//  Created by Ayush Kumar on 6/26/18.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#version 330 core
in vec3 vcolor;

out vec4 color;

void main() {
    color = vec4(vcolor, 1.0);
    return;
}