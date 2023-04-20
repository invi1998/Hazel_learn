#include "hzpch.h"

#include <GLFW/glfw3.h>

#include "Hazel/Core//Application.h"
#include "Hazel/Core/Input.h"

namespace Hazel
{
	// 按键是否按下判断
	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto [x,y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x = 0;
		double y = 0;
		glfwGetCursorPos(window, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}
}
