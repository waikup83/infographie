//Classe de création des morceaux d'une maison

#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "TextureLoader\Texture_Loader.h"
#include <iostream>
#include<vector>


namespace Forme
{
	class Forme_Maker
	{
	public:
		Forme_Maker::Forme_Maker(void);
		~Forme_Maker(void);
				
		void creeSkyBox();
		void creeSol(GLfloat Largeur, GLfloat Profondeur);

		void creeMur(GLfloat largeur, GLfloat hauteur, GLfloat profondeur);
		void creeToit(GLfloat largeur, GLfloat hauteur, GLfloat profondeur);
		void creerTrouve(GLfloat largeur, GLfloat hauteur, GLfloat profondeur);
	private:
		float m_HauteurBase;

		void AjouterIndice(int nTriangle, int nTexTx, int nTexTy, int nCarre, int nTexCx, int nTexCy);
	};
}


