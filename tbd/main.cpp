#include "config.h"

// Transform Control
void modelMatrixControl(glm::mat4& model, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, scale);
}

// Buffer Frame Size Callback
void frameBufferSizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

// Initialize GLFW
void initializeGLFW()
{
	if (!glfwInit())
		std::cout << "ERROR::INITIALIZEGLFW::FAILED\n";
}

// Create Window
GLFWwindow* createWindow(int& width, int& height)
{
	GLFWwindow* window;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	window = glfwCreateWindow(500, 500, "Walang Internet", NULL, NULL);
	glfwGetFramebufferSize(window, &width, &height);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);


	return window;

}

// Initialize GLEW
void initializeGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::INITIALIZEGLEW::FAILED\n";
		glfwTerminate();
	}

}

// Compile Shader
unsigned int compileShader(const unsigned int type, const std::string& fileDir)
{	

	unsigned int id;
	const char* shaderSrc;

	int success;
	char infoLog[512];
	std::string temp = "";
	std::string src = "";
	std::ifstream file;

	file.open(fileDir);
	if (!file.is_open())
	{
		std::cout << "ERROR::COMPILESHADER::OPENFILE::FAILED\n";
		glfwTerminate();
	};
	
	while (std::getline(file, temp))
		src += temp + "\n";

	id = glCreateShader(type);
	shaderSrc = src.c_str();
	glShaderSource(id, 1, &shaderSrc, NULL);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::COMPILESHADER::FAILED::" + fileDir + "\n";
		std::cout << infoLog << "\n";
		glfwTerminate();
	};

	file.close();

	return id;
}

// Create Shader Program
unsigned int createShader(const unsigned int vertex, const unsigned int fragment)
{
	int success;
	char infoLog[512];
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::CREATESHADER::FAILED\n";
		std::cout << infoLog << "\n";
		glfwTerminate();
	}

	return program;
}

// Textures
unsigned int createTexture(const unsigned int type, const std::string& fileDir)
{
	unsigned int texture0;

	int w, h;
	const char* imgDir = fileDir.c_str();
	unsigned char* image = SOIL_load_image(imgDir, &w, &h, 0, SOIL_LOAD_RGBA);
	
	if (!image)
	{
		std::cout << "ERROR::CREATETEXTURE::OPENFILE::FAILED\n";
		glfwTerminate();
		return -1;
	}

	glGenTextures(1, &texture0);

	glBindTexture(type, texture0);
	glTexImage2D(type, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(type);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	SOIL_free_image_data(image);

	return texture0;
}

// Inputs
void updateInputs(GLFWwindow* window, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
{
	float pos_s = 0.1f;
	float rot_s = 1.0f;
	float scl_s = 0.1f;

	if (glfwGetKey(window, GLFW_KEY_W)) pos.z -= pos_s;
	else if (glfwGetKey(window, GLFW_KEY_S)) pos.z += pos_s;
	else if (glfwGetKey(window, GLFW_KEY_A)) pos.x -= pos_s;
	else if (glfwGetKey(window, GLFW_KEY_D)) pos.x += pos_s;
	else if (glfwGetKey(window, GLFW_KEY_Q)) rot.z += rot_s;
	else if (glfwGetKey(window, GLFW_KEY_E)) rot.z -= rot_s;
	else if (glfwGetKey(window, GLFW_KEY_N)) rot.y -= rot_s;
	else if (glfwGetKey(window, GLFW_KEY_M)) rot.y += rot_s;
}


int main()
{
	GLFWwindow* window;
	int winWidth, winHeight;

	unsigned int vertex, fragment, program, VAO, VBO, EBO, texture0;
	float fov, nearPlane, farPlane;

	// GLFW
	initializeGLFW();

	// Window
	window = createWindow(winWidth, winHeight);
	glfwMakeContextCurrent(window);

	// GLEW
	initializeGLEW();

	// Compile Shaders
	vertex = compileShader(GL_VERTEX_SHADER, "vertex_core.glsl");
	fragment = compileShader(GL_FRAGMENT_SHADER, "fragment_core.glsl");

	// Create Program
	program = createShader(vertex, fragment);

	// Buffer Objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	// Texture
	texture0 = createTexture(GL_TEXTURE_2D, "images/luffy.png");

	// Lights
	glm::vec3 lightPos0(0.f, 0.f, -2.f);

	// Uniform
	glm::mat4 ModelMatrix(1.f);
	glm::vec3 pos(0.f);
	glm::vec3 rot(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ViewMatrix(1.f);
	glm::vec3 camPos(0.f, 0.f, 1.f);
	glm::vec3 camFront(0.f, 0.f, -.1f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	ViewMatrix = glm::lookAt(camPos, camPos + camFront, worldUp);

	glm::mat4 ProjectionMatrix(1.f);
	fov = 90.f;
	nearPlane = 0.1f;
	farPlane = 1000.f;

	glUseProgram(program);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUniform3fv(glGetUniformLocation(program, "lightPos0"), 1, glm::value_ptr(lightPos0));


	glUseProgram(0);

	// Loop
	while (!glfwWindowShouldClose(window))
	{
		// Inputs
		updateInputs(window, pos, rot, scale);

		// Clear
		glClearColor(.0f, .0f, .0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Model Matrix Updates
		ModelMatrix = glm::mat4(1.f);
		modelMatrixControl(ModelMatrix, pos, rot, scale);

		// Projection Matrix Updates
		glfwGetFramebufferSize(window, &winWidth, &winHeight);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(winWidth) / winHeight, nearPlane, farPlane);


		// Programs and Uniform activation
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		// Texture
		glBindTextureUnit(0, texture0);

		// Vertex Buffers
		glBindVertexArray(VAO);

		// Draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap Buffers and watch for events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}