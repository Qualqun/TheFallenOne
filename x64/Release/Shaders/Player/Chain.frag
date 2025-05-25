#version 120

uniform sampler2D texture;
uniform float hypotenuse;
uniform float sizeSprite;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	
	
	if(sizeSprite * texcoord.x > hypotenuse)
	{
		pixel.a = 0;
	}
	
	gl_FragColor = pixel;
}