#version 120

uniform sampler2D texture;

uniform vec2 posToReach;
uniform vec2 posOrigin;
uniform vec2 posCurrent;
uniform vec2 vecDir;
uniform float alpha;

void main()
{
	vec2 texcoord = gl_TexCoord[0].xy;
	float distCentre = distance(texcoord, vec2(0.5,0.5));
	
	vec4 pixel = texture2D(texture, texcoord);
	
	//Set all transparent part to stay transparent
	pixel.a = (alpha / 255) * pixel.a;
	
	float distCurrent = distance(posToReach, posCurrent);
	float distOrigin = distance(posToReach, posOrigin);
	
	float dist = distOrigin - distCurrent;
	
	float distShad = dist / distOrigin;
	
	if(texcoord.x < vecDir.x * distShad)
	{
		if(texcoord.y < vecDir.y * distShad)
		{
			texcoord.x += 0.5;
			texcoord.y += 0.5;
			
			pixel = texture2D(texture, vec2(0.5,0.5));
			pixel.a = (alpha / 255);
		}
	}
	vec2 vecDirNorm = normalize(vecDir);
	
	//Lecture d'un pixel décalé le long du vecteur par rapport au pixel courant
	float factor = 	( 0.1*cos(distShad * 100) 
					/ pow((1+distShad),20));	//Atténuation de l'effet avec la distance
	pixel = texture2D(texture, texcoord + distShad * factor);
	
	// if(distShad > 0.4)
	// {
		// pixel.a = 1;
		// gl_FragColor = vec4(1,0,0,1);
	// }

    gl_FragColor = pixel ;
}