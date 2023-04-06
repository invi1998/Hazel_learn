#include "hzpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/gl.h>
#include <GL/GL.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(m_WindowHandle, "Window Handle is Null!");
	}

	void OpenGLContext::Init()
	{
		HZ_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		// int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		int status = gladLoaderLoadGL();
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		/*HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));*/

		std::cout << "OpenGL Info:" << std::endl;
		std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;

#ifdef HZ_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;

		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		HZ_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");

#endif

		// HZ_CORE_INFO("OpenGL Renderer: {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	}

	void OpenGLContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
