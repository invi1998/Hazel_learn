#include "hzpch.h"

#include "Application.h"

#include "Hazel/Core/Timestep.h"
#include "GLFW/glfw3.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Core/Log.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverLayer(m_ImGuiLayer);
	}

	Application::~Application()
	{
		HZ_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		HZ_PROFILE_FUNCTION();

		while (m_Running)
		{
			HZ_PROFILE_SCOPE("RunLoop");

			// todo Platform::GetTime()
			float time = static_cast<float>(glfwGetTime());
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				HZ_PROFILE_SCOPE("LayerStack Onupdate");

				for (Layer* layer : m_LayStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				HZ_PROFILE_SCOPE("LayerStack OnImGuiRender")

				for (Layer* layer : m_LayStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

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
		HZ_PROFILE_FUNCTION();

		m_LayStack.PushLayer(layer);
		layer->OnDetach();
	}

	void Application::PushOverLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();

		m_LayStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}

