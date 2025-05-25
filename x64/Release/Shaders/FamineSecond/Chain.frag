#version 120

uniform sampler2D texture;
uniform float distChainY;
uniform float heightSpritesheet;
uniform int heightAnim;


void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	
	if(heightSpritesheet * texcoord.y < distChainY + heightAnim)
	{
		pixel.a = 0;
	}

	gl_FragColor = pixel;
}