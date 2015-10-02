#include "game/core/window.h"

#include "bakaglobal.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging/logger.h"
#include "game/core/registry.h"

using namespace Baka;

Window::Window(int width, int height, bool fullscreen, bool vsync, const char *title)
{
	status_ = (glfwInit() != GL_FALSE);

	if (!status_)
		return;

	if (fullscreen) {
		// TODO: Picking monitor for fullscreen would be niceee
		wnd_ = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
	} else {
		wnd_ = glfwCreateWindow(width, height, title, NULL, NULL);
	}

	glfwMakeContextCurrent(wnd_);

	if (vsync)
		glfwSwapInterval(1);

	int fb_w, fb_h;
	glfwGetFramebufferSize(wnd_, &fb_w, &fb_h);
	glViewport(0, 0, fb_w, fb_h);

	glfwSetErrorCallback(&Window::ErrorCallback);
	glfwSetFramebufferSizeCallback(wnd_, &Window::FramebufferSizeCallback);
	glfwSetWindowSizeCallback(wnd_, &Window::WindowSizeCallback);
}

Window::~Window()
{
	if (status_)
		glfwTerminate();
}

FramebufferSize Window::GetFbSize() const
{
	FramebufferSize size;

	glfwGetFramebufferSize(wnd_, &size.width, &size.height);

	return size;
}

bool Window::GetStatus() const
{
	return status_;
}

GLFWwindow* Window::GetWindow() const
{
	return wnd_;
}

void Window::ErrorCallback(int code, const char *description)
{
	LOG_INFO("GLFW Error (%d): %s", code, description);
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::WindowSizeCallback(GLFWwindow *window, int width, int height)
{
	WindowSize size;

	size.width = width;
	size.height = height;

	Registry::Instance()->GetEventManager()->Broadcast("window.WindowResize", &size);
}