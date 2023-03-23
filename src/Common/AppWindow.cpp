#include "FracturePCH.h"
#include "AppWindow.h"

Fracture::AppWindow::AppWindow(const WindowCreationInfo& info):Info(info)
{

}

Fracture::AppWindow::~AppWindow()
{
	glfwTerminate();
}

void Fracture::AppWindow::Init()
{
	glfwSetFramebufferSizeCallback(Context, Fracture::AppWindow::OnWindowResize);
	glfwSetWindowIconifyCallback(Context, OnMinimised);
}

void Fracture::AppWindow::SwapBuffers()
{
	glfwSwapBuffers(Context);

}

void Fracture::AppWindow::PollEvents(bool waitforInput)
{
	if (waitforInput)
		glfwWaitEvents();
	else
		glfwPollEvents();
}

bool Fracture::AppWindow::ShouldWindowClose()
{
 return glfwWindowShouldClose(Context);
}

void Fracture::AppWindow::SetWindowTitle(const std::string& title)
{
	glfwSetWindowTitle(Context, title.c_str());
}

void Fracture::AppWindow::OnWindowResize(GLFWwindow* window, int width, int height)
{
	///TODO Dispatch Window Resize events
	glViewport(0, 0, width, height);
}

void Fracture::AppWindow::OnMinimised(GLFWwindow* window, int iconified)
{
	if (iconified)
	{
		// The window was iconified
	}
	else
	{
		// The window was restored
	}
}
