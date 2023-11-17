#version 460 core
out vec4 color;

uniform sampler2D Albedo;

in vec3 worldNormal;  
in vec3 worldPosition;  
in vec2 uvsFragment;

void main()
{
	const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	const vec3 lightPosition = vec3(10.0f, 50.0f, 10.0f);
	vec4 objectColor = texture(Albedo, uvsFragment);
	if (objectColor.w < 0.1f)
	{
		discard;
	}
	
    // ambient
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
	vec3 normal = normalize(worldNormal);
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diff * lightColor;
        
    color = vec4((ambient + diffuse) * objectColor.xyz, 1.0f);
}
