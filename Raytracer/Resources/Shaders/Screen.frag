#version 460 core
out vec4 color;

uniform sampler2D accumulated;
uniform float invFrameCount;

in vec2 uvsFragment;

void main()
{ 
    color = vec4(texture(accumulated, uvsFragment).rgb * invFrameCount, 1.0f);
}
