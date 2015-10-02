#include "game/core/input.h"

#include "bakaglobal.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game/core/registry.h"
#include "game/event/eventmanager.h"
#include "game/event/eventdispatcherstatic.h"
#include "game/core/window.h"
#include "logging/logger.h"

using namespace Baka;

const char *Input::glfw_mouse_map_[] = {
	"MOUSE_BTN1", "MOUSE_BTN2", "MOUSE_BTN3", "MOUSE_BTN4", "MOUSE_BTN5", "MOUSE_BTN6", "MOUSE_BTN7", "MOUSE_BTN8"
};

const char *Input::glfw_key_map_[] = {
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"SPACE", "", "", "", "", "", "", "APOSTROPHE", "", "", "", "", "COMMA", "MINUS", "PERIOD", "SLASH",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "SEMICOLON", "", "EQUAL", "", "", "",
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"LEFT_BRACKET", "BACKSLASH", "RIGHT_BRACKET", "", "", "GRAVE_ACCENT", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"WORLD_1", "WORLD_2", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"ESCAPE", "ENTER", "TAB", "BACKSPACE", "INSERT", "DELETE", "RIGHT", "LEFT", "DOWN", "UP",
	"PAGE_UP", "PAGE_DOWN", "HOME", "END", "", "", "", "", "", "", "", "", "", "",
	"CAPS_LOCK", "SCROLL_LOCK", "NUM_LOCK", "PRINT_SCREEN", "PAUSE", "", "", "", "", "",
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13",
	"F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "F25",
	"", "", "", "", "",
	"KP_0", "KP_1", "KP_2", "KP_3", "KP_4", "KP_5", "KP_6", "KP_7", "KP_8", "KP_9",
	"KP_DECIMAL", "KP_DIVIDE", "KP_MULTIPLY", "KP_SUBTRACT", "KP_ADD", "KP_ENTER", "KP_EQUAL",
	"", "", "",
	"LEFT_SHIFT", "LEFT_CONTROL", "LEFT_ALT", "LEFT_SUPER", "RIGHT_SHIFT", "RIGHT_CONTROL",
	"RIGHT_ALT", "RIGHT_SUPER", "MENU"
};

Input::Input(GLFWwindow *window) : window_(window), mode_(InputMouseMode::Locked), empty_binding_(), default_binding_(),
current_binding_()
{
	glfwGetWindowSize(window, &window_size_.width, &window_size_.height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, &Input::KeyCallback);
	glfwSetMouseButtonCallback(window, &Input::MouseButtonCallback);
	glfwSetCursorPosCallback(window, &Input::MouseMoveCallback);

	EventDispatcherStatic *dispatcher = Registry::Instance()->GetEventManager()->GetDispatcherStatic();
	dispatcher->Register("window.WindowResize", BIND_METHOD_CALL(Input::OnWindowResize, this));
}

void Input::SetMouseMode(InputMouseMode mode)
{
	mode_ = mode;

	switch (mode) {
		case InputMouseMode::Locked:
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;

		default:
			glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

InputMouseCoords Input::GetCurrentCoords() const
{
	InputMouseCoords out;

	glfwGetCursorPos(window_, &out.x, &out.y);

	return out;
}

const String& Input::GetBinding(const String &key) const
{
	auto it = current_binding_.find(key);

	if (it == current_binding_.end()) {
		auto it2 = default_binding_.find(key);

		if (it2 == default_binding_.end())
			return empty_binding_;

		return it2->second;
	}

	return it->second;
}

void Input::SetBinding(const ConfigKvMap &map)
{
	current_binding_ = map;
}

void Input::SetDefaultBinding(const ConfigKvMap &map)
{
	default_binding_ = map;
}

void Input::SetBinding(const String &key, const String &action)
{
	current_binding_[key] = action;
}

void Input::OnKeyPressed(int key, int scancode, int action, int mods)
{
	if (key < 0 || key >= std::extent<decltype(glfw_key_map_)>::value)
		return;

	const char *key_name = glfw_key_map_[key];
	const String &binding = GetBinding(key_name);

	if (!binding.empty()) {
		if (binding.front() != '+' && action == GLFW_RELEASE)
			return;

		Registry::Instance()->GetActions()->Execute(binding, (action == GLFW_PRESS));
	}
}

void Input::OnMouseButtonPressed(int button, int action, int mods)
{
	if (button < 0 || button >= std::extent<decltype(glfw_mouse_map_)>::value)
		return;

	const char *key_name = glfw_mouse_map_[button];
	const String &binding = GetBinding(key_name);

	if (!binding.empty()) {
		if (binding.front() != '+' && action == GLFW_RELEASE)
			return;

		Registry::Instance()->GetActions()->Execute(binding, (action == GLFW_PRESS));
	}
}

void Input::OnMouseMove(double xpos, double ypos)
{
	if (mode_ == InputMouseMode::NormalNormalized) {
		xpos /= window_size_.width;
		ypos /= window_size_.height;
	}

	InputMouseCoords coords;
	coords.x = xpos;
	coords.y = ypos;

	Registry::Instance()->GetEventManager()->Broadcast("input.MouseMove", &coords);
}

void Input::OnWindowResize(void *param)
{
	WindowSize *size = static_cast<WindowSize*>(param);

	window_size_.width = size->width;
	window_size_.height = size->height;
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		return;

	Registry::Instance()->GetInput()->OnKeyPressed(key, scancode, action, mods);
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	Registry::Instance()->GetInput()->OnMouseButtonPressed(button, action, mods);
}

void Input::MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
	Registry::Instance()->GetInput()->OnMouseMove(xpos, ypos);
}