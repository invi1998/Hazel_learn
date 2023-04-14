#pragma once

#include "Hazel/Event/Event.h"
#include "Window.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Core/LayStack.h"

namespace Hazel
{
	class VertexArray;
	class IndexBuffer;
	class VertexBuffer;
	class ImGuiLayer;

	class Application
	{
	public:
		Application(const std::string& name = "Hazel APP");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverLayer(Layer* layer);

		void Close();

		inline Window& GetWindow() const { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		LayStack m_LayStack;

		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// 在客户端定义
	Application* CreateApplication();
}


