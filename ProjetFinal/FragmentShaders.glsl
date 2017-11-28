#version 430

in vec2 g_TexturePos;
out vec4 g_Couleur;

uniform bool g_TexModele;
uniform sampler2D texture_diffuse1;	// Pour les modèle
uniform vec3 g_CorrigerCouleur;

uniform sampler2D g_TextureRecu;	// Ma texture
uniform vec3 g_CouleurT;


void main() {
	vec4 Couleur;

	// Brouillard
	float Distance = gl_FragCoord.z / gl_FragCoord.w;
	float FacteurBrouillard = 0;
	vec3 Brouillard;


	// Couleur
	if (g_TexModele)
		Couleur = vec4(g_CorrigerCouleur, 1) * texture(texture_diffuse1, g_TexturePos);
	else
		Couleur = texture(g_TextureRecu, g_TexturePos);

	// Brouillard
	FacteurBrouillard = (100.f - Distance) / (100.f - 5.f);
	FacteurBrouillard = clamp(FacteurBrouillard, 0.0, 1.0); // Facteur entre 0 et 1
	Brouillard = mix(vec3(0.8, 0.8, 0.8), Couleur.xyz, FacteurBrouillard);
	

	// Couleur finale
	g_Couleur = vec4(g_CouleurT * Brouillard, Couleur.w);
}


