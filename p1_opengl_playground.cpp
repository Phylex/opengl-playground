#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define PROJECT_NAME "Opengl-playground part 1"

int main(int argc, char **argv) {
    if(argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }
    printf("This is project %s.\n", PROJECT_NAME);
	glewExperimental = true;

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Part 1", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// the documentation specifies that glew can only be initialized after a valid
	// rendering context has been enabled
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW initialisation failed!" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	glfwTerminate();
    return 0;
}
