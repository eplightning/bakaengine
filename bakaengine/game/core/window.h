#ifndef GAME_CORE_WINDOW_H_
#define GAME_CORE_WINDOW_H_

#include "bakaglobal.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Baka
{
	struct WindowSize {
		int width;
		int height;
	};

	struct FramebufferSize {
		int width;
		int height;
	};

	class Window {
	public:
		Window(int width, int height, bool fullscreen, bool vsync, const char *title);
		~Window();

		FramebufferSize GetFbSize() const;
		bool GetStatus() const;
		GLFWwindow* GetWindow() const;

		static void ErrorCallback(int code, const char *description);
		static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
		static void WindowSizeCallback(GLFWwindow *window, int width, int height);

	private:
		GLFWwindow *wnd_;
		bool status_;
	};
} // namespace Baka

#endif