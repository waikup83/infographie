#include <vector>
#include<iostream>
#include<fstream>
#include "Texture_Loader.h"

using namespace texCore;

Texture_Loader::Texture_Loader(void) {}
Texture_Loader::~Texture_Loader(void) {}


unsigned char* Texture_Loader::ReadImage(char * filename) {
	unsigned char* image = SOIL_load_image(filename, &width, &height, &canaux, SOIL_LOAD_AUTO);

	if (image == 0) {
		std::cout << "SOIL loading error: '%s'\n";
		std::cout << SOIL_last_result();
	}

	return image;
}


GLuint texCore::Texture_Loader::CreateTexture2D(char * textureImageName, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter) {
	GLuint newTexture;
	unsigned char* image;
	
	//Générer un objet texture et le lier
	glGenTextures(1, &newTexture);
	glBindTexture(GL_TEXTURE_2D, newTexture);

	// Texture wrapping				
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	// Lire l'image, associer le data à l'objet texture et créer les mipmaps
	image = ReadImage(textureImageName);
	if (canaux == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	glGenerateMipmap(GL_TEXTURE_2D);


	SOIL_free_image_data(image); //Libérer la mémoire de l'image
	glBindTexture(GL_TEXTURE_2D, 0);  //Délier l'objet texture

	return newTexture;
}


GLuint texCore::Texture_Loader::CreateSkyBox() {
	GLuint newTexture;
	unsigned char* image;
	std::vector<char*> Faces;
	
	// Images pour le skybox en ordre
	Faces.push_back("skybox/majestic_rt.tga");
	Faces.push_back("skybox/majestic_lf.tga");
	Faces.push_back("skybox/majestic_up.tga");
	Faces.push_back("skybox/majestic_dn.tga");
	Faces.push_back("skybox/majestic_bk.tga");
	Faces.push_back("skybox/majestic_ft.tga");

	//Générer un objet texture et le lier
	glGenTextures(1, &newTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, newTexture);

	// Texture wrapping				
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// texture filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Lire les images, associer le data à l'objet texture
	for (GLuint i = 0; i < Faces.size(); i++) {
		image = ReadImage(Faces[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);  //Délier l'objet texture

	return newTexture;
}




