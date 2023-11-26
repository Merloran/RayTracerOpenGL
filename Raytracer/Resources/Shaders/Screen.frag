#version 460 core
out vec4 color;

uniform sampler2D image;

in vec2 uvsFragment;

void main()
{ 
    color = vec4(texture(image, uvsFragment).rgb, 1.0f);
}
