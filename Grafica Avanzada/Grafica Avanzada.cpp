// Grafica Avanzada.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Medidas de pantalla
const unsigned int largo = 800;
const unsigned int ancho = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 1.5f, 0.5f, 1.0f);\n"
"}\n\0";


int main()
{
	//Incializamos gflw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creacion de nuestra ventana
	GLFWwindow* window = glfwCreateWindow(largo, ancho,"AAAAAAAAAAAA", NULL,NULL);
	if (window == NULL) 
	{
		std::cout << "F"<<std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "No se ejecuto glad" << std::endl;
	}

	//inicializaciones 
	//inicializacion del vertex
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//comprobacion del vertex
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "error error erro la cagaste en el vertex\n" << infoLog << std::endl;
	}
	//inicializacion el fragment
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//comprobacion del fragment
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "error error erro la cagaste en el fragment\n" << infoLog << std::endl;
	}
	//inicializamos el programa
		//linkeamos todo
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//comprobar los linkeos
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "error error erro la cagaste en todo el pinchi programa\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[]{
		0.0f, 0.0f, 0.0f, //izquierda
		1.0f, 0.0f, 0.0f,  //derecha
		0.0f, 1.0f, 0.0f,    //arriba
	};

	float vertices1[]{
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	float vertices2[]{
		-1.0f, 0.0f, 0.0f, //izquierda
		0.0f, 0.0f, 0.0f,  //derecha
		-1.0f, 1.0f, 0.0f,    //arriba
	};

	float vertices3[]{
		-1.0f, -0.2f, 0.0f, //izquierda
		0.0f, -0.2f, 0.0f,  //derecha
		-0.5f, 0.0f, 0.0f,    //arriba
		-1.0f, -0.2f, 0.0f, //izquierda
		0.0f, -0.2f, 0.0f,  //derecha
		-1.0f, -0.8f, 0.0f,    //arriba
		0.0f, -0.2f, 0.0f, //izquierda
		0.0f, -0.8f, 0.0f,  //derecha
		-1.f, -0.8f, 0.0f,    //arriba
		-1.0f, -0.8f, 0.0f, //izquierda
		0.0f, -0.8f, 0.0f,  //derecha
		-0.5f, -1.0f, 0.0f,    //arriba
		
		0.0f, -1.0f, 0.0f, //izquierda
		1.0f, -1.0f, 0.0f,  //derecha
		0.5f, -0.5f, 0.0f,    //arriba

		0.25f, -0.5f, 0.0f, //izquierda
		0.5f, -0.5f, 0.0f,  //derecha
		0.25f, 0.0f, 0.0f,    //arriba
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//Unificamos
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	//Habilitar nuestros atributos de nuestras coordenadas
	glEnableVertexAttribArray(0);
	//Unificacion de los buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int VBO1, VAO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	//Unificamos
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Habilitar nuestros atributos de nuestras coordenadas
	glEnableVertexAttribArray(0);
	//Unificacion de los buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int VBO2, VAO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	//Unificamos
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Habilitar nuestros atributos de nuestras coordenadas
	glEnableVertexAttribArray(0);
	//Unificacion de los buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	unsigned int VBO3, VAO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	//Unificamos
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Habilitar nuestros atributos de nuestras coordenadas
	glEnableVertexAttribArray(0);
	//Unificacion de los buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Generacio del ciclo de dibujo
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);
		//Color
		glClearColor(0.1f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Dibujar
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO3);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) 
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
void framebuffer_size_callback(GLFWwindow*window,int width,int height)
{
	glViewport(0,0,width,height);
}
// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
