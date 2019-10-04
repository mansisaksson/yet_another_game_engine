//#include <GLFW/glfw3.h>
//
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

#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/twindow.h"

int main()
{
	TavlaApplication Application
	{
		Tavla::CreateSlot()
		->SetProperty(&Tavla::Slot::Width, 10)
		->SetContent(nullptr)
		
	};

    return Application.Run();

	return 0;
}