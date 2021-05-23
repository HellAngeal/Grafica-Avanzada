#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shader/shader.h>

#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camara.h"

using namespace std;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
unsigned int loadCubeMap(vector<string> faces);
void BuildCube(float _ancho, float _alto, float largo, Shader _objShader, unsigned int _VBO, unsigned int _VAO, unsigned int _EBO);
void DrawCube(Shader _objShader, unsigned int _VAO, glm::vec3 pos, float angles, int axis, unsigned int _diffuseMap, unsigned int _specularMap);
//medidas de la pantalla
const unsigned int largo = 1000;
const unsigned int ancho = 850;

//configuraciones para nuestra camara
Camara cam(glm::vec3(2.0f, 0.0f, 2.0f));
float lastX = ancho / 2.0f;
float lastY = largo / 2.0f;
bool firstMouse = true;


//timing
float deltaTime = 0.0f; //marca el tiempo transcurrido entre el frame mas actual y el anterior
float lastFrame = 0.0f;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int main()
{
    //inicialiamos glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creacion de nuestra ventanita
    GLFWwindow* window = glfwCreateWindow(largo, ancho, "Casa minimalista", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "fallaste hijo de toda tu putisima madre" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //le indicamos a glfw que reciba datos del nuestro mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "oye wey no se esta ejecutando xd" << std::endl;
        return -1;
    }
    //comprobacion del buffer de profundidad en el eje z
    glEnable(GL_DEPTH_TEST);
    //Declarar nuestro programa shader y sus complicaciones
    Shader nuestroShader("luces1.vs", "luces1.fs"); //nuestro cubo que va a recibir la luz
    Shader otroShader("luces2.vs", "luces2.fs"); //el origen de la luz
    Shader skyboxShader("skybox.vs", "skybox.fs"); //el origen de la luz

    float vertices[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    float skybox[] = {
        -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    unsigned int indices[]{

        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        14, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    glm::vec3 posicionesCubo[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int skybxVBO, skybxVAO;
    glGenVertexArrays(1, &skybxVAO);
    glGenBuffers(1, &skybxVBO);
    glBindVertexArray(skybxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skybxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), &skybox, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<string> skyboxFaces
    {
        ("skybox/right.jpg"),
        ("skybox/left.jpg"),
        ("skybox/top.jpg"),
        ("skybox/bottom.jpg"),
        ("skybox/front.jpg"),
        ("skybox/back.jpg")

    };
    unsigned int VBO, VAO, EBO;
    unsigned int VBO2, VAO2;
    unsigned int VBO3, VAO3;
    unsigned int VBO4, VAO4;
    unsigned int VBO5, VAO5;
    unsigned int VBO6, VAO6;
    unsigned int VBO7, VAO7;
    unsigned int VBO8, VAO8;
    unsigned int VBO9, VAO9;
    unsigned int VBO10, VAO10;
    unsigned int VBO11, VAO11;
    unsigned int VBO12, VAO12;
    unsigned int VBO13, VAO13;
    unsigned int VBO14, VAO14;
    unsigned int VBO15, VAO15;
    unsigned int VBO16, VAO16;
    unsigned int VBO17, VAO17;
    unsigned int VBO18, VAO18;
    unsigned int VBO19, VAO19;
    unsigned int VBO20, VAO20;
    unsigned int VBO21, VAO21;
    unsigned int VBO22, VAO22;
    unsigned int VBO23, VAO23;
    unsigned int VBO24, VAO24;
    unsigned int VBO25, VAO25;
    unsigned int VBO26, VAO26;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glGenVertexArrays(1, &VAO6);
    glGenBuffers(1, &VBO6);
    glGenVertexArrays(1, &VAO7);
    glGenBuffers(1, &VBO7);
    glGenVertexArrays(1, &VAO8);
    glGenBuffers(1, &VBO8);
    glGenVertexArrays(1, &VAO9);
    glGenBuffers(1, &VBO9);
    glGenVertexArrays(1, &VAO10);
    glGenBuffers(1, &VBO10);
    glGenVertexArrays(1, &VAO11);
    glGenBuffers(1, &VBO11);
    glGenVertexArrays(1, &VAO12);
    glGenBuffers(1, &VBO12);
    glGenVertexArrays(1, &VAO13);
    glGenBuffers(1, &VBO13);
    glGenVertexArrays(1, &VAO14);
    glGenBuffers(1, &VBO14);
    glGenVertexArrays(1, &VAO15);
    glGenBuffers(1, &VBO15);
    glGenVertexArrays(1, &VAO16);
    glGenBuffers(1, &VBO16);
    glGenVertexArrays(1, &VAO17);
    glGenBuffers(1, &VBO17);
    glGenVertexArrays(1, &VAO18);
    glGenBuffers(1, &VBO18);
    glGenVertexArrays(1, &VAO19);
    glGenBuffers(1, &VBO19);
    glGenVertexArrays(1, &VAO20);
    glGenBuffers(1, &VBO20);
    glGenVertexArrays(1, &VAO21);
    glGenBuffers(1, &VBO21);
    glGenVertexArrays(1, &VAO22);
    glGenBuffers(1, &VBO22);
    glGenVertexArrays(1, &VAO23);
    glGenBuffers(1, &VBO23);
    glGenVertexArrays(1, &VAO24);
    glGenBuffers(1, &VBO24);
    glGenVertexArrays(1, &VAO25);
    glGenBuffers(1, &VBO25);
    glGenVertexArrays(1, &VAO26);
    glGenBuffers(1, &VBO26);


    unsigned int diffuseMap = loadTexture("ghost.png");
    unsigned int LwoodMap = loadTexture("lightwood.jpg");
    unsigned int WwindowMap = loadTexture("woodwindow.png");
    unsigned int BbrickMap = loadTexture("blackbrick.jpg");
    unsigned int WbrickMap = loadTexture("whitebrick.jpg");
    unsigned int DoorMap = loadTexture("door.jpg");
    unsigned int GrassMap = loadTexture("grass.jpg");
    unsigned int WoodMap = loadTexture("wood1.jpg");
    unsigned int CoachMap = loadTexture("coach.jpg");
    unsigned int TvMap = loadTexture("tv.png");
    unsigned int BedMap = loadTexture("colchon.jpg");

    ////Piso
    BuildCube(100.0f, 1.0f, 100.0f, nuestroShader, VBO, VAO, EBO);
    BuildCube(70.0f, 1.0f, 30.0f, nuestroShader, VBO15, VAO15, EBO);

    ////Paredes
    BuildCube(1.0f, 50.0f, 100.0f, nuestroShader, VBO2, VAO2, EBO);
    BuildCube(100.0f, 50.0f, 1.0f, nuestroShader, VBO9, VAO9, EBO);
    BuildCube(70.0f, 50.0f, 1.0f, nuestroShader, VBO4, VAO4, EBO);
    BuildCube(1.0f, 50.0f, 30.0f, nuestroShader, VBO5, VAO5, EBO);
    BuildCube(30.0f, 50.0f, 1.0f, nuestroShader, VBO6, VAO6, EBO);

    //Ventana
    BuildCube(1.0f, 15.0f, 60.0f, nuestroShader, VBO3, VAO3, EBO);

    //Door
    BuildCube(1.0f, 20.0f, 15.0f, nuestroShader, VBO7, VAO7, EBO);
    BuildCube(15.0f, 20.0f, 1.0f, nuestroShader, VBO8, VAO8, EBO);

    //terraza
    BuildCube(66.0f, 1.0f, 30.0f, nuestroShader, VBO10, VAO10, EBO);
    BuildCube(35.0f, 1.0f, 100.0f, nuestroShader, VBO11, VAO11, EBO);
    BuildCube(35.0f, 15.0f, 1.0f, nuestroShader, VBO12, VAO12, EBO);
    BuildCube(1.0f, 15.0f, 130.0f, nuestroShader, VBO13, VAO13, EBO);
    BuildCube(66.0f, 15.0f, 1.0f, nuestroShader, VBO14, VAO14, EBO);

    //Grass
    BuildCube(200.0f, 1.0f, 200.0f, nuestroShader, VBO16, VAO16, EBO);

    //Viga de soporte
    BuildCube(5.0f, 26.5f, 5.0f, nuestroShader, VBO17, VAO17, EBO);

    //Coach
    BuildCube(30.0f, 10.0f, 2.0f, nuestroShader, VBO18, VAO18, EBO);
    BuildCube(2.0f, 7.0f, 6.0f, nuestroShader, VBO19, VAO19, EBO);
    BuildCube(26.0f, 4.5f, 4.0f, nuestroShader, VBO20, VAO20, EBO);

    //Tv
    BuildCube(20.0f, 10.0f, 1.0f, nuestroShader, VBO21, VAO21, EBO);

    //Table
    BuildCube(1.0f, 10.0f, 1.0f, nuestroShader, VBO22, VAO22, EBO);
    BuildCube(15.0f, 1.0f, 20.0f, nuestroShader, VBO23, VAO23, EBO);

    //Chair
    BuildCube(1.0f, 5.0f, 1.0f, nuestroShader, VBO24, VAO24, EBO);
    BuildCube(4.0f, 1.0f, 4.0f, nuestroShader, VBO25, VAO25, EBO);

    //Colchon
    BuildCube(15.0f, 2.0f, 20.0f, nuestroShader, VBO26, VAO26, EBO);

    unsigned int cubemapTexture = loadCubeMap(skyboxFaces);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

      //loop de dibujado
    while (!glfwWindowShouldClose(window))
    {

        // carcular el tiempo entre los frames para poder implementar le movimiento de la camara
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //entradas
        processInput(window);

        //Renderizado
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 pos(0, 0, 0);
        ////Piso
        DrawCube(nuestroShader, VAO, glm::vec3(0.0f, -7.5f, -10), 0, 0, diffuseMap, BbrickMap);

        //piso blanco
        DrawCube(nuestroShader, VAO15, glm::vec3(0.0f, -7.5f, 90), 0, 0, diffuseMap, WbrickMap);

        //Techo negro
        DrawCube(nuestroShader, VAO, glm::vec3(0.0f, 42.5f, -10), 0, 0, diffuseMap, BbrickMap);
        DrawCube(nuestroShader, VAO, glm::vec3(0.0f, 20.0f, -10), 0, 0, diffuseMap, BbrickMap);

        //techo blanco
        DrawCube(nuestroShader, VAO15, glm::vec3(0.0f, 42.5f, 90), 0, 0, diffuseMap, WbrickMap);

        ////Paredes negras
        DrawCube(nuestroShader, VAO2, glm::vec3(0.0f, -7.5f, -10), 0, 0, diffuseMap, BbrickMap);
        DrawCube(nuestroShader, VAO2, glm::vec3(100.0f, -7.5f, -10), 0, 0, diffuseMap, BbrickMap);
        DrawCube(nuestroShader, VAO9, glm::vec3(0.0f, -7.5f, -10), 0, 0, diffuseMap, BbrickMap);
        DrawCube(nuestroShader, VAO6, glm::vec3(70.0f, -7.5f, 90), 0, 0, diffuseMap, BbrickMap);

        //paredes blancas
        DrawCube(nuestroShader, VAO4, glm::vec3(0.0f, -7.5f, 120), 0, 0, diffuseMap, WbrickMap);
        DrawCube(nuestroShader, VAO5, glm::vec3(0.0f, -7.5f, 90), 0, 0, diffuseMap, WbrickMap);
        DrawCube(nuestroShader, VAO5, glm::vec3(70.0f, -7.5f, 90), 0, 0, diffuseMap, WbrickMap);

        //ventanas
        DrawCube(nuestroShader, VAO3, glm::vec3(101.0f, 0.0f, 10), 0, 0, diffuseMap, WwindowMap);
        DrawCube(nuestroShader, VAO3, glm::vec3(101.0f, 20.0f, 10), 0, 0, diffuseMap, WwindowMap);

        //Door
        DrawCube(nuestroShader, VAO7, glm::vec3(71.0f, -7.5f, 97.5), 0, 0, diffuseMap, DoorMap);
        DrawCube(nuestroShader, VAO8, glm::vec3(77.5f, 20.0f, 91), 0, 0, diffuseMap, DoorMap);

        //terraza
        DrawCube(nuestroShader, VAO10, glm::vec3(70.0f, 19.0f, 90), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO11, glm::vec3(101.0f, 19.0f, -10), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO12, glm::vec3(101.0f, 19.0f, -10), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO13, glm::vec3(136.0f, 19.0f, -10), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO14, glm::vec3(71.0f, 19.0f, 120), 0, 0, diffuseMap, LwoodMap);

        //grass
        DrawCube(nuestroShader, VAO16, glm::vec3(-40.0f, -8.5f, -45), 0, 0, diffuseMap, GrassMap);

        //viga de soporte
        DrawCube(nuestroShader, VAO17, glm::vec3(132.0f, -7.5f, 116), 0, 0, diffuseMap, WoodMap);

        //Coach
        DrawCube(nuestroShader, VAO18, glm::vec3(30.0f, -6.5f, 70), 0, 0, diffuseMap, WoodMap);
        DrawCube(nuestroShader, VAO19, glm::vec3(30.0f, -6.5f, 64), 0, 0, diffuseMap, WoodMap);
        DrawCube(nuestroShader, VAO19, glm::vec3(58.0f, -6.5f, 64), 0, 0, diffuseMap, WoodMap);
        DrawCube(nuestroShader, VAO20, glm::vec3(32.0f, -6.5f, 66), 0, 0, diffuseMap, WoodMap);

        //TV
        DrawCube(nuestroShader, VAO21, glm::vec3(30.0f, 5.0f, -9), 0, 0, diffuseMap, TvMap);

        //table
        DrawCube(nuestroShader, VAO22, glm::vec3(30.0f, -6.5f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO22, glm::vec3(30.0f, -6.5f, 39), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO22, glm::vec3(44.0f, -6.5f, 39), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO22, glm::vec3(44.0f, -6.5f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO23, glm::vec3(30.0f, 3.5f, 20), 0, 0, diffuseMap, LwoodMap);

        DrawCube(nuestroShader, VAO22, glm::vec3(110.0f, 20.0f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO22, glm::vec3(110.0f, 20.0f, 39), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO22, glm::vec3(124.0f, 20.0f, 39), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO22, glm::vec3(124.0f, 20.0f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO23, glm::vec3(110.0f, 30.0f, 20), 0, 0, diffuseMap, LwoodMap);

        //Chair
        DrawCube(nuestroShader, VAO24, glm::vec3(36.0f, -6.5f, 41), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(36.0f, -6.5f, 44), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(39.0f, -6.5f, 44), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(39.0f, -6.5f, 41), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO25, glm::vec3(36.0f, -1.5f, 41), 0, 0, diffuseMap, LwoodMap);

        DrawCube(nuestroShader, VAO24, glm::vec3(115.0f, 20.0f, 41), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(115.0f, 20.0f, 44), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(118.0f, 20.0f, 44), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(118.0f, 20.0f, 41), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO25, glm::vec3(115.0f, 25.0f, 41), 0, 0, diffuseMap, LwoodMap);

        //Cama
        DrawCube(nuestroShader, VAO24, glm::vec3(50.0f, 21.0f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(50.0f, 21.0f, 39), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(64.0f, 21.0f, 39), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO24, glm::vec3(64.0f, 21.0f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO23, glm::vec3(50.0f, 26.0f, 20), 0, 0, diffuseMap, LwoodMap);
        DrawCube(nuestroShader, VAO26, glm::vec3(50.0f, 27.0f, 20), 0, 0, diffuseMap, BedMap);

//mvp 
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)ancho / (float)largo, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();
        nuestroShader.setMat4("projection", projection);
        nuestroShader.setMat4("view", view);

        ////SKYBOX//////

        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        glBindVertexArray(skybxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    ////SKYBOX//////
    glDeleteVertexArrays(1, &skybxVAO);
    glDeleteVertexArrays(1, &skybxVAO);

    glfwTerminate();
    return 0;
}

void BuildCube(float _ancho, float _alto, float _largo, Shader _objShader, unsigned int _VBO, unsigned int _VAO, unsigned int _EBO) {


    float vertex[] = {
        ////--X--/////////--Y--///////--Z--
         0.0f,          0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.0f + _ancho, 0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f,          0.0f + _alto, 0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
         0.0f,          0.0f,         0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

         ////--X--/////////--Y--/////////--Z--
         0.0f,          0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.0f + _ancho, 0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f + _ancho, 0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.0f,          0.0f + _alto, 0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
         0.0f,          0.0f,         0.0f + _largo,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

         //--X--/////--Y--/////////--Z--
           0.0f,  0.0f + _alto, 0.0f + _largo, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           0.0f,  0.0f + _alto, 0.0f,         -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           0.0f,  0.0f,         0.0f,         -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f,  0.0f,         0.0f,         -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f,  0.0f,         0.0f + _largo, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           0.0f,  0.0f + _alto, 0.0f + _largo, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           ///--X--///////////--Y--/////////--Z--
           0.0f + _ancho,  0.0f + _alto, 0.0f + _largo,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           0.0f + _ancho,  0.0f + _alto, 0.0f,          1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           0.0f + _ancho,  0.0f,         0.0f,          1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho,  0.0f,         0.0f,          1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho,  0.0f,         0.0f + _largo,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           0.0f + _ancho,  0.0f + _alto, 0.0f + _largo,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           ///--X--///////--Y--//////--Z--
           0.0f,          0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho, 0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
           0.0f + _ancho, 0.0f,  0.0f + _largo,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           0.0f + _ancho, 0.0f,  0.0f + _largo,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           0.0f,          0.0f,  0.0f + _largo,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
           0.0f,          0.0f,  0.0f,          0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

           ///--X--//////////--Y--/////////--Z--
           0.0f,          0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
           0.0f + _ancho, 0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
           0.0f + _ancho, 0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           0.0f + _ancho, 0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           0.0f,          0.0f + _alto,  0.0f + _largo,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
           0.0f,          0.0f + _alto,  0.0f,          0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int indices[]{

      0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        14, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35

    };

    //Unificamos
    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //atributos de posicion 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //atributos de colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //attribs de testuras :v
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    _objShader.use();
    _objShader.setInt("material.diffuse", 1);
    _objShader.setInt("material.specular", 0);

}

void DrawCube(Shader _objShader, unsigned int _VAO, glm::vec3 pos, float angles, int axis, unsigned int _diffuseMap, unsigned int _specularMap) {

    //activar las caracteristicas de luz en nuestro objeto
    _objShader.use();
    _objShader.setVec3("light.position", cam.Position);
    _objShader.setVec3("light.direction", cam.Front);
    _objShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    _objShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    _objShader.setVec3("viewPos", cam.Position);

    //propiedades de la luz
    _objShader.setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
    _objShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    _objShader.setVec3("light.specular", 3.0f, 3.0f, 3.0f);
    _objShader.setFloat("light.constant", 1.0f);
    _objShader.setFloat("light.linear", 0.045f);
    _objShader.setFloat("light.quadratic", 0.0075f);

    //propiedades de los materiales
    _objShader.setFloat("material.shininess", 64.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _specularMap);
    glBindVertexArray(_VAO);
    //configuraciones del modelo
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    switch (axis)
    {
    case 0:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(1.0f, 0.0f, 0.0f));
        break;
    case 1:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 2:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    case 3:
        model = glm::rotate(model, glm::radians(angles), glm::vec3(1.0f, 1.0f, 1.0f));
        break;

    }


    _objShader.setMat4("model", model);

    //linkeamos nuestras texturas
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARDS, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
}


// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(yoffset);
}

//Funcion que carga texturas
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "fallamos al cargar las texturas del siguiente origen: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}
unsigned int loadCubeMap(vector<string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            cout << "Fallo en cargar las texturas del skybox " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_LINEAR);

    return textureID;

}