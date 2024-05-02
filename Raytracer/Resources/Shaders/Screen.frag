#version 460 core
out vec4 color;

uniform sampler2D accumulated;
uniform float invFrameCount;

in vec2 uvsFragment;

void main()
{
	vec3 c = texture(accumulated, uvsFragment).rgb * invFrameCount;
	c = sqrt(c);
    color = vec4(c, 1.0f);
}
