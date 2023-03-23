#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
#include "Hazel/LayStack.h"
#include "Hazel/Renderer/Shader.h"
#include "Renderer/OrthographicCamera.h"

namespace Hazel
{
	class VertexArray;
	class IndexBuffer;
	class VertexBuffer;
	class ImGuiLayer;

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverLayer(Layer* layer);

		inline Window& GetWindow() const { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

		LayStack m_LayStack;

		ImGuiLayer* m_ImGuiLayer;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;

	private:
		static Application* s_Instance;
	};

	// �ڿͻ��˶���
	Application* CreateApplication();
}


