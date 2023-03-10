#include "hzpch.h"

#include <glad/gl.h>

#include "Application.h"

#include "GLFW/glfw3.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Log.h"
#include "Hazel/Input.h"

namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		HZ_INFO(e);

		while (true)
		{
			while (m_Running)
			{
				glClearColor(.1, .1, .1, 1);
				glClear(GL_COLOR_BUFFER_BIT);

				for (Layer* layer : m_LayStack)
				{
					layer->OnUpdate();
				}

				m_Window->OnUpdate();
			}
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayStack.end(); it != m_LayStack.begin();)
		{
			(*--it)->OnEvent(e);
			if(e.Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayStack.PushLayer(layer);
		layer->OnDetach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		m_LayStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}

