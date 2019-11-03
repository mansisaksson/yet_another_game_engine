#version 120

varying vec2 tex_coord_0;
varying vec3 normal_0;

uniform sampler2D diffuse;

void main()
{
    vec3 light_direction = vec3(1, 0, 0);
    float light_strength = clamp(dot(-light_direction, normal_0), 0.0, 1.0);
    gl_FragColor = texture2D(diffuse, tex_coord_0);// * light_strength;
}