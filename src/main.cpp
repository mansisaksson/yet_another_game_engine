#include <GLFW/glfw3.h>

int exit(int ErrorCode)
{
	glfwTerminate();
	return 1;
}

int main()
{
	glfwInit();
	if (!glfwInit())
	{
		return exit(1);
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
	{
		return exit(1);
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		// Keep running
	}

    return 0;
}