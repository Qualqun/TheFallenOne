#version 120

uniform sampler2D texture;
uniform float timer;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	float dist = distance(vec2(0), texcoord);
	
	if(dist < tan(timer*1.5) && dist > tan(timer*1.5)/2)
	{
		pixel.rgb = vec3(1,1,1);
	}
	
	gl_FragColor = pixel;
}