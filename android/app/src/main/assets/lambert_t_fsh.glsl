#version 300 es
#include lambert_functions_fsh

uniform highp vec3 camera_position;
uniform lowp vec4 material_diffuse_surface_color;
uniform lowp float material_diffuse_intensity;
uniform lowp float material_alpha;

uniform sampler2D diffuse_texture;

in lowp vec3 v_normal;
in highp vec2 v_texcoord;
in highp vec3 v_surface_position;

out lowp vec4 frag_color;

void main() {
    VROLambertLighting lambert;
    lambert.normal = v_normal;
    lambert.texcoord = v_texcoord;
    lambert.surface_position = v_surface_position;
    lambert.camera_position = camera_position;

    lambert.ambient_color = ambient_light_color.xyz;
    lambert.material_color = material_diffuse_surface_color;
    lambert.diffuse_intensity = material_diffuse_intensity;
    lambert.material_alpha = material_alpha;

    lowp vec4 diffuse_texture_color = texture(diffuse_texture, lambert.texcoord);
    frag_color = lambert_lighting_diffuse_texture(lambert, diffuse_texture_color);
}