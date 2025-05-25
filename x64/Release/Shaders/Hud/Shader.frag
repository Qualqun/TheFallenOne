#version 120

#define PI 3.14159265

uniform sampler2D texture;
uniform float value; //   = m_health / m_healthMax * M_PI * 2 - M_PI

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	
	float anglePxl = atan(texcoord.y-0.5, texcoord.x-0.5); //origin
	float angleHp = value;
	
	if(anglePxl < -angleHp)
	{
		float grey = 0.299f * pixel.r + 0.587f * pixel.g + 0.114f * pixel.b;
		pixel.rgb = vec3(grey);
	}

	gl_FragColor =  pixel;
}