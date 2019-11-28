//
//  Created by Ayush Kumar on 6/26/18.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#version 330 core
in vec3 vnormal;
in vec3 vpos;
in vec3 vlightpos;
in vec2 vuv;
flat in uint vmtl_idx;

uniform sampler2D diffuse_textures[16];
//uniform sampler2D specular_textures[16];

out vec4 color;

void main() {
    //we're working in the view space so the camera is at origin
    vec3 vcamerapos = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(vnormal);
    vec3 cameradir = normalize(vcamerapos - vpos);
    vec3 lightdir = normalize(vlightpos - vpos);
    vec3 reflectdir = reflect(-lightdir, normal);
    vec3 h = (cameradir + lightdir) / length(cameradir + lightdir);
    float costheta = 0.4 * max(dot(lightdir, normal), 0.0);
    //blinn-phong
    float cosalpha = 0.3 * pow(max(dot(h, normal), 0.0), 4.0);
    
    vec3 tex_diff;
    switch(uint(vmtl_idx)) {
        case(0u) : tex_diff = vec3(texture(diffuse_textures[0], vuv).rgb); break;
        case(1u) : tex_diff = vec3(texture(diffuse_textures[1], vuv).rgb); break;
        case(2u) : tex_diff = vec3(texture(diffuse_textures[2], vuv).rgb); break;
        case(3u) : tex_diff = vec3(texture(diffuse_textures[3], vuv).rgb); break;
        case(4u) : tex_diff = vec3(texture(diffuse_textures[4], vuv).rgb); break;
        case(5u) : tex_diff = vec3(texture(diffuse_textures[5], vuv).rgb); break;
        case(6u) : tex_diff = vec3(texture(diffuse_textures[6], vuv).rgb); break;
        case(7u) : tex_diff = vec3(texture(diffuse_textures[7], vuv).rgb); break;
        case(8u) : tex_diff = vec3(texture(diffuse_textures[8], vuv).rgb); break;
        case(9u) : tex_diff = vec3(texture(diffuse_textures[9], vuv).rgb); break;
        case(10u) : tex_diff = vec3(texture(diffuse_textures[10], vuv).rgb); break;
        case(11u) : tex_diff = vec3(texture(diffuse_textures[11], vuv).rgb); break;
        case(12u) : tex_diff = vec3(texture(diffuse_textures[12], vuv).rgb); break;
        case(13u) : tex_diff = vec3(texture(diffuse_textures[13], vuv).rgb); break;
        case(14u) : tex_diff = vec3(texture(diffuse_textures[14], vuv).rgb); break;
        case(15u) : tex_diff = vec3(texture(diffuse_textures[15], vuv).rgb); break;
    }
    //vec3 tex_spec = vec3(texture(specular_textures[vmtl_idx], vuv).rgb);
    //vec4 amb = vec4(0.1, 0.1, 0.1, 1.0);
    //vec4 diff = vec4(costheta, costheta, costheta, 1.0);
    vec3 spec = vec3(cosalpha, cosalpha, cosalpha);

    //color = amb + diff + spec;
    vec3 amb = 0.2 * tex_diff;
    vec3 diff = 2.0 * costheta * tex_diff;
    //vec3 spec = cosalpha * tex_diff;

    color = vec4(amb + diff + spec, 1.0);
    //color = vec4(costheta, costheta, costheta, 1.0);
    return;
}