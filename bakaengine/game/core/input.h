#ifndef GAME_CORE_INPUT_H
#define GAME_CORE_INPUT_H

#include "bakaglobal.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config/config.h"
#include "game/core/window.h"

namespace Baka
{
	enum class InputMouseMode {
		NormalNormalized = 0,
		NormalAbsolute,
		Locked
	};

	struct InputMouseCoords {
		double x;
		double y;
	};

	class Input {
	public:
		Input(GLFWwindow *window);

		void SetMouseMode(InputMouseMode mode);
		InputMouseCoords GetCurrentCoords() const;

		// binding
		const String& GetBinding(const String &key) const;
		void SetDefaultBinding(const ConfigKvMap &map);
		void SetBinding(const ConfigKvMap &map);
		void SetBinding(const String &key, const String &action);

		// event handlers
		void OnKeyPressed(int key, int scancode, int action, int mods);
		void OnMouseButtonPressed(int button, int action, int mods);
		void OnMouseMove(double xpos, double ypos);
		void OnWindowResize(void *param);

		// GLFW callbacks
		static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);

	private:
		static const char *glfw_key_map_[];
		static const char *glfw_mouse_map_[];

		InputMouseMode mode_;
		WindowSize window_size_;
		GLFWwindow *window_;
		ConfigKvMap current_binding_;
		ConfigKvMap default_binding_;

		String empty_binding_;
	};
}

#endif