#include "hzpch.h"
#include "Application.h"

#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel
{
	Application::Application()
	{}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		HZ_INFO(e);

		while (true);
	}
}

