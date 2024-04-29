#include "libs.h"

static void resizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

static GLFWwindow* createWindow()
{

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	GLFWwindow* window = glfwCreateWindow(500, 500, "Hello World", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	return window;
}

static unsigned int compileShader(const unsigned int type, const std::string& file_name)
{
	unsigned int id = glCreateShader(type);
	
	std::string src = "";
	std::string temp = "";

	std::ifstream file;
	file.open(file_name);
	if (file.is_open())
		while (std::getline(file, temp))
			src += temp + "\n";

	const char* shaderSrc = src.c_str();
	glShaderSource(id, 1, &shaderSrc, NULL);
	glCompileShader(id);

	return id;
}

static unsigned int creaeteShader(unsigned int vertex, unsigned int fragment)
{
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	return program;
}

static unsigned int createTexture(const unsigned int type, const char* file_name)
{
	int img_width, img_height;
	unsigned char* image = SOIL_load_image(file_name, &img_width, &img_height, nullptr, SOIL_LOAD_RGBA);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	glBindTexture(type, texture0);

	glTextureParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(type, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(type);

	
	SOIL_free_image_data(image);
	glBindTexture(type, 0);

	return texture0;
}

static unsigned int createBufferObjects(const float vertices[], int verticesSize, const int indices[], int indicesSize, const int nOfAttrib, const int sequence[])
{
	std::cout << verticesSize << " " << indicesSize << "\n";
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
	
	int offset = 0;
	for (int i = 0; i < nOfAttrib; i++)
	{
		glVertexAttribPointer(i, sequence[i], GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offset * sizeof(float)));
		offset += sequence[i];
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(0);

	return VAO;
}

int main()
{

	// GLFW
	glfwInit();

	// Window
	GLFWwindow* window = createWindow();
	glfwMakeContextCurrent(window);

	// GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Shader
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, "vertex_core.glsl");
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, "fragment_core.glsl");
	unsigned int fragmentShader2 = compileShader(GL_FRAGMENT_SHADER, "fragment2_core.glsl");
	
	unsigned int program = creaeteShader(vertexShader, fragmentShader);
	unsigned int program2 = creaeteShader(vertexShader, fragmentShader2);

	// Buffer Objects
	int sequence[] = {3, 3, 2};
	int wholeSizeOfVertices = sizeof(vertices);
	int wholeSizeOfIndices = sizeof(indices);
	unsigned int VAO = createBufferObjects(vertices, wholeSizeOfVertices, indices, wholeSizeOfIndices, 3, sequence);
	unsigned int VAO2 = createBufferObjects(vertices2, wholeSizeOfVertices, indices, wholeSizeOfIndices, 3, sequence);

	// Textures
	unsigned int texture0 = createTexture(GL_TEXTURE_2D, "images/luffy.png");
	unsigned int texture1 = createTexture(GL_TEXTURE_2D, "images/meat.png");

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Texture
		glBindTextureUnit(0, texture0);

		glUniform1i(glGetUniformLocation(program2, "texture1"), 1);
		glBindTextureUnit(1, texture1);

		// Draw
		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeOfIndices, GL_UNSIGNED_INT, 0);
		
		glUseProgram(program2);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, sizeOfIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &VAO);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glfwTerminate();

	return 0;
}