#include "EditorLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

namespace Hazel
{
	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1920.f / 1080.f, true)
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

		m_ActiveScene = std::make_shared<Scene>();

		// Entity
		m_SquaredEntity = m_ActiveScene->CreateEntity("Square");
		m_SquaredEntity.AddComponent<SpriteRendererComponent>(m_SquareColor);

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Clip-Space Entity");
		auto &cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto &transform = GetCompnent<TransformComponent>().Transform;
				transform[3][0] = rand() % 10 - 0.5f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto &transform = GetCompnent<TransformComponent>().Transform;
				float spead = 5.0f;

				if (Input::IsKeyPressed(Key::W))
					transform[3][1] += spead * ts;
				if (Input::IsKeyPressed(Key::S))
					transform[3][1] -= spead * ts;
				if (Input::IsKeyPressed(Key::A))
					transform[3][0] -= spead * ts;
				if (Input::IsKeyPressed(Key::D))
					transform[3][0] += spead * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
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

		// Resize
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		}

		// Update
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(timeStep);
		}

		// Renderer
		Renderer2D::ResetStarts();

		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
		RenderCommand::Clear();

		// update scene
		m_ActiveScene->OnUpdate(timeStep);

		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
		Layer::OnImGuiRender();

		HZ_PROFILE_FUNCTION();

		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport *viewport = ImGui::GetMainViewport();
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
		ImGuiIO &io = ImGui::GetIO();
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

		auto stats = Renderer2D::GetStarts();
		ImGui::Text("Renderer2D Starts:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("QuadCounts: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (m_SquaredEntity)
		{
			ImGui::Separator();
			const auto tag = m_SquaredEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor));
			m_SquaredEntity.GetComponent<SpriteRendererComponent>().Color = m_SquareColor;
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}

		{
			auto &camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
			{
				camera.SetOrthographicSize(orthoSize);
			}
		}

		ImGui::NewLine();

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("ViewPort");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvent(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *reinterpret_cast<glm::vec2 *>(&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		{
			m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}
		const uint32_t textureID = m_FrameBuffer->GetColorAttachmentRenderer2D();
		ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event &event)
	{
		Layer::OnEvent(event);

		m_CameraController.OnEvent(event);
	}
}
