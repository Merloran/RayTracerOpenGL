#version 460 core
out vec4 color;

uniform sampler2D accumulated;
uniform float invFrameCount;

in vec2 uvsFragment;

void main()
{
	vec3 c = texture(accumulated, uvsFragment).rgb * invFrameCount;
	//const float gamma = 1.2f;
	//c = pow(c, vec3(1.0f / gamma));
	c = sqrt(c);
    color = vec4(c, 1.0f);
}
