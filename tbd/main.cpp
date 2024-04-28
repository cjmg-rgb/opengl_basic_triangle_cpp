#include "libs.h"

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

GLuint compile_shader(const GLenum& shader_type, const char* shader_source)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	return shader;
}

GLuint create_program(GLuint& vertex_shader, GLuint& fragment_shader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	return program;
}

void bind_buffer_object(GLuint& VAO, float data[], int size)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);

	std::cout << sizeof(data) << " " << data << "\n";

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

int main()
{

	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	
	GLFWwindow* window = glfwCreateWindow(500, 500, "Tringles", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize_callback);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, for_vertex_shader);
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, for_fragment_shader1);
	GLuint fragment_shader2 = compile_shader(GL_FRAGMENT_SHADER, for_fragment_shader2);
	GLuint program = create_program(vertex_shader, fragment_shader);
	GLuint program2 = create_program(vertex_shader, fragment_shader2);

	std::vector<GLuint> programs;
	programs.push_back(program);
	programs.push_back(program2);

	GLuint VAOs[2];
	glGenVertexArrays(2, VAOs);


	bind_buffer_object(VAOs[0], triangle1, sizeof(triangle1));
	bind_buffer_object(VAOs[1], triangle2, sizeof(triangle2));

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwTerminate();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);


		for (int i = 0; i < 2; i++)
		{
			glUseProgram(programs[i]);
			glBindVertexArray(VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glfwTerminate();

	return 0;
}