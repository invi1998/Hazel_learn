#include "hzpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/gl.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_WindowHandle, "Window Handle is Null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		const int status = gladLoadGL(glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
