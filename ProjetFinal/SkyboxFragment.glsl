#version 430

in vec3 g_TexturePos;
out vec4 g_Couleur;

uniform samplerCube g_TextureRecu;
uniform vec3 g_CouleurT;

void main() {

	g_Couleur = vec4(g_CouleurT, 1) * texture(g_TextureRecu, g_TexturePos);
}
