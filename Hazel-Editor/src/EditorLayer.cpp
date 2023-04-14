#include "EditorLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

namespace Hazel
{
	EditorLayer::EditorLayer() :Layer("EditorLayer"), m_CameraController(1920.f / 1080.f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		Layer::OnAttach();

		m_BackgroundTexture = Texture2D::Create("assets/textures/batthern.png");

		m_CameraController.SetZoomLevel(5.0f);

		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();

		Layer::OnDetach();
	}

	void EditorLayer::OnUpdate(Timestep timeStep)
	{
		HZ_PROFILE_FUNCTION();

		Layer::OnUpdate(timeStep);

		// Update
		m_CameraController.OnUpdate(timeStep);


		// Renderer
		Renderer2D::ResetStarts();

		{
			HZ_PROFILE_SCOPE("Renderer Prep");
			m_FrameBuffer->Bind();

			RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
			RenderCommand::Clear();
		}

		{
			static float rotation = 0.0f;
			rotation += timeStep * 50.0f;

			HZ_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());

			Renderer2D::DrawQuad({ 0.5f, -0.5f , -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			Renderer2D::DrawQuad({ -0.5f, 0.1f }, { 0.75f, 0.35f }, m_SquareColor);
			Renderer2D::DrawQuad({ -0.0f, -0.0f, -0.7f }, { 10.5f, 10.5f }, m_BackgroundTexture, 10.0f);
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 0.8f, 0.8f }, m_BackgroundTexture);

			Renderer2D::DrawQuad({ -2.5f, 0.0f, 0.8f }, { 1.5f, 1.5f }, rotation, m_BackgroundTexture, 10.0f);

			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.23f, (y + 5.0f) / 10.0f, 1.0f };
					Renderer2D::DrawQuad({ x, y }, { 0.49f, 0.49f }, color);
				}
			}
			Renderer2D::EndScene();
			m_FrameBuffer->UnBind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		Layer::OnImGuiRender();

		HZ_PROFILE_FUNCTION();

		static bool dockingEnabled = true;

		if (dockingEnabled)
		{
			static bool p_open = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &p_open, window_flags);
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit"))
					{
						Application::Get().Close();
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");
			ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor));

			ImGui::NewLine();

			auto stats = Renderer2D::GetStarts();
			ImGui::Text("Renderer2D Starts:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("QuadCounts: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			const uint32_t textureID = m_FrameBuffer->GetColorAttachmentRenderer2D();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 1280, 720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();

			ImGui::End();
		}
		else
		{
			ImGui::Begin("Settings");

			ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor));

			ImGui::NewLine();

			auto stats = Renderer2D::GetStarts();
			ImGui::Text("Renderer2D Starts:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("QuadCounts: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			const uint32_t textureID = m_BackgroundTexture->GetRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 1280, 720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();
		}


	}

	void EditorLayer::OnEvent(Event& event)
	{
		Layer::OnEvent(event);

		m_CameraController.OnEvent(event);
	}
}


