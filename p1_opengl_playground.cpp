#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define PROJECT_NAME "Opengl-playground"

int main(int argc, char **argv) {
    if(argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }
    printf("This is project %s.\n", PROJECT_NAME);
	GLFWwindow* window;


	if (!glfwInit())
		return -1;
	
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// the documentation specifies that glew can only be initialized after a valid
	// rendering context has been enabled
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW initialisation failed!" << std::endl;
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glVertex2f(-.5f, -.5f);
		glVertex2f(-.0f, .5f);
		glVertex2f(.5f, -.5f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
    return 0;
}
