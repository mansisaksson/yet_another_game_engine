#include "glfwplatform.h"
#include "glfwinput.h"
#include "glfwwindow.h"

//#include <stdlib.h>
//
//void shut_down(int code)
//{
//	glfwTerminate();
//	exit(code);
//}
//
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//}
//
//int main()
//{
//	glfwInit();
//	if (!glfwInit())
//	{
//		shut_down(1);
//	}
//
//	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
//	if (!window)
//	{
//		shut_down(1);
//	}
//
//	glfwMakeContextCurrent(window);
//
//	while (!glfwWindowShouldClose(window))
//	{
//		// Keep running
//		glfwPollEvents();
//	}
//
//    return 0;
//}

glfw_platform::glfw_platform()
{
	glfwInit();
	if (!glfwInit())
	{
		// TODO: error
	}
}

glfw_platform::~glfw_platform()
{
	glfwTerminate();
}

std::shared_ptr<platform_window> glfw_platform::create_platform_window()
{
	return std::make_shared<glfw_window>();
}

std::shared_ptr<platform_input> glfw_platform::create_platform_input()
{
	return std::make_shared<glfw_input>();
}