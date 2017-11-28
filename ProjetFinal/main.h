#include "ShaderLoader/Shader_Loader.h"

#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtx\transform.hpp"

#include "TextureLoader\Texture_Loader.h"
#include "Forme_Maker.h"
#include <vector>
#include <random>
#include "Dependencies\Model.h"
#include <time.h>


void InitShaders();

void Affichage();
void FaireMonde();

void FermerFenetre();
void GestionClavier(unsigned char Touche, int x, int y);
void MouvementSouris(int x, int y);


void VerifierFinMonde();
void VerifierPosition();
void FaireVP();
void FaireSkyBoxVP();


void DessinAutomatique(int i);
void GestionTemps(int i);


GLuint g_Programme;


// Matrice MVP
glm::mat4 g_matM;
GLint g_empM;
glm::mat4 g_matV;
GLint g_empV;
glm::mat4 g_matP;
GLint g_empP;



// Rotation de la tête
GLfloat g_Tete = 90;
GLfloat g_DernierX = 0;
GLfloat g_Corps = 0;
GLfloat g_DernierY = 0;



// Matrice de vue
glm::vec3 g_Position;
glm::vec3 g_Direction;


// SkyBox
GLuint g_ProgrammeSkyBox;
GLuint g_empVSkyBox;
GLuint g_empPSkyBox;
GLuint g_empTextureSkyBox;
GLuint g_TextureSkyBox;
GLuint g_vaoSkyBox;


// Texture objets
GLuint g_empTexture;
GLuint g_TextureSol;
GLuint g_TexMurMaison;
GLuint g_TexToitMaison;
GLuint g_TexObjetTrouve;


// Objets
GLuint g_empTexModele;
GLuint g_empCorrigerCouleur;
GLuint g_empSeulementCouleur;
std::vector<glm::vec4> g_ListePetitsPalmiers;
glm::vec4 g_PositionMaison;		// Position + rotation
Model g_PetitPalmier;
bool g_Trouve;
std::vector<glm::vec4> g_ListeObjets;
Model g_Chevreuil;
Model g_Serpent;


// VAO
GLuint g_vaoSol;
GLuint g_vaoMurMaison;
GLuint g_vaoToitMaison;
GLuint g_vaoObjetTrouve;
GLuint g_vaoBrouillard;


// Couleur qui change selon le temps
bool g_LeverSoleil;
glm::vec3 g_CouleurTemps;
GLuint g_empCouleurT;
GLuint g_empSkyboxCouleurT;