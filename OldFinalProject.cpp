#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_DLL
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include <iostream>

#include "cylinder.h"
#include "Sphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



// camera
Camera camera;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
bool perspective = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;




// projection matrix
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);






int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CS 330 Final Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	//This shader is the shader that is proven to work. This shows textures, ambient lighting, and positions of objects.
	Shader ourShader("shaderfiles/7.3.camera.vs", "shaderfiles/7.3.camera.fs");

	//Shaders for the lights
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");



	//This is repeated, where this causes a redefinition error. 
	//Initialize positions of point lights 
	// positions of the point lights
	/*glm::vec3 pointLightPositions[] = {
		glm::vec3(0.8f,  2.8f,  -1.2f),
		glm::vec3(2.0f, 0.5f, 1.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};*/

	//// cup handle cube's VAO and VBO ?
	//unsigned int cubeVBO, cubeVAO;

	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &cubeVBO);

	//glBindVertexArray(cubeVAO);
	//// position attribute for cube
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// texture attribute for cube
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// normal attribute for cube
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	//glEnableVertexAttribArray(2);


	//// position attribute for cube
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// texture attribute for cube
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//// normal attribute for cube
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//// lightCube's VAO and VBO
	//unsigned int lightCubeVAO;
	//glGenVertexArrays(1, &lightCubeVAO);
	//glBindVertexArray(lightCubeVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	//// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Mesh                  //Normals           //Texture Coords
		-0.5f, -0.5f, -0.5f,                          0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,                          1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,                          1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,                          1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,                          0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,                          0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,                          0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,                          1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,                          1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,                          1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,                          0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,                          0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,                          1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,                          1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,                          0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,                          0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,                          0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,                          1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,                          1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,                          1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,                          0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,                          0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,                          0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,                          1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,                          0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,                          1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,                          1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,                          1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,                          0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,                          0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,                          0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,                          1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,                          1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,                          1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,                          0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,                          0.0f, 1.0f


		
	};



	//These are the point light positions that are going to be used for the scene.
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.8f,  2.8f,  -1.2f),
		glm::vec3(2.0f, 0.5f, 1.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

		

		 
	//These are the VBOs and VAOs for the objects within the scene 
		
	unsigned int VBO, VAO;
	unsigned int VBO2, VAO2;
	

	//This is for the cube
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//This is for the cylinder and sphere
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);


	// cup handle cube's VAO and VBO ?
	unsigned int cubeVBO, cubeVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	// position attribute for cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute for cube
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute for cube
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// position attribute for cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute for cube
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute for cube
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// lightCube's VAO and VBO
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	//These blocks initialize the textures being used for the scene
	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("images/Ocean2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("images/egg.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 3
// ---------
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data2 = stbi_load("images/Ball2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	//texture 4
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data3 = stbi_load("images/Eevee Collar 1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);


	//texture 5
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data4 = stbi_load("images/GlueStick2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data4);

	//texture 6 painting 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data5 = stbi_load("images/Mudkip3.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data5);

	//texture 7 Eevee's head
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data6 = stbi_load("images/Eevee4.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data6);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data6);


	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);
	ourShader.setInt("texture4", 3);
	ourShader.setInt("texture5", 4);
	ourShader.setInt("texture6", 5);
	ourShader.setInt("texture7", 6);


	//lighting sampler
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0); //plane
	lightingShader.setInt("material.specular", 1);  //egg
	lightingShader.setInt("material.specular", 2); //ball
	lightingShader.setInt("material.specular", 3);  //collar
	lightingShader.setInt("material.specular", 4); //glue stick
	lightingShader.setInt("material.specular", 5); //painting
	lightingShader.setInt("material.specular", 6); //head


	glm::mat4 model;
	float angle;

	//Sphere
	Sphere S(1, 30, 30);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);


		// render
		// ------
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//This renders the lighting within the scene
		//lightingShader
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setVec3("lightPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		// directional light
		lightingShader.setVec3("dirLight.direction", 2.5f, 0.0f, 0.0f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// key light
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);

		// fill light
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.7f, 0.7f, 0.7f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);

		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);

		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);

		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 0.7f, 0.7f, 0.7f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//Bind more textures?
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture7);



		// activate shader

		ourShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		//glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
		ourShader.setMat4("projection", projection);
		//lightingShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);
		//lightingShader.setMat4("view", view);


		// world transformation
		//glm::mat4 model = glm::mat4(1.0f);
		//lightingShader.setMat4("model", model);

		// render boxes
		//This is for the  first cube foot for Eevee
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-12.0f, -3.0f, -6.3f)); //cube
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//This is for Eevee's second foot
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-10.0f, -3.0f, -6.3f)); //cube
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//This is for Eevee's third foot
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-12.0f, -3.0f, -3.0f)); //cube
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);




		//This is for Eevee's fourth foot
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-10.0f, -3.0f, -3.0f)); //cube
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);





		//This is for Eevee's Tail
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, 0.0f, -7.0f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, 1.0f, -7.3f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);






		//This is for Eevee's Ears
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-12.5f, 3.0f, -1.0f)); //cube
		model = glm::scale(model, glm::vec3(0.6f, 3.0f, 0.5f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(-5.0f, 5.0f, 5.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-9.5f, 3.0f, -1.0f)); //cube
		model = glm::scale(model, glm::vec3(0.6f, 3.0f, 0.5f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(-5.0f, 5.0f, 5.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 6);







		//This is for Eevee's Body
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, -1.0f, -6.0f)); //cube
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, -1.0f, -4.0f)); //cube
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);







		//This is for Eevee's Head
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture7);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, 1.0f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//side 1 of Head
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, 1.5f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//side 2 of Head
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-10.5f, 1.0f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//side 3 of Head
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.5f, 1.0f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//This is for Eevee's Collar

		//front
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-11.0f, -1.5f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//side 1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-12.0f, -1.0f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//side 2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-10.0f, -1.0f, -2.0f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//back 1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-10.0f, -1.0f, -4.0f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//back 2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-12.0f, -1.0f, -4.0f)); //cube
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);







		//This is for the plane
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-2.0f, -5.0f, -3.0f)); //cube

		//Make plane big
		model = glm::scale(model, glm::vec3(50.0f, 1.0f, 50.0f));

		//rotate plane
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//angle += 0.2;
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);
		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 36);




		//This is for the painting
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glBindVertexArray(VAO);
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(5.0f, 1.0f, 0.0f)); //painting translation

		//Make painting bigger
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 0.5f));

		//rotate plane
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);
		//36 triangles for one cube
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//This is the cylinder
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 8.0f));
		//make the cylinder smaller
		model = glm::scale(model, glm::vec3(0.5f, 10.0f, 0.5f));

		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder C(2, 10, 1, true, true, true);
		C.render();

		//This is the sphere
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO2);
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first		
		model = glm::translate(model, glm::vec3(-10.0f, -3.0f, 1.0f));
		ourShader.setMat4("model", model);
		//lightingShader.setMat4("model", model);

		S.Draw();



		// activate shader for drawing light cube
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		//originally i < 1
		for (unsigned int i = 0; i < 1; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.4f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);

	
	

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;

	
		



	// change view between perspective and orthographics
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {  //P allows you to switch between perspective and ortho
		if (perspective) {
			projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
			perspective = false;
		}
		else {
			//camera.Zoom for glm::radians?
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			perspective = true;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
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
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
