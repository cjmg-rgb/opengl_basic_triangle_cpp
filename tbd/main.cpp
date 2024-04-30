#include "config.h"

void initGLFW()
{
	glfwInit();
};

void bufferFrameCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

GLFWwindow* createWindow(int& width, int& height)
{

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	GLFWwindow* window = glfwCreateWindow(500, 500, "Transforms", NULL, NULL);
	glfwGetFramebufferSize(window, &width, &height);
	glfwSetFramebufferSizeCallback(window, bufferFrameCallback);

	return window;
}

unsigned int compileShader(unsigned int type, const std::string& fileDir)
{
	

	std::ifstream file;
	std::string temp = "";
	std::string src = "";
	
	file.open(fileDir);
	if (file.is_open())
		while (std::getline(file, temp))
			src += temp + "\n";

	unsigned int id = glCreateShader(type);
	const char* srcShader = src.c_str();
	glShaderSource(id, 1, &srcShader, NULL);
	glCompileShader(id);

	return id;
}

unsigned int createProgram(const unsigned int& vertex, const unsigned int& fragment)
{
	unsigned int programId = glCreateProgram();
	glAttachShader(programId, vertex);
	glAttachShader(programId, fragment);
	glLinkProgram(programId);

	return programId;
}

unsigned int createTexture(const unsigned int type, const char* fileDir)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fileDir, &width, &height, 0, SOIL_LOAD_RGBA);;

	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(type, id);


	glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(type);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	SOIL_free_image_data(image);
	glBindTexture(type, 0);

	return id;
}

void modelMatrixFunc(glm::mat4& ModelMatrix, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
	ModelMatrix = glm::translate(ModelMatrix, pos);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, scale);
}

void updateInput(GLFWwindow* window, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
	if (glfwGetKey(window, GLFW_KEY_W)) pos.z += -.1f;
	else if (glfwGetKey(window, GLFW_KEY_S)) pos.z += .1f;
	else if (glfwGetKey(window, GLFW_KEY_A)) pos.x += -.1f;
	else if (glfwGetKey(window, GLFW_KEY_D)) pos.x += .1f;
	else if (glfwGetKey(window, GLFW_KEY_F)) pos.y += .1f;
	else if (glfwGetKey(window, GLFW_KEY_G)) pos.y += -.1f;
	else if (glfwGetKey(window, GLFW_KEY_Q)) rot.z +=  1.f;
	else if (glfwGetKey(window, GLFW_KEY_E)) rot.z += -1.f;
	else if (glfwGetKey(window, GLFW_KEY_N)) rot.y += -1.f;
	else if (glfwGetKey(window, GLFW_KEY_M)) rot.y +=  1.f;
}

void initGLEW()
{
	glewExperimental = GL_TRUE;
	glewInit();
}

int main()
{
	// GLFW
	initGLFW();
	
	// Window
	int win_width, win_height;
	GLFWwindow* window = createWindow(win_width, win_height);
	glfwMakeContextCurrent(window);

	// GLEW
	initGLEW();

	// Compile Shader
	unsigned int vertex = compileShader(GL_VERTEX_SHADER, "vertex_core.glsl");
	unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, "fragment_core.glsl");

	// Create Shader
	unsigned int program = createProgram(vertex, fragment);

	// Buffer
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Texture
	unsigned int texture0 = createTexture(GL_TEXTURE_2D, "images/luffy.png");

	// Uniform
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);
	glm::mat4 ModelMatrix(1.f);

	glm::mat4 ViewMatrix(1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::vec3 camPosition(0.f, 0.f, 1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
	
	glm::mat4 ProjectionMatrix(1.f);
	float fov = 90.f;
	float nearPlane = .1f;
	float farPlane = 1000.f;
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(win_width) / win_height, nearPlane, farPlane);

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		
		// Update Inputs
		updateInput(window, position, rotation, scale);

		// Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Update Model Matrix
		ModelMatrix = glm::mat4(1.f);
		modelMatrixFunc(ModelMatrix, position, rotation, scale);

		// update Projection Matrix
		glViewport(0, 0, win_width, win_height);
		ProjectionMatrix = glm::perspective(fov, static_cast<float>(win_width) / win_height, nearPlane, farPlane);


		// Program
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		
		// Texure
		glBindTextureUnit(0, texture0);
		
		// Vertex
		glBindVertexArray(VAO);

		// Draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Chaning buffers and watching for events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(program);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}