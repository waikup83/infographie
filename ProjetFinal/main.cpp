#include "main.h"


#define FACTEUR_DEPLACEMENT 0.7f

#define GRANDEUR_CARTE 200
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 600

#define TEMPS_PAR_CHANGEMENT 500

#define NOMBRE_PETIT_PALMIER 200


void InitShaders() {
	Core::Shader_Loader Programme;

	g_Programme = Programme.CreateProgram("VertexShaders.glsl", "FragmentShaders.glsl");
	g_ProgrammeSkyBox = Programme.CreateProgram("SkyboxVertex.glsl", "SkyboxFragment.glsl");
}

int main(int argc, char* argv[]) {
	//Initialisation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(LARGEUR_FENETRE, HAUTEUR_FENETRE);
	glutCreateWindow("Projet final - David Desbiens");

	glEnable(GL_DEPTH_TEST); // activer la profondeur
	glDepthFunc(GL_LEQUAL); // pour garder le skybox
	glEnable(GL_BLEND); // activer la transparence
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Comment faire la transparence de la source vers la destination
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Calcule les pixels dans les coins


	//Fonctions de rappel
	glutDisplayFunc(Affichage);
	glutCloseFunc(FermerFenetre);
	glutKeyboardFunc(GestionClavier);
	glutPassiveMotionFunc(MouvementSouris);


	glewInit();
	InitShaders();


	// Par défaut
	FreeConsole();
	glutFullScreen();
	srand(time(NULL));
	g_Position = glm::vec3(0, 1, 0);
	g_Direction = glm::vec3(0, 0, 0);
	
	glutTimerFunc(10, DessinAutomatique, 0);

	glutTimerFunc(TEMPS_PAR_CHANGEMENT, GestionTemps, 0);
	g_CouleurTemps = glm::vec3(0.7, 0.7, 0.7);
	g_empCouleurT = glGetUniformLocation(g_Programme, "g_CouleurT");
	g_empSkyboxCouleurT = glGetUniformLocation(g_ProgrammeSkyBox, "g_CouleurT");
	g_LeverSoleil = true;

	// Curseur
	glutWarpPointer(LARGEUR_FENETRE / 2, HAUTEUR_FENETRE / 2);
	glutSetCursor(GLUT_CURSOR_NONE);

	
	// Programme normal
	g_matM = glm::mat4();
	g_empM = glGetUniformLocation(g_Programme, "g_M");
	g_matV = glm::mat4();
	g_empV = glGetUniformLocation(g_Programme, "g_V");
	g_matP = glm::mat4();
	g_empP = glGetUniformLocation(g_Programme, "g_P");
	g_empTexture = glGetUniformLocation(g_Programme, "g_TextureRecu");

	// Programme Skybox
	g_empTextureSkyBox = glGetUniformLocation(g_ProgrammeSkyBox, "g_TextureRecu");
	g_empVSkyBox = glGetUniformLocation(g_ProgrammeSkyBox, "g_V");
	g_empPSkyBox = glGetUniformLocation(g_ProgrammeSkyBox, "g_P");


	// Objets
	g_empTexModele = glGetUniformLocation(g_Programme, "g_TexModele");
	g_empCorrigerCouleur = glGetUniformLocation(g_Programme, "g_CorrigerCouleur");
	g_empSeulementCouleur = glGetUniformLocation(g_Programme, "g_SeulementCouleur");
	g_Trouve = false;


	FaireMonde();


	glutMainLoop();
	return 0;
}


void Affichage() {
	glm::mat4 Matrice = glm::mat4();
	glm::vec3 Couleur;
	int i;

	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(g_empM, 1, GL_FALSE, &Matrice[0][0]);


	// ********** Dessiner SkyBox **********
	glUseProgram(g_ProgrammeSkyBox);
	glUniform3fv(g_empSkyboxCouleurT, 1, &(g_CouleurTemps.x));
	FaireSkyBoxVP();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_TextureSkyBox);
	glUniform1i(g_empTextureSkyBox, 0);
	glBindVertexArray(g_vaoSkyBox);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// ********** DESSINER FORMES **********
	glUseProgram(g_Programme);
	FaireVP();
	glUniform3fv(g_empCouleurT, 1, &(g_CouleurTemps.x));
	glUniform1i(g_empTexModele, 0); // Mettre à faux
	glUniform1i(g_empSeulementCouleur, 0); // Mettre à faux

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(g_empTexture, 0);

	// Dessiner Sol
	glBindTexture(GL_TEXTURE_2D, g_TextureSol);
	glBindVertexArray(g_vaoSol);
	glDrawArrays(GL_TRIANGLES, 0, 6);



	// ************** MAISON **************
	// Positionner la maison
	Matrice = glm::rotate(glm::radians((float)g_PositionMaison.w), glm::vec3(0, 1, 0));
	Matrice = glm::translate(Matrice, glm::vec3(g_PositionMaison));
	glUniformMatrix4fv(g_empM, 1, GL_FALSE, &Matrice[0][0]);

	// Dessiner les murs
	glBindTexture(GL_TEXTURE_2D, g_TexMurMaison);
	glBindVertexArray(g_vaoMurMaison);
	glDrawArrays(GL_TRIANGLES, 0, 30);
	
	// Dessiner le toit
	glBindTexture(GL_TEXTURE_2D, g_TexToitMaison);
	glBindVertexArray(g_vaoToitMaison);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	

	// ********** OBJETS ***********
	// Objet trouvé
	if (g_Trouve) {
		glBindTexture(GL_TEXTURE_2D, g_TexObjetTrouve);
		glBindVertexArray(g_vaoObjetTrouve);
		Matrice = glm::translate(glm::vec3(g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER -1]) 
			+ glm::vec3(0, 2, 0)); 	// Suivre le personnage
		glUniformMatrix4fv(g_empM, 1, GL_FALSE, &Matrice[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 24);
	}


	// Petits palmiers
	Couleur = glm::vec3(0.5, 0.5, 0.5); // Correction de la couleur
	glUniform3fv(g_empCorrigerCouleur, 1, &Couleur.x);
	glUniform1i(g_empTexModele, 1); // Mettre à vrai

	for (int i = 0; i < NOMBRE_PETIT_PALMIER - 1; i++) {
		Matrice = glm::scale(glm::vec3(g_ListePetitsPalmiers[i].w, g_ListePetitsPalmiers[i].w, g_ListePetitsPalmiers[i].w));
		Matrice = glm::translate(glm::vec3(g_ListePetitsPalmiers[i])) * Matrice;
		glUniformMatrix4fv(g_empM, 1, GL_FALSE, &Matrice[0][0]);
		
		g_PetitPalmier.Draw(g_Programme);
	}

	// ********* Petits palmiers à trouver (noir) **********
	Couleur = glm::vec3(0.1, 0.1, 0.1); // Correction de la couleur
	glUniform3fv(g_empCorrigerCouleur, 1, &Couleur.x);
	Matrice = glm::scale(glm::vec3(g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].w, g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].w, 
		g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].w));
	Matrice = glm::translate(glm::vec3(g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1])) * Matrice;
	glUniformMatrix4fv(g_empM, 1, GL_FALSE, &Matrice[0][0]);

	g_PetitPalmier.Draw(g_Programme);


	// ********* Divers **********
	// Serpents
	Couleur = glm::vec3(0.7, 0.7, 0.7); // Correction de la couleur
	glUniform3fv(g_empCorrigerCouleur, 1, &Couleur.x);
	Matrice = glm::scale(glm::vec3(0.2, 0.2, 0.2));
	for (i = 0; i < 10; i++) {
		g_matM = glm::rotate(glm::radians(g_ListeObjets[i].w), glm::vec3(0, 1, 0)) * Matrice;
		g_matM = glm::translate(glm::vec3(g_ListeObjets[i])) * g_matM;
		glUniformMatrix4fv(g_empM, 1, GL_FALSE, &g_matM[0][0]);
		g_Serpent.Draw(g_Programme);
	}
	// Chevreuils
	Couleur = glm::vec3(0.5, 0.5, 0.5); // Correction de la couleur
	glUniform3fv(g_empCorrigerCouleur, 1, &Couleur.x);
	Matrice = glm::scale(glm::vec3(0.1, 0.1, 0.1));
	for ( ; i < 30; i++) {
		g_matM = glm::rotate(glm::radians(g_ListeObjets[i].w), glm::vec3(0, 1, 0)) * Matrice;
		g_matM = glm::translate(glm::vec3(g_ListeObjets[i])) * g_matM;
		glUniformMatrix4fv(g_empM, 1, GL_FALSE, &g_matM[0][0]);
		g_Chevreuil.Draw(g_Programme);
	}



	glBindVertexArray(0);
	glutSwapBuffers();
}


// Faire le monde
void FaireMonde() {
	Forme::Forme_Maker Monde;
	texCore::Texture_Loader Texture;
	float Grosseur;


	// Skybox
	glGenVertexArrays(1, &g_vaoSkyBox);
	glBindVertexArray(g_vaoSkyBox);
	Monde.creeSkyBox();
	g_TextureSkyBox = Texture.CreateSkyBox();

	// Plage
	glGenVertexArrays(1, &g_vaoSol);
	glBindVertexArray(g_vaoSol);
	Monde.creeSol(GRANDEUR_CARTE * 1.7, GRANDEUR_CARTE * 1.7);
	g_TextureSol = Texture.CreateTexture2D("textures/sable.png",
		GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);


	// ********** Maison **********
	// Base
	glGenVertexArrays(1, &g_vaoMurMaison);
	glBindVertexArray(g_vaoMurMaison);
	Monde.creeMur(3, 2, 4);
	g_TexMurMaison = Texture.CreateTexture2D("textures/bambooBrun.jpg",
		GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
	// Toit
	glGenVertexArrays(1, &g_vaoToitMaison);
	glBindVertexArray(g_vaoToitMaison);
	Monde.creeToit(3.2f, 0.5f, 4.2f);
	g_TexToitMaison = Texture.CreateTexture2D("textures/feuille2.png",
		GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
	// Position
	g_PositionMaison = glm::vec4(rand() % (GRANDEUR_CARTE - 20) - GRANDEUR_CARTE / 2, 0,
		rand() % (GRANDEUR_CARTE - 20) - GRANDEUR_CARTE / 2, (float)(rand() % 360));


	// ********** Objets ***********
	// Divers
	for (int i = 0; i < 30; i++)
		g_ListeObjets.push_back(glm::vec4(rand() % GRANDEUR_CARTE - GRANDEUR_CARTE / 2, 0,
			rand() % GRANDEUR_CARTE - GRANDEUR_CARTE / 2, rand() % 360));
	g_Serpent = Model("3D/serpent/serpent.fbx"); // Serpent
	g_Chevreuil = Model("3D/chevreuil/chevreuil.fbx"); // Chevreuil


	// Petits palmiers
	for (int i = 0; i < NOMBRE_PETIT_PALMIER; i++) {
		Grosseur = rand() % 10 / 100.0 + 0.01;
		if (Grosseur < 0.2)
			Grosseur += 0.01;
		g_ListePetitsPalmiers.push_back(glm::vec4(rand() % GRANDEUR_CARTE - GRANDEUR_CARTE / 2, 0,
			rand() % GRANDEUR_CARTE - GRANDEUR_CARTE / 2, Grosseur));
	}
	g_PetitPalmier = Model("3D/Palm_01/Palm_01.obj");


	// Objet trouvé
	glGenVertexArrays(1, &g_vaoObjetTrouve);
	glBindVertexArray(g_vaoObjetTrouve);
	Monde.creerTrouve(20, 1, 20);
	g_TexObjetTrouve = Texture.CreateTexture2D("textures/trouve.png",
		GL_NONE, GL_NONE, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);


	glBindVertexArray(0);
}


void FermerFenetre() {
	glutLeaveMainLoop();
}
void GestionClavier(unsigned char Touche, int x, int y) {
	glm::vec3 Direction;

	switch (Touche) {
		case 27:
			FermerFenetre();
			break;

		// Déplacement
		case 'w':
			Direction = g_Direction;
			Direction.y = 0;
			g_Position += glm::normalize(Direction * FACTEUR_DEPLACEMENT);
			VerifierFinMonde();
			VerifierPosition();
			break;
		case 's':
			Direction = g_Direction;
			Direction.y = 0;
			g_Position -= glm::normalize(Direction * FACTEUR_DEPLACEMENT);
			VerifierFinMonde();
			VerifierPosition();
			break;
		case 'a':
			g_Position -= glm::normalize(glm::cross(g_Direction, glm::vec3(0, 1, 0))) * FACTEUR_DEPLACEMENT;
			VerifierFinMonde();
			VerifierPosition();
			break;
		case 'd':
			g_Position += glm::normalize(glm::cross(g_Direction, glm::vec3(0, 1, 0))) * FACTEUR_DEPLACEMENT;
			VerifierFinMonde();
			VerifierPosition();
			break;

	    // Mouvement en hauteur
		case 'q':
			g_Position.y++;
			break;
		case 'e':
			if (g_Position.y > 1)
				g_Position.y--;
			break;

		// Réponse
		case 'z':
			g_Trouve = true;
			break;

		// Recommencer
		case 'r':
			// Petits palmiers
			g_ListePetitsPalmiers.clear();
			for (int i = 0; i < NOMBRE_PETIT_PALMIER; i++)
				g_ListePetitsPalmiers.push_back(glm::vec4(rand() % GRANDEUR_CARTE - GRANDEUR_CARTE / 2, 0,
					rand() % GRANDEUR_CARTE - GRANDEUR_CARTE / 2, rand() % 10 / 100.0 + 0.01));
			g_Trouve = false;
			break;
	}
}

void VerifierFinMonde() {
	GLfloat Max = GRANDEUR_CARTE / 2 - 2;
	if (g_Position.z > Max)
		g_Position.z = Max;
	else if (g_Position.z < -Max)
		g_Position.z = -Max;
	if (g_Position.x > Max)
		g_Position.x = Max;
	else if (g_Position.x < -Max)
		g_Position.x = -Max;
}


void VerifierPosition() {
	if (g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].x > g_Position.x - 1 &&
		g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].x < g_Position.x + 1 &&
		g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].z > g_Position.z - 1 &&
		g_ListePetitsPalmiers[NOMBRE_PETIT_PALMIER - 1].z < g_Position.z + 1)


		//Gagner !!
		g_Trouve = true;
}


void MouvementSouris(int x, int y) {	
	glm::vec3 Devant;

	if (g_DernierX != 0)
		g_Tete += (x - g_DernierX) * 0.2f;

	if (g_DernierY != 0)
		g_Corps -= (y - g_DernierY) * 0.2f;


	// Changer la direction
	Devant.x = cos(glm::radians(g_Tete)) * cos(glm::radians(g_Corps));
	Devant.y = sin(glm::radians(g_Corps));
	Devant.z = sin(glm::radians(g_Tete)) * cos(glm::radians(g_Corps));
	g_Direction = glm::normalize(Devant);


	// Repositionner le curseur en x
	if (x != LARGEUR_FENETRE / 2 || y != HAUTEUR_FENETRE / 2) {
		glutWarpPointer(LARGEUR_FENETRE / 2, HAUTEUR_FENETRE / 2);
		g_DernierX = LARGEUR_FENETRE / 2;
		g_DernierY = HAUTEUR_FENETRE / 2;
	}
	else {
		g_DernierX = (float)x;
		g_DernierY = (float)y;
	}
}




void FaireVP() {
	float Ratio = LARGEUR_FENETRE / HAUTEUR_FENETRE;

	// Matrice de vue
	g_matV = glm::lookAt(g_Position, g_Direction + g_Position, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(g_empV, 1, GL_FALSE, &g_matV[0][0]);

	// Matrice de projection
	g_matP = glm::perspective(glm::radians(45.f), Ratio, 0.1f, (float)(GRANDEUR_CARTE + GRANDEUR_CARTE * 0.1));
	glUniformMatrix4fv(g_empP, 1, GL_FALSE, &g_matP[0][0]);
}

void FaireSkyBoxVP() {
	float Ratio = LARGEUR_FENETRE / HAUTEUR_FENETRE;

	// Matrice de vue
	g_matV = glm::mat4(glm::mat3(g_matV));
	glUniformMatrix4fv(g_empVSkyBox, 1, GL_FALSE, &g_matV[0][0]);

	// Matrice de projection
	g_matP = glm::perspective(glm::radians(45.f), Ratio, 0.1f, (float)(GRANDEUR_CARTE + GRANDEUR_CARTE * 0.1));
	glUniformMatrix4fv(g_empPSkyBox, 1, GL_FALSE, &g_matP[0][0]);
}



void DessinAutomatique(int i) {
	glutTimerFunc(10, DessinAutomatique, 0);


	glutPostRedisplay();
}


void GestionTemps(int i) {
	if (g_CouleurTemps.x > 1)
		g_LeverSoleil = false;
	else if(g_CouleurTemps.x < 0)
		g_LeverSoleil = true;



	if (g_LeverSoleil)
		g_CouleurTemps += glm::vec3(0.005, 0.005, 0.005);
	else
		g_CouleurTemps -= glm::vec3(0.005, 0.005, 0.005);


	glutTimerFunc(TEMPS_PAR_CHANGEMENT, GestionTemps, 0);
}