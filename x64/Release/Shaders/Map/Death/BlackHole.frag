#version 120

uniform sampler2D texture;

uniform float time;

void main()
{
    // récupère le pixel dans la texture
	vec2 texcoord = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(texture, texcoord);
	
	vec2 mousePosNorm = vec2(0.5,0.5);//mousepos / resolution; //Mettre la position de la souris entre 0 et 1
	
	//Détermination du vecteur allant du pixel courant à la souris
	vec2 vecteur=mousePosNorm - texcoord;
	float dist = length(vecteur); //La longueur du vecteur est utile
	vecteur = normalize(vecteur); //Normalization nécessaire pour les traitements suivants
	
	if(texture2D(texture, texcoord).r < 0.5)
	{
		//Lecture d'un pixel décalé le long du vecteur par rapport au pixel courant
		float factor = 	( 0.1*cos(dist * 100 + time*5) 
						/ pow((1+dist),13));	//Atténuation de l'effet avec la distance
		pixel = texture2D(texture, texcoord + vecteur * factor);
	}
	gl_FragColor = pixel;//vec4(factor,factor,factor,1);
}


// void main()
// {
    // // récupère le pixel dans la texture
	// vec2 texcoord = gl_TexCoord[0].xy;
	// vec2 mousePosNorm = vec2(0.5,0.5);//mousepos / resolution; //Mettre la position de la souris entre 0 et 1
	
	// //Détermination du vecteur allant du pixel courant à la souris
	// vec2 vecteur=mousePosNorm - texcoord;
	// float dist = length(vecteur); //La longueur du vecteur est utile
	// vecteur = normalize(vecteur); //Normalization nécessaire pour les traitements suivants
   
	// //Lecture d'un pixel décalé le long du vecteur par rapport au pixel courant
	// float factor = 	( 0.1*cos(dist * 100 + time*5) 
					// / pow((1+dist),13));	//Atténuation de l'effet avec la distance
	// vec4 pixel = texture2D(texture, texcoord + vecteur * factor);
			
    // gl_FragColor = pixel;//vec4(factor,factor,factor,1);
// }