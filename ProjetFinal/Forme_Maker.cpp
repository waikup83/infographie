#include "Forme_Maker.h"

using namespace Forme;

Forme_Maker::Forme_Maker(void) {}
Forme_Maker::~Forme_Maker(void) {}


void Forme_Maker::creeSkyBox() {
	GLuint vertexbuffer;
	GLfloat Sommets[] = {     
		-1,  1, -1, -1, -1, -1, 1, -1, -1, 1, -1, -1, 1,  1, -1, -1,  1, -1,
		-1, -1,  1, -1, -1, -1, -1,  1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1,
		1, -1, -1, 1, -1,  1, 1,  1,  1, 1,  1,  1, 1,  1, -1, 1, -1, -1,
		-1, -1, 1, -1,  1, 1, 1,  1,  1, 1,  1,  1, 1, -1,  1, -1, -1,  1,
		-1, 1, -1, 1, 1, -1, 1, 1, 1, 1, 1, 1, -1, 1, 1, -1, 1, -1,
		-1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1, -1, -1, -1, 1, 1, -1, 1 };

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sommets), Sommets, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);
}


void Forme_Maker::creeSol(GLfloat Largeur, GLfloat Profondeur) {
	GLuint vertexbuffer;
	glm::vec3 Sol[6];

	// Sommets
	Sol[0] = glm::vec3(-Largeur / 2, -0.1, -Profondeur / 2);
	Sol[1] = glm::vec3(Largeur / 2, -0.1, -Profondeur / 2);
	Sol[2] = glm::vec3(-Largeur / 2, -0.1, Profondeur / 2);
	
	Sol[3] = glm::vec3(Largeur / 2, -0.1, -Profondeur / 2);
	Sol[4] = glm::vec3(-Largeur / 2, -0.1, Profondeur / 2);
	Sol[5] = glm::vec3(Largeur / 2, -0.1, Profondeur / 2);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sol), Sol, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);

	// Envoyer les indices
	AjouterIndice(0, 0, 0, 1, 50, 50);
}


void Forme_Maker::creeMur(GLfloat largeur, GLfloat hauteur, GLfloat profondeur) {
	GLuint vertexbuffer;
	glm::vec3 Sommets[30];

	// Affectation de la variable m_hauteurBase qui permet de savoir à quelle hauteur dessiner le toit
	m_HauteurBase = hauteur;

	// Gauche
	Sommets[0] = glm::vec3(-largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[1] = glm::vec3(-largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[2] = glm::vec3(-largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[3] = glm::vec3(-largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[4] = glm::vec3(-largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[5] = glm::vec3(-largeur / 2.0, 0, profondeur / 2.0);

	//Droite
	Sommets[6] = glm::vec3(largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[7] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[8] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[9] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[10] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[11] = glm::vec3(largeur / 2.0, 0, profondeur / 2.0);

	// Arrière
	Sommets[12] = glm::vec3(largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[13] = glm::vec3(-largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[14] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[15] = glm::vec3(-largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[16] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[17] = glm::vec3(-largeur / 2.0, 0, -profondeur / 2.0);

	// Face gauche
	Sommets[18] = glm::vec3(-largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[19] = glm::vec3(-1 / 3.0, hauteur, profondeur / 2.0);
	Sommets[20] = glm::vec3(-largeur / 2.0, 0, profondeur / 2.0);
	Sommets[21] = glm::vec3(-1 / 3.0, hauteur, profondeur / 2.0);
	Sommets[22] = glm::vec3(-largeur / 2.0, 0, profondeur / 2.0);
	Sommets[23] = glm::vec3(-1 / 3.0, 0, profondeur / 2.0);

	// Face Droite
	Sommets[24] = glm::vec3(1 / 3.0, hauteur, profondeur / 2.0);
	Sommets[25] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[26] = glm::vec3(1 / 3.0, 0, profondeur / 2.0);
	Sommets[27] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[28] = glm::vec3(1 / 3.0, 0, profondeur / 2.0);
	Sommets[29] = glm::vec3(largeur / 2.0, 0, profondeur / 2.0);


	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sommets), Sommets, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);

	// Envoyer les indices
	AjouterIndice(0, 0, 0, 5, 1, 1);
}


void Forme_Maker::creeToit(GLfloat largeur, GLfloat hauteur, GLfloat profondeur) {
	GLuint vertexbuffer;
	glm::vec3 Sommets[18];


	// Arrière
	Sommets[0] = glm::vec3(0, m_HauteurBase + hauteur, profondeur / 2.0);
	Sommets[1] = glm::vec3(-largeur / 2.0, m_HauteurBase, profondeur / 2.0);
	Sommets[2] = glm::vec3(largeur / 2.0, m_HauteurBase, profondeur / 2.0);

	//Avant
	Sommets[3] = glm::vec3(0, m_HauteurBase + hauteur, -profondeur / 2.0);
	Sommets[4] = glm::vec3(-largeur / 2.0, m_HauteurBase, -profondeur / 2.0);
	Sommets[5] = glm::vec3(largeur / 2.0, m_HauteurBase, -profondeur / 2.0);

	// Gauche
	Sommets[6] = glm::vec3(0, m_HauteurBase + hauteur, -profondeur / 2.0);
	Sommets[7] = glm::vec3(0, m_HauteurBase + hauteur, profondeur / 2.0);
	Sommets[8] = glm::vec3(-largeur / 2.0, m_HauteurBase, -profondeur / 2.0);
	Sommets[9] = glm::vec3(0, m_HauteurBase + hauteur, profondeur / 2.0);
	Sommets[10] = glm::vec3(-largeur / 2.0, m_HauteurBase, -profondeur / 2.0);
	Sommets[11] = glm::vec3(-largeur / 2.0, m_HauteurBase, profondeur / 2.0);

	//Droite
	Sommets[12] = glm::vec3(0, m_HauteurBase + hauteur, -profondeur / 2.0);
	Sommets[13] = glm::vec3(0, m_HauteurBase + hauteur, profondeur / 2.0);
	Sommets[14] = glm::vec3(largeur / 2.0, m_HauteurBase, -profondeur / 2.0);
	Sommets[15] = glm::vec3(0, m_HauteurBase + hauteur, profondeur / 2.0);
	Sommets[16] = glm::vec3(largeur / 2.0, m_HauteurBase, -profondeur / 2.0);
	Sommets[17] = glm::vec3(largeur / 2.0, m_HauteurBase, profondeur / 2.0);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sommets), Sommets, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);

	// Envoyer les indices
	AjouterIndice(2, 1, 1, 2, 2, 1);

}


// 4 côtés
void Forme_Maker::creerTrouve(GLfloat largeur, GLfloat hauteur, GLfloat profondeur) {
	GLuint vertexbuffer;
	glm::vec3 Sommets[24];

	// Gauche
	Sommets[0] = glm::vec3(-largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[1] = glm::vec3(-largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[2] = glm::vec3(-largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[3] = glm::vec3(-largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[4] = glm::vec3(-largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[5] = glm::vec3(-largeur / 2.0, 0, profondeur / 2.0);

	//Droite
	Sommets[6] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[7] = glm::vec3(largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[8] = glm::vec3(largeur / 2.0, 0, profondeur / 2.0);
	Sommets[9] = glm::vec3(largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[10] = glm::vec3(largeur / 2.0, 0, profondeur / 2.0);
	Sommets[11] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);

	// Arrière
	Sommets[12] = glm::vec3(largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[13] = glm::vec3(-largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[14] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[15] = glm::vec3(-largeur / 2.0, hauteur, -profondeur / 2.0);
	Sommets[16] = glm::vec3(largeur / 2.0, 0, -profondeur / 2.0);
	Sommets[17] = glm::vec3(-largeur / 2.0, 0, -profondeur / 2.0);

	// Avant
	Sommets[18] = glm::vec3(-largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[19] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[20] = glm::vec3(-largeur / 2.0, 0, profondeur / 2.0);
	Sommets[21] = glm::vec3(largeur / 2.0, hauteur, profondeur / 2.0);
	Sommets[22] = glm::vec3(-largeur / 2.0, 0, profondeur / 2.0);
	Sommets[23] = glm::vec3(largeur / 2.0, 0, profondeur / 2.0);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sommets), Sommets, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(0);

	// Envoyer les indices
	AjouterIndice(0, 0, 0, 4, 2, 1);
}


void Forme_Maker::AjouterIndice(int nTriangle, int nTexTx, int nTexTy, int nCarre, int nTexCx, int nTexCy) {
	GLuint vertexbuffer;
	glm::vec2 *Indice = new glm::vec2[3 * nTriangle + 6 * nCarre];
	int t, c;

	// Triangle
	for (t = 0; t < nTriangle; t++) {
		Indice[t * 3] = glm::vec2(nTexTx / 2.0, 0);
		Indice[t * 3 + 1] = glm::vec2(0, nTexTy);
		Indice[t * 3 + 2] = glm::vec2(nTexTx, nTexTy);
	}

	// Carré
	t = t * 3;
	for (c = 0 ; c < nCarre; c++) {
		Indice[t + c * 6] = glm::vec2(nTexCx, 0);
		Indice[t + c * 6 + 1] = glm::vec2(0, 0);
		Indice[t + c * 6 + 2] = glm::vec2(nTexCx, nTexCy);

		Indice[t + c * 6 + 3] = glm::vec2(0, 0);
		Indice[t + c * 6 + 4] = glm::vec2(nTexCx, nTexCy);
		Indice[t + c * 6 + 5] = glm::vec2(0, nTexCy);
	}

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (3 * nTriangle + 6 * nCarre), Indice, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(2);

	delete Indice;
}
