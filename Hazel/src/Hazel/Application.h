#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "Window.h"

namespace Hazel
{

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
	};

	// 在客户端定义
	Application* CreateApplication();
}


