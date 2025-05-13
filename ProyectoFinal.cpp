/*---------------------------------------------------------*/
/* ----------------   Proyecto Final   --------------------*/
/*-----------------       2025-2       --------------------*/
/*------------- Alumno: Adolfo Rom n Jim nez --------------*/
/*------------- No. Cuenta: 410098363 ---------------------*/
/*------------- Alumno: Ileana Ver nica Lee Obando --------*/
/*------------- No. Cuenta: 318118408 ---------------------*/
/*------------- Alumno: Alicia Aislinn Gonz lez Nava ------*/
/*------------- No. Cuenta: 317251357 ---------------------*/
/*---------------------------------------------------------*/

#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>
#include<irrKlang.h>
#include <filesystem>
#include <vector>
#include <string>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

void loadPlaylist(const std::string& folderPath);
void playNext();
void updateAudio();
void setCameraFrontView();
void setCameraLabView();
void setCameraCiscoView();
void setCameraIsoView();
void setCameraBiciAnimation();
void setCameraAnimation();

//sonido
namespace fs = std::filesystem;
std::vector<std::string> playlist;
int currentIndex = 0;
ISoundEngine* engine = createIrrKlangDevice();
ISound* currentSound = nullptr;

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

GLuint VBO[3], VAO[3], EBO[3];

//Camera
Camera camera(glm::vec3(-10.0f, 7.0f, -100.0));
float MovementSpeed = 0.98f;
GLfloat lastX = SCR_WIDTH / 2.0f,
lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;
float contador_sol, contador_paja;
float angulo_ala = sin((contador_paja) * 30.0f);
float c_time = glfwGetTime();

void getResolution(void);
void myData(void);							// De la practica 4
void LoadTextures(void);					// De la pr ctica 6
unsigned int generateTextures(char*, bool, bool);	// De la pr ctica 6

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Texture
unsigned int	t_concrete,
t_toalla,
t_unam,
t_white,
t_grass,
t_ladrillos,
t_glass,
t_micro_b,
t_micro_f,
t_micro_l,
t_micro_r,
t_cisco_b,
t_cisco_f,
t_cisco_l,
t_cisco_r,
pared_cisco;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 90.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;

//Keyframes (Manipulaci n y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giro_bici = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotRodIzqInc = 0.0f,
giro_bici_inc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giro_bici;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//introducir n mero en caso de tener Key guardados
bool play = false;
int playIndex = 0;


bool animarCamara = false;
float velocidadRotacion = 0.5f;
float maxNegar = 7.0f;
bool girandoDerecha = true;
float anguloNegar = 7.0f;
void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giro_bici = giro_bici;

	printf("posX %f\n", posX);
	printf("posX %f\n", posY);
	printf("posX %f\n", posZ);
	printf("posX %f\n", giro_bici);
	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giro_bici = KeyFrame[0].giro_bici;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotRodIzqInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giro_bici_inc = (KeyFrame[playIndex + 1].giro_bici - KeyFrame[playIndex].giro_bici) / i_max_steps;

}

unsigned int generateTextures(const char* filename, bool alfa, bool isPrimitive)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	if (isPrimitive)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	else
		stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
	t_glass = generateTextures("Texturas/glass.png", 1, true);
	t_micro_b = generateTextures("Texturas/micro_b.jpg", 0, true);
	t_micro_f = generateTextures("Texturas/micro_f.jpg", 0, true);
	t_micro_l = generateTextures("Texturas/micro_l.jpg", 0, true);
	t_micro_r = generateTextures("Texturas/micro_r.jpg", 0, true);
	t_cisco_b = generateTextures("Texturas/cisco_b.jpg", 0, true);
	t_cisco_f = generateTextures("Texturas/cisco_f.jpg", 0, true);
	t_cisco_l = generateTextures("Texturas/cisco_l.jpg", 0, true);
	t_cisco_r = generateTextures("Texturas/cisco_r.jpg", 0, true);
	pared_cisco = generateTextures("Texturas/cisco.png", 0, true);
	t_grass = generateTextures("Texturas/grass.jpg", 0, true);
	t_concrete = generateTextures("Texturas/concrete.jpg", 0, true);
	//This must be the last
	t_white = generateTextures("Texturas/white.jpg", 0, false);

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotRodIzqInc;
			giro_bici += giro_bici_inc;

			i_curr_steps++;
		}
	}

	//Veh culo
	if (animacion)
	{
		movAuto_x += 3.0f;
	}

	//camara
	if (animarCamara) {
		if (girandoDerecha) {
			anguloNegar += velocidadRotacion;
			if (anguloNegar >= maxNegar) {
				girandoDerecha = false;
			}
		}
		else {
			anguloNegar -= velocidadRotacion;
			if (anguloNegar <= -maxNegar) {
				girandoDerecha = true;
				// Si quieres que se detenga tras una vez:
				// animarCamara = false;
			}
		}
	}

}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData() {
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float verticesPiso[] = {
		// positions          // texture coords
		 10.5f,  10.5f, 0.0f,   4.0f, 4.0f, // top right
		 10.5f, -10.5f, 0.0f,   4.0f, 0.0f, // bottom right
		-10.5f, -10.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-10.5f,  10.5f, 0.0f,   0.0f, 4.0f  // top left 
	};
	unsigned int indicesPiso[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLfloat verticesCubo[] = {
		//Position				//texture coords
		-0.5f, -0.5f, 0.5f,		0.5f, 0.66f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		0.5f, 0.33f,	//V1
		0.5f, 0.5f, 0.5f,		0.75f, 0.33f,	//V5
		-0.5f, -0.5f, 0.5f,		0.5f, 0.66f,	//V0
		-0.5f, 0.5f, 0.5f,		0.75f, 0.66f,	//V4
		0.5f, 0.5f, 0.5f,		0.75f, 0.33f,	//V5

		0.5f, -0.5f, -0.5f,		0.25f, 0.33f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	0.25f, 0.66f,	//V3
		-0.5f, 0.5f, -0.5f,		0.0f, 0.66f,	//V7
		0.5f, -0.5f, -0.5f,		0.25f, 0.33f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 0.33f,	//V6
		-0.5f, 0.5f, -0.5f,		0.0f, 0.66f,	//V7

		-0.5f, 0.5f, 0.5f,		0.5f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.25f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.25f, 0.66f,	//V3
		-0.5f, -0.5f, -0.5f,	0.25f, 0.66f,	//V3
		-0.5f, 0.5f, 0.5f,		0.5f, 1.0f,	//V4
		-0.5f, -0.5f, 0.5f,		0.5f, 0.66f,	//V0

		0.5f, 0.5f, 0.5f,		0.5f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.5f, 0.33f,	//V1
		0.5f, -0.5f, -0.5f,		0.25f, 0.33f,	//V2
		0.5f, 0.5f, 0.5f,		0.5f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		0.25f, 0.0f,	//V6
		0.5f, -0.5f, -0.5f,		0.25f, 0.33f,	//V2

		-0.5f, 0.5f, 0.5f,		0.75f, 0.66f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		0.75f, 0.33f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.33f,	//V6
		-0.5f, 0.5f, 0.5f,		0.75f, 0.66f,	//V4
		-0.5f, 0.5f, -0.5f,		1.0f, 0.66f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 0.33f,	//V6

		-0.5f, -0.5f, 0.5f,		0.5f, 0.66f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	0.25f, 0.66f,	//V3
		0.5f, -0.5f, -0.5f,		0.25f, 0.33f,	//V2
		-0.5f, -0.5f, 0.5f,		0.5f, 0.66f,	//V0
		0.5f, -0.5f, -0.5f,		0.25f, 0.33f,	//V2
		0.5f, -0.5f, 0.5f,		0.5f, 0.33f,	//V1
	};

	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para Piso
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPiso), verticesPiso, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPiso), indicesPiso, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//PARA CUBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {

	/******** irrKlang Code ************/
	loadPlaylist("media");
	playNext();

	/************************************/

	// glfw: initialize and configure
	glfwInit();

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final 2025-2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);



	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//To use with skybox
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");	//To use with animated models 

	vector<std::string> faces{
		"resources/skybox/right_6.jpg",
		"resources/skybox/left_6.jpg",
		"resources/skybox/top_6.jpg",
		"resources/skybox/bottom_6.jpg",
		"resources/skybox/front_6.jpg",
		"resources/skybox/back_6.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	//Model piso("resources/objects/piso/piso.obj");

	// -------------------------------------------------------------------------------------------------------------------------
	// Modelos Adolfo
	// -------------------------------------------------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------------------------------------------------
	// Modelos Ileana
	// -------------------------------------------------------------------------------------------------------------------------

	//Estaticos arboles y plantas
	Model Arbol1("resources/objects/OakTree/Oak_Tree.obj"); //1 
	Model Planta1("resources/objects/SugarCane/Sugar_Cane.obj"); //2
	Model Planta2("resources/objects/Fern/Fern1.obj"); //3
	Model Planta3("resources/objects/Yucca/Yucca1.obj"); //4
	Model Planta4("resources/objects/Aloe/Aloe1.obj"); //5
	Model Planta5("resources/objects/RectGrass/RectGrass1.obj"); //6
	Model Planta6("resources/objects/RectGrass2/RectGrass2.obj"); //7
	Model Planta7("resources/objects/Rose/Rose1.obj"); //8
	Model Planta8("resources/objects/Cactus/Cactus1.obj"); //9
	Model Planta9("resources/objects/Sunflower/Sunflower1.obj"); //10
	Model Planta10("resources/objects/Flower/Flower1.obj"); //11
	Model Planta11("resources/objects/RoundGrass/RoundGrass1.obj"); //12
	Model Suelo1("resources/objects/CircularGrass/CircularGrass1.obj"); //13
	Model Barda1("resources/objects/Fence/Fence.obj"); //14
	Model Barda2("resources/objects/Fence2/Fence2.obj"); //15
	Model Banco1("resources/objects/StoneBench1/StoneBech1.obj"); //16
	Model Banco2("resources/objects/WoodBench/WoodBench1.obj"); //17
	Model Banco3("resources/objects/WoodBench2/WoodBench2.obj"); //18
	Model Banco4("resources/objects/WoodBench3/WoodBench3.obj"); //19
	Model Banco5("resources/objects/ConcreteBench1/ConcreteBench1.obj"); //20
	Model Banco6("resources/objects/ConcreteBench2/ConcreteBench2.obj"); //21
	Model Suelo2("resources/objects/RectGrassPatch/RectGrassPatch1.obj"); //22
	Model Arbol2("resources/objects/Tree2/Tree2.obj"); //23
	Model Edificio1("resources/objects/Building1/Building1.obj"); //24

	//Muebles de ambiente

	Model Estatua1("resources/objects/Statue/Statue1.obj"); //25
	Model Pared1("resources/objects/White1/White1.obj"); //26 
	Model Puerta1("resources/objects/Door1/Door1.obj"); //27 
	Model Puerta2("resources/objects/Door2/Door2.obj"); //28 
	Model Puerta3("resources/objects/Door3/Door3.obj"); //29 
	Model Puerta4("resources/objects/Door4/Door4.obj"); //30
	Model Cristal1("resources/objects/Crystal/Crystal1.obj"); //31 
	Model Cristal2("resources/objects/Crystal2/Crystal2.obj"); //32 
	Model Cristal3("resources/objects/Crystal3/Crystal3.obj"); //33 

	//Decoracion de salon

	Model Mesa1("resources/objects/Table1/Table1.obj"); //34
	Model Mesa2("resources/objects/Table2/Table2.obj"); //35
	Model Silla1("resources/objects/Chair1/Chair1.obj"); //36
	Model Silla2("resources/objects/Chair2/Chair2.obj"); //37
	Model Pizaron1("resources/objects/Board/Board1.obj"); //38
	Model Closet1("resources/objects/Closet1/Closet1.obj"); //39


	//Equipos de computo

	Model GabineteCpu1("resources/objects/CPU_Gabinet/Gabinet1.obj"); //40
	Model GabineteCpu2("resources/objects/CPU_Gabinet2/CPU2.obj"); //41
	Model Monitor1("resources/objects/LCD/LCD1.obj"); //42
	Model Monitor2("resources/objects/LCD2/LCD2.obj"); //43
	Model Tecleado1("resources/objects/Keyboard/Keyboard1.obj"); //44
	Model Tecleado2("resources/objects/Keyboard2/Keyboard2.obj"); //45
	Model Mouse1("resources/objects/Mouse/Mouse1.obj"); //46
	Model Mouse2("resources/objects/Mouse2/Mouse2.obj"); //47

	// -------------------------------------------------------------------------------------------------------------------------
	// Modelos Alicia
	// -------------------------------------------------------------------------------------------------------------------------
	Model arbol("resources/objects/arbol/candytree.obj");
	Model pajaro("resources/objects/pajaro/bird.dae");
	Model raton("resources/objects/raton/mr721_09.dae");
	Model extinguidor("resources/objects/extinguidor/ROBJ_Extinguisher.dae");
	Model sol("resources/objects/sol/sol.obj");
	Model ala_der("resources/objects/pajaro/ala_der.obj");
	Model ala_izq("resources/objects/pajaro/ala_izq.obj");
	Model cuerpo_paja("resources/objects/pajaro/cuerpo.obj");
	Model bici("resources/objects/bici_lego/bici.obj");
	Model llantas("resources/objects/bici_lego/llantas.obj");
	Model soporte("resources/objects/camara/soporte.obj");
	Model camara("resources/objects/camara/camara.obj");

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].giro_bici = 0;
	}


	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 tempCuerpo = glm::mat4(1.0f);
	glm::mat4 tempBici = glm::mat4(1.0f);
	glm::mat4 tempCam = glm::mat4(1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		updateAudio();

		contador_sol += 0.05;
		contador_paja += 0.4;
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", ambientColor);
		staticShader.setVec3("dirLight.diffuse", diffuseColor);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Setup shader for primitives
		myShader.use();
		// view/projection transformations
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
		viewOp = camera.GetViewMatrix();
		// pass them to the shaders
		//myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/

		//Model

		//Building render
		glBindVertexArray(VAO[1]);

		float tx, ty, tz, sx, sy, sz, r, g, b;

		float points[124][9] =
		{//   tx     ty     tz    sx     sy     sz     r     g     b
			{-88.0f, 26.0f, 102.0f,  56.0f,4.0f, 340.0f, 0.502f, 0.502f, 0.502f}, // 0
			{-88.0f, -1.0f, 102.0f,  72.0f,2.0f, 344.0f, 0.502f, 0.502f, 0.502f}, // 1
			{88.0f, 26.0f, 0.0f,  56.0f,4.0f, 268.0f, 0.502f, 0.502f, 0.502f}, // 2
			{88.0f, -1.0f, 0.0f,  72.0f,2.0f, 276.0f, 0.502f, 0.502f, 0.502f}, // 3
			{0.0f, 66.0f, 36.0f,  120.0f,84.0f, 68.0f, 0.502f, 0.502f, 0.502f}, // 4
			{0.0f, -1.0f, 36.0f,  120.0f,2.0f, 68.0f, 0.502f, 0.502f, 0.502f}, // 5
			{0.0f, -1.0f, -2.0f,  104.0f,2.0f, 8.0f, 0.502f, 0.502f, 0.502f}, // 6
			//Fachada
			{-18.0f, 54.0f, 0.0f,  4.0f,108.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 7 (Columna fachada frontal 1)
			{ 18.0f, 54.0f, 0.0f,  4.0f,108.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 8 (Columna fachada frontal 2)
			{-56.0f, 54.0f, 0.0f,  8.0f,108.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 9 (Columna fachada frontal 3)
			{ 56.0f, 54.0f, 0.0f,  8.0f,108.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 10 (Columna fachada frontal 4)
			{  0.0f, 28.0f, 0.0f, 32.0f,  8.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 11
			{  0.0f, 52.0f, 0.0f, 32.0f,  8.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 12
			{  0.0f, 78.0f, 0.0f, 32.0f, 12.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 13
			{  0.0f,102.0f, 0.0f, 32.0f, 12.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 14
			{-36.0f, 28.0f, 0.0f, 32.0f,  8.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 15
			{-36.0f, 52.0f, 0.0f, 32.0f,  8.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 16
			{-36.0f, 78.0f, 0.0f, 32.0f, 12.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 17
			{-36.0f,102.0f, 0.0f, 32.0f, 12.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 18
			{ 36.0f, 28.0f, 0.0f, 32.0f,  8.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 19
			{ 36.0f, 52.0f, 0.0f, 32.0f,  8.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 20
			{ 36.0f, 78.0f, 0.0f, 32.0f, 12.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 21
			{ 36.0f,102.0f, 0.0f, 32.0f, 12.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 22
			//Columnas I1
			{-56.0f, 16.0f, -34.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 23
			{-88.0f, 16.0f, -68.0f, 72.0f, 32.0f, 4.0f, 1.0f, 1.0f, 1.0f}, // 24
			//
			{-58.0f,  4.0f, -17.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 25
			{-58.0f,  4.0f, -51.0f,  4.0f,   8.0f,30.0f,1.0f, 1.0f, 1.0f}, // 26
			{-56.0f, 24.0f, -17.0f,  8.0f,  8.0f,30.0f,1.0f, 1.0f, 1.0f}, // 27
			{-56.0f, 24.0f, -51.0f,  8.0f,  8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 28
			//Columnas I2
			{-120.0f, 16.0f, -34.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 29
			{-125.5f, 16.0f,   7.5f,  19.0f, 32.0f, 19.0f, 0.502f, 0.502f, 0.502f}, // 30
			{-120.0f, 16.0f,   34.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 31
			{-120.0f, 16.0f,   68.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 32
			{-120.0f, 16.0f,  102.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 33
			{-120.0f, 16.0f,   136.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 34
			{-120.0f, 16.0f,   170.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 35
			{-125.5f, 16.0f,   196.5f,  19.0f, 32.0f, 19.0f, 0.502f, 0.502f, 0.502f}, // 36
			{-120.0f, 16.0f,   238.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 37
			{-88.0f, 16.0f, 272.0f, 72.0f, 32.0f, 4.0f, 1.0f, 1.0f, 1.0f}, // 38
			//
			{-118.0f,  4.0f, -17.0f,  4.0f,   8.0f,30.0f,1.0f, 1.0f, 1.0f}, // 39
			{-118.0f,  4.0f, -51.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 40
			{-120.0f, 24.0f, -17.0f,  8.0f,  8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 41
			{-120.0f, 24.0f, -51.0f,  8.0f,  8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 42
			{-118.0f,  4.0f, 24.5f,  4.0f,   8.0f,15.0f, 0.502f, 0.502f, 0.502f}, // 43
			{-118.0f,  4.0f, 51.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 44
			{-120.0f, 24.0f, 24.5f,  8.0f,  8.0f, 15.0f, 0.502f, 0.502f, 0.502f}, // 45
			{-120.0f, 24.0f, 51.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 46
			{-118.0f,  4.0f, 85.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 47
			{-118.0f,  4.0f, 119.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 48
			{-120.0f, 24.0f, 85.0f,  8.0f,  8.0f, 30.0f, 0.502f, 0.502f, 0.502f}, // 49
			{-120.0f, 24.0f, 119.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 50
			{-118.0f,  4.0f, 153.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 51
			{-118.0f,  4.0f, 179.5f,  4.0f,   8.0f,15.0f, 1.0f, 1.0f, 1.0f}, // 52
			{-120.0f, 24.0f, 153.0f,  8.0f,  8.0f, 30.0f, 0.502f, 0.502f, 0.502f}, // 53
			{-120.0f, 24.0f, 179.5f,  8.0f,  8.0f,15.0f, 1.0f, 1.0f, 1.0f}, // 54
			{-118.0f,  4.0f, 221.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 55
			{-118.0f,  4.0f, 255.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 56
			{-120.0f, 24.0f, 221.0f,  8.0f,  8.0f, 30.0f, 1.0f, 1.0f, 1.0f}, // 57
			{-120.0f, 24.0f, 255.0f,  8.0f,  8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 58
			// Columnas I3
			{-56.0f, 16.0f,   238.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 59
			{-56.0f, 16.0f,   204.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 60
			{-56.0f, 16.0f,  170.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 61
			{-56.0f, 16.0f,   136.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 62
			{-56.0f, 16.0f,  102.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 63
			{-56.0f, 12.0f,   68.0f,  8.0f, 24.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 64
			//
			{-58.0f,  4.0f, 255.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 65
			{-58.0f,  4.0f, 221.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 66
			{-56.0f, 24.0f, 255.0f,  8.0f,  8.0f, 30.0f, 1.0f, 1.0f, 1.0f}, // 67
			{-56.0f, 24.0f, 221.0f,  8.0f,  8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 68
			{-58.0f,  4.0f, 187.0f,  4.0f,   8.0f,30.0f, 1.0f, 1.0f, 1.0f}, // 69
			{-58.0f,  4.0f, 153.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 70
			{-56.0f, 24.0f, 187.0f,  8.0f,  8.0f, 30.0f, 1.0f, 1.0f, 1.0f}, // 71
			{-56.0f, 24.0f, 153.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 72
			{-58.0f,  4.0f, 119.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 73
			{-58.0f,  4.0f, 85.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 74
			{-56.0f, 24.0f, 119.0f,  8.0f,  8.0f, 30.0f, 0.502f, 0.502f, 0.502f}, // 75
			{-56.0f, 24.0f, 85.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 76

			//Divisor Lab grafica
			{-92.0f, 4.0f, -8.0f,  48.0f,  8.0f,1.0f, 1.0f, 1.0f, 1.0f}, // 77
			//Division I
			{-59.0f, 12.0f, 7.0f,  2.0f, 24.0f, 14.0f, 0.502f, 0.502f, 0.502f}, // 78
			{-59.0f, 12.0f, 42.0f,  2.0f, 24.0f, 14.0f, 0.502f, 0.502f, 0.502f}, // 79
			//Columnas D1
			{56.0f, 16.0f, -34.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 80
			{56.0f, 16.0f, -68.0f, 8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 81
			{56.0f, 16.0f, -102.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 82
			{88.0f, 16.0f, -136.0f, 72.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 83
			//
			{58.0f,  4.0f, -17.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 84
			{58.0f,  4.0f, -51.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 85
			{56.0f, 24.0f, -17.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 86
			{56.0f, 24.0f, -51.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 87
			{58.0f,  4.0f, -85.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 88
			{58.0f,  4.0f, -119.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 89
			{56.0f, 24.0f, -85.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 90
			{56.0f, 24.0f, -119.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 91
			//Columnas D2
			{120.0f, 16.0f, -102.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 92
			{120.0f, 16.0f,  -68.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 93
			{120.0f, 16.0f,  -34.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 94
			{120.0f, 16.0f,  0.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 95
			{120.0f, 16.0f,  34.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 96
			{120.0f, 16.0f,  68.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 97
			{120.0f, 16.0f,  102.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 98
			{88.0f, 16.0f, 136.0f, 72.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f}, // 99
			//
			{118.0f,  4.0f, -85.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 100
			{118.0f,  4.0f, -119.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 101
			{120.0f, 24.0f, -85.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 102
			{120.0f, 24.0f, -119.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 103
			{118.0f,  4.0f, -17.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 104
			{118.0f,  4.0f, -51.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 105
			{120.0f, 24.0f, -17.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 106
			{120.0f, 24.0f, -51.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 107
			{118.0f,  4.0f, 17.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 108
			{118.0f,  4.0f, 51.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 109
			{120.0f, 24.0f, 17.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 110
			{120.0f, 24.0f, 51.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 111
			{118.0f,  4.0f, 85.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 112
			{118.0f,  4.0f, 119.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 113
			{120.0f, 24.0f, 85.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 114
			{120.0f, 24.0f, 119.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f}, // 115
			// Columnas D3
			{ 56.0f, 16.0f, 102.0f,  8.0f, 32.0f, 4.0f, 0.502f, 0.502f, 0.502f }, // 116
			{ 56.0f, 12.0f, 68.0f, 8.0f, 24.0f, 4.0f, 0.502f, 0.502f, 0.502f }, // 117
			//
			{ 58.0f,  4.0f, 119.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f }, // 118
			{ 58.0f,  4.0f, 85.0f,  4.0f,   8.0f,30.0f, 0.502f, 0.502f, 0.502f }, // 119
			{ 56.0f, 24.0f, 119.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f }, // 120
			{ 56.0f, 24.0f, 85.0f,  8.0f,  8.0f,30.0f, 0.502f, 0.502f, 0.502f }, // 121
			//
			{-34.0f, 12.0f, 68.0f,  36.0f,  24.0f,4.0f, 0.502f, 0.502f, 0.502f }, // 122
			{ 34.0f, 12.0f, 68.0f,  36.0f,  24.0f,4.0f, 0.502f, 0.502f, 0.502f }, // 123
		};

		for (int i = 0; i < 124; i++)
		{
			tx = points[i][0];
			ty = points[i][1];
			tz = points[i][2];
			sx = points[i][3];
			sy = points[i][4];
			sz = points[i][5];
			r = points[i][6];
			g = points[i][7];
			b = points[i][8];

			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
			modelOp = glm::scale(modelOp, glm::vec3(sx, sy, sz));
			myShader.setMat4("model", modelOp);
			myShader.setVec3("aColor", glm::vec3(r, g, b));

			if (i == 24) {
				glBindTexture(GL_TEXTURE_2D, t_micro_f);
			}
			else if (i == 25 || i == 26 || i == 27 || i == 28) {
				glBindTexture(GL_TEXTURE_2D, t_micro_l);
			}
			else if (i == 38) {
				glBindTexture(GL_TEXTURE_2D, t_cisco_b);
			}
			else if (i == 39 || i == 40 || i == 41 || i == 42) {
				glBindTexture(GL_TEXTURE_2D, t_micro_r);
			}
			else if (i == 52 || (i >= 54 && i <= 58)) {
				glBindTexture(GL_TEXTURE_2D, t_cisco_r);
			}
			else if ((i >= 65 && i <= 69) || i == 71) {
				glBindTexture(GL_TEXTURE_2D, t_cisco_l);
			}
			else if (i == 77) {
				glBindTexture(GL_TEXTURE_2D, t_micro_b);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, t_concrete);
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glBindVertexArray(VAO[0]);
		//Colocar código aquí
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_glass);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion Mixamo Adolfo
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion Mixamo Ileana
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion Mixamo Alicia
		// -------------------------------------------------------------------------------------------------------------------------



		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		//Tener Piso como referencia
		glBindVertexArray(VAO[2]);
		//Colocar c digo aqu 
		modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 2.0f, 40.0f));
		modelOp = glm::translate(modelOp, glm::vec3(0.0f, -1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_grass);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[0]);
		//Colocar c digo aqu 
		/*modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_unam);
		//glDrawArrays(GL_TRIANGLES, 0, 36); //A lonely cube :(
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

		/***   Segundo objeto  **/
		/*
		glBindVertexArray(VAO[1]);
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_unam);
		glDrawArrays(GL_TRIANGLES, 0, 36); //A lonely cube :(
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		*/
		glBindVertexArray(0);
		// ------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.2f));
		staticShader.setMat4("model", modelOp);
		//piso.Draw(staticShader);


				//Estaticos, Decoración: Parte izquierda del edificio

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Arbol1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -90.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Arbol1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -125.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Arbol1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(70.0f, -1.9f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Arbol1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.5f, 6.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(63.5f, 6.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(86.5f, 6.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(109.5f, 6.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(132.5f, 6.0f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(30.5f, 6.0f, -158.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(30.5f, 6.0f, -181.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(30.5f, 6.0f, -204.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Barda2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -133.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -123.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -113.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -103.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -93.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -83.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -73.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -63.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -53.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -43.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -33.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -23.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -23.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -83.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -73.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -63.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -53.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -43.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -33.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta6.Draw(staticShader);

		//Plantas de decoracion

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -15.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.20f));
		staticShader.setMat4("model", modelOp);
		Planta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Planta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -26.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Planta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -20.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.80f));
		staticShader.setMat4("model", modelOp);
		Planta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -31.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.65f));
		staticShader.setMat4("model", modelOp);
		Planta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.9f, -35.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Planta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(29.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.65f));
		staticShader.setMat4("model", modelOp);
		Planta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Planta1.Draw(staticShader);

		//Decoración: Parte derecha del edificio

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-57.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-67.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-77.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-87.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-97.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-107.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-117.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-127.0f, -1.9f, -87.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-77.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Arbol1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-107.0f, -1.9f, -70.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Arbol1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-57.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-67.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-77.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-87.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-97.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-107.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-117.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-127.0f, -1.9f, -76.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.9f, -62.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Estatua1.Draw(staticShader);

		//Plantas de decoracion

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, -15.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.20f));
		staticShader.setMat4("model", modelOp);
		Planta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Planta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, -26.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Planta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, -20.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.80f));
		staticShader.setMat4("model", modelOp);
		Planta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, -31.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.65f));
		staticShader.setMat4("model", modelOp);
		Planta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.9f, -35.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Planta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-29.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.65f));
		staticShader.setMat4("model", modelOp);
		Planta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, -1.9f, -13.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Planta1.Draw(staticShader);

		//Muebles de ambiente

		//Bancos Izquierda

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -70.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, -108.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.9f, -70.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.9f, -80.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -1.9f, -80.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.9f, -95.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, -85.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Suelo2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, -85.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Arbol2.Draw(staticShader);

		//Bancos Derecha

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.9f, -50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.9f, -63.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.06f));
		staticShader.setMat4("model", modelOp);
		Planta11.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-45.0f, -1.9f, -75.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco4.Draw(staticShader);

		//Edificios

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, -190.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Edificio1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-130.0f, -1.9f, -190.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		staticShader.setMat4("model", modelOp);
		Edificio1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-27.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-37.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-47.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-57.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-67.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-77.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-87.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-97.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-107.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-117.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-127.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-137.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-147.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-157.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-167.0f, -1.9f, -157.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.05f));
		staticShader.setMat4("model", modelOp);
		Planta5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, -1.9f, -135.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-32.0f, -1.9f, -135.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, -135.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco5.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, -1.9f, -145.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-65.5f, -1.9f, -145.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Banco6.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-11.0f, 2.5f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Suelo2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-38.0f, 2.5f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Suelo2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 2.5f, -150.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.09f));
		staticShader.setMat4("model", modelOp);
		Suelo2.Draw(staticShader);

		//Relleno de espacios ventanas FRENTE

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-42.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(35.0, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(42.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		//Limitado del salon de amarillo


		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-85.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		//Limitado el salon

		//ATRAS

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta1.Draw(staticShader);

		//Lado derecho

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 10.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 20.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 40.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, 60.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta3.Draw(staticShader);

		//Lado Izquierdo

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 60.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 10.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 20.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 40.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta4.Draw(staticShader);

		//ADELANTE

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-85.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(115.0f, -1.9f, 5.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal1.Draw(staticShader);

		//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -50.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -60.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -40.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -50.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		//Relleno de espacios ventanas Edificios de lado derecho

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -17.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -7.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -17.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -50.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -60.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -40.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-55.0f, -1.9f, -50.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		//Relleno de espacios ventanas Edificios de lado izquierdo

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(52.0f, -1.9f, -17.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -7.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -17.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -30.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(52.0f, -1.9f, -50.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -60.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -40.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -50.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(52.0f, -1.9f, -83.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -98.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -70.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -83.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		//

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(52.0f, -1.9f, -116.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.17f));
		staticShader.setMat4("model", modelOp);
		Cristal3.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -126.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -96.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(55.0f, -1.9f, -116.5f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Puerta2.Draw(staticShader);

						//MODELOS Y DECORACIÓN DEL SALON

		//Derecha Mesas

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, 55.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Silla2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, 52.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-34.5f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-24.5f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-34.5f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-24.5f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		//Equipos derecha

		//MESA 1

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-39.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-29.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-19.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-33.5f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-33.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);
		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-47.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-36.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-26.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		//MESA 2

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-39.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-29.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-19.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-33.5f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-33.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-47.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-36.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-26.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);


		// Izquierda Mesas

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -1.9f, 45.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(43.0f, -1.9f, 45.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, -1.9f, 45.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(34.5f, -1.9f, 45.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(24.5f, -1.9f, 45.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.9f, 45.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(43.0f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(34.5f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(24.5f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.9f, 27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(43.0f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.13f));
		staticShader.setMat4("model", modelOp);
		Mesa1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(34.5f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(24.5f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.9f, 9.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.50f));
		staticShader.setMat4("model", modelOp);
		Silla1.Draw(staticShader);

		// Equipos izquierda

		//MESA 1
		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(49.0f, 7.9f, 51.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(39.0f, 7.9f, 51.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(29.0f, 7.9f, 51.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(19.0f, 7.9f, 51.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 7.9f, 52.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.5f, 7.9f, 52.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(23.5f, 7.9f, 52.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 7.9f, 52.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.5f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(23.5f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(41.5f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(31.0f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(21.0f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(11.0f, 7.9f, 50.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);


		//MESA 2

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(49.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(39.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(29.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(19.0f, 7.9f, 33.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.5f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(23.5f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 7.9f, 34.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(23.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(41.5f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(31.0f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(21.0f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(11.0f, 7.9f, 32.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		//MESA 3

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(49.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(39.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(29.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(19.0f, 7.9f, 15.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.5f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(23.5f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 7.9f, 16.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Monitor1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(33.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(23.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Tecleado1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(41.5f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(31.0f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(21.0f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(11.0f, 7.9f, 14.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.08f));
		staticShader.setMat4("model", modelOp);
		Mouse1.Draw(staticShader);


		//Decoración adicional

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 4.0f, 63.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.50f));
		staticShader.setMat4("model", modelOp);
		Pizaron1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(14.0f, -1.9f, 49.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Closet1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(26.0f, -1.9f, 49.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Closet1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(38.0f, -1.9f, 49.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Closet1.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -1.9f, 49.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.10f));
		staticShader.setMat4("model", modelOp);
		Closet1.Draw(staticShader);

		//Escritorio profesor

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0f, 7.9f, 51.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		GabineteCpu2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-39.5f, 7.9f, 49.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Monitor2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-39.5f, 7.9f, 53.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Tecleado2.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-32.5f, 7.9f, 53.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.15f));
		staticShader.setMat4("model", modelOp);
		Mouse2.Draw(staticShader);







		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion y Estaticos Alicia
		// -------------------------------------------------------------------------------------------------------------------------
		//sol
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-490.0f, 1000.0f, -310.0));
		modelOp = glm::rotate(modelOp, contador_sol, glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		sol.Draw(staticShader);

		//pájaro
		// cuerpo
		modelOp = glm::rotate(glm::mat4(1.0f), glm::radians(contador_paja), glm::vec3(0.0f, 1.0f, 0.0f));
		tempCuerpo = modelOp = glm::translate(modelOp, glm::vec3(-300.0f, 500.0f, -200.0));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		cuerpo_paja.Draw(staticShader);

		//ala_der
		modelOp = glm::translate(tempCuerpo, glm::vec3(0.0f, 0.0f, 0.0));
		modelOp = glm::rotate(modelOp, glm::radians((30 * sin(contador_paja))), glm::vec3(0.0f, 0.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		ala_der.Draw(staticShader);

		//ala_izq
		modelOp = glm::translate(tempCuerpo, glm::vec3(0.0f, 0.0f, 0.0));
		modelOp = glm::rotate(modelOp, glm::radians(-(30 * sin(contador_paja))), glm::vec3(0.0f, 0.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		ala_izq.Draw(staticShader);

		//bici
		tempBici = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f + posX, -1.5f, -150.0 + posZ));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giro_bici), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		bici.Draw(staticShader);

		//llantas
		modelOp = glm::translate(tempBici, glm::vec3(0.0f, 0.0f, 0.0));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(giro_bici), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		llantas.Draw(staticShader);

		//cámara 
		//soporte
		tempCam = modelOp = glm::translate(modelOp, glm::vec3(-50.0f, 26.0f, 60.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		soporte.Draw(staticShader);

		//animación cámara 
		modelOp = glm::translate(tempCam, glm::vec3(0.0f, 0.0f, 0.0f)); 
		modelOp = glm::rotate(modelOp, glm::radians(anguloNegar), glm::vec3(0.0f, 1.0f, 0.0f)); 
		modelOp = glm::scale(modelOp, glm::vec3(1.0f));
		staticShader.setMat4("model", modelOp);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		camara.Draw(staticShader);

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//skybox.Terminate();
	engine->drop();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giro_bici--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giro_bici++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Bici animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		for (int i = 0; i < MAX_FRAMES; i++)
		{
			KeyFrame[i].posY = 0;
			KeyFrame[0].posX = 0;
			KeyFrame[0].posZ = 0;
			KeyFrame[0].giro_bici = 0;
			KeyFrame[1].posX = 21;
			KeyFrame[1].posZ = 0;
			KeyFrame[1].giro_bici = 0;
			KeyFrame[2].posX = 21;
			KeyFrame[2].posZ = 0;
			KeyFrame[2].giro_bici = -96;
			KeyFrame[3].posX = 21;
			KeyFrame[3].posZ = 51;
			KeyFrame[3].giro_bici = -96;
			KeyFrame[4].posX = 21;
			KeyFrame[4].posZ = 51;
			KeyFrame[4].giro_bici = -15;
			KeyFrame[5].posX = 34;
			KeyFrame[5].posZ = 51;
			KeyFrame[5].giro_bici = -15;
			FrameIndex = 6;
		}
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//camara
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		animarCamara = !animarCamara; // alterna entre true y false
		std::cout << "Animación de cámara: " << (animarCamara ? "Activada" : "Desactivada") << std::endl;
	}


	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	//Camera Positions
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) setCameraFrontView();
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) setCameraLabView();
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) setCameraCiscoView();
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) setCameraIsoView();
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) setCameraBiciAnimation();
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) setCameraAnimation();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


//Carga los archivos .ogg y .mp3 de la carpeta
void loadPlaylist(const std::string& folderPath) {
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.path().extension() == ".ogg" || entry.path().extension() == ".mp3") {
			playlist.push_back(entry.path().string());
		}
	}
}

//Reproduce el siguiente de la playlist
void playNext() {
	if (playlist.empty()) return;

	//si hay uno sonando, lo paramos
	if (currentSound && !currentSound->isFinished()) {
		currentSound->stop();
	}

	//reproduce el siguiente
	currentSound = engine->play2D(playlist[currentIndex].c_str(), false, false, true);

	currentIndex = (currentIndex + 1) % playlist.size();
}

//Función para actualizar el audio
void updateAudio() {
	if (!currentSound || currentSound->isFinished()) {
		playNext();
	}
}

//posiciones de la cámara 
void setCameraFrontView() {
	camera.Position = glm::vec3(-10.0f, 7.0f, -100.0);
	camera.Front = glm::normalize(glm::vec3(0.0f, 5.0f, 20.0f));
}

void setCameraLabView() {
	camera.Position = glm::vec3(-118.0f, 10.0f, 150.0f);
	camera.Front = glm::normalize(glm::vec3(5.0f, -2.0f, 0.0f));
}

void setCameraCiscoView() {
	camera.Position = glm::vec3(3.0f, 1.5f, -3.0f);
	camera.Front = glm::normalize(glm::vec3(-5.0f, -2.0f, 0.0f));
}

void setCameraIsoView() {
	camera.Position = glm::vec3(200.0f, 220.0f, -350.0f);
	camera.Front = glm::normalize(glm::vec3(-5.0f, -7.0f, 10.0f));
}

void setCameraBiciAnimation() {
	camera.Position = glm::vec3(-100.0f, 13.0f, -120.0);
	camera.Front = glm::normalize(glm::vec3(-5.0f, -7.0f, 10.0f));
}

void setCameraAnimation() {
	camera.Position = glm::vec3(60.0f, 85.0f, -100.0f);
	camera.Front = glm::normalize(glm::vec3(-5.0f, -7.0f, 10.0f));
}

