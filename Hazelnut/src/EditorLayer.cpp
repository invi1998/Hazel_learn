#include "EditorLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include <imgui/imgui.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include "Hazel/Scene/SceneSerializer.h"

#include <chrono>

#include "Hazel/Utils/PlatformUtils.h"
#include "Hazel/Math/Math.h"

#include "ImGuizmo.h"

namespace Hazel
{
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1920.f / 1080.f, true), m_EditorCamera()
	{
		m_DropTargetTexture = Texture2D::Create("Resources/Icons/ContentBrowser/Drop.png");
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		Layer::OnAttach();

		m_BackgroundTexture = Texture2D::Create("assets/textures/batthern.png");
		m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");

		m_CameraController.SetZoomLevel(5.0f);

		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = {FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_ActiveScene = std::make_shared<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0

		// Entity
		auto square = m_ActiveScene->CreateEntity("Blue Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.1f, 0.23f, 0.88f, 1.0f });

		auto pinkSquare = m_ActiveScene->CreateEntity("Pink Square");
		pinkSquare.AddComponent<SpriteRendererComponent>(glm::vec4{0.95f, 0.11f, 0.45f, 1.0f});

		m_SquaredEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto &cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto &translation = GetCompnent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 0.5f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto & translation = GetCompnent<TransformComponent>().Translation;
				float spead = 5.0f;

				if (Input::IsKeyPressed(Key::W))
					translation.y += spead * ts;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= spead * ts;
				if (Input::IsKeyPressed(Key::A))
					translation.x -= spead * ts;
				if (Input::IsKeyPressed(Key::D))
					translation.x += spead * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		
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

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		}

		// Renderer
		Renderer2D::ResetStarts();

		m_FrameBuffer->Bind();

		RenderCommand::SetClearColor({.1f, .1f, .1f, 1});
		RenderCommand::Clear();

		// clear our entity ID attachment to -1
		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
			case SceneState::Edit:
				m_ActiveScene->OnUpdateEditor(timeStep, m_EditorCamera);

				// Update
				if (m_ViewportFocused)
				{
					m_CameraController.OnUpdate(timeStep);
					m_EditorCamera.OnUpdate(timeStep);
				}

				break;
			case SceneState::Play:
				m_ActiveScene->OnUpdateRuntime(timeStep);
				break;
		}


		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		const glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;

		const int mouseX = static_cast<int>(mx);
		const int mouseY = static_cast<int>(my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY < static_cast<int>(viewportSize.y))
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity{static_cast<entt::entity>(pixelData), m_ActiveScene.get()};
		}

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveAs();
				}

				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Renderer2d Stats");

		ImGui::Text("Hovered Entity: %s", m_HoveredEntity ? m_HoveredEntity.GetComponent<TagComponent>().Tag.c_str() : "None");

		auto stats = Renderer2D::GetStarts();
		ImGui::Text("Renderer2D Starts:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("QuadCounts: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("ViewPort");
		
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
		m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvent(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
		// if (m_ViewportSize != *reinterpret_cast<glm::vec2 *>(&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		// {
		// 	m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		// 	m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
		// 	m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		// }
		const uint32_t textureID = m_FrameBuffer->GetColorAttachmentRenderer2D();
		ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
		
		if (ImGui::BeginDragDropTarget())
		{
			// 修改鼠标指针为拖拽
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			// 获取当前窗口的绘图上下文
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			// 获取 ViewPort 的边界
			ImVec2 viewportMin = ImGui::GetWindowContentRegionMin();
			ImVec2 viewportMax = ImGui::GetWindowContentRegionMax();
			ImVec2 viewportPos = ImGui::GetWindowPos();
			ImVec2 viewportMinPos = ImVec2(viewportMin.x + viewportPos.x, viewportMin.y + viewportPos.y);
			ImVec2 viewportMaxPos = ImVec2(viewportMax.x + viewportPos.x, viewportMax.y + viewportPos.y);

			// 计算图片位置，使其居中
			ImVec2 imageSize = ImVec2(100, 100); // 假设图片大小为100x100
			ImVec2 imagePos = ImVec2(
				(viewportMinPos.x + viewportMaxPos.x - imageSize.x) / 2,
				(viewportMinPos.y + viewportMaxPos.y - imageSize.y) / 2
			);

			// 绘制半透明背景层
			draw_list->AddRectFilled(viewportMinPos, viewportMaxPos, IM_COL32(110, 110, 110, 128));

			// 绘制高亮边框
			draw_list->AddRect(viewportMinPos, viewportMaxPos, IM_COL32(255, 255, 0, 255), 0.0f, 0, 3.0f);

			// 绘制拖拽图片
			ImTextureID myTexture = reinterpret_cast<ImTextureID>(m_DropTargetTexture->GetRendererID());
			draw_list->AddImage(myTexture, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));

			if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t *path = static_cast<const wchar_t *>(payload->Data);
				OpenScene(path);
				// HZ_CORE_TRACE("Dropped file: {0}", path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera

			// Runtime Camera form entity
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity Transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;	// 对齐到 0.5 进行平移/缩放
			// 对齐 45° 进行旋转
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}

			float snapValues[3] = { snapValue , snapValue , snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;

				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;

			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event &event)
	{
		Layer::OnEvent(event);

		m_CameraController.OnEvent(event);
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.GetRepeatCount() > 0)
		{
			return false;
		}
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (event.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
				{
					NewScene();
				}
				break;
			}
			case Key::O:
			{
				if (control)
				{
					OpenScene();
				}
				break;
			}
			case Key::S:
			{
				if (control && shift)
				{
					SaveAs();
				}
				break;
			}

			// Gizmos
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		default: ;
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
	}

	void EditorLayer::SaveAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}
		return false;
	}

	void EditorLayer::OnScenePlay()
	{
		m_ActiveScene->OnRuntimeStart();
		m_SceneState = SceneState::Play;
	}

	void EditorLayer::OnSceneStop()
	{
		m_ActiveScene->OnRuntimeEnd();
		m_SceneState = SceneState::Edit;
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

		// Toolbar，不显示标题栏，不显示滚动条，不显示鼠标滚动
		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		auto& color = ImGui::GetStyle().Colors;
		const auto& buttonHovered = color[ImGuiCol_ButtonHovered];
		const auto& buttonActive = color[ImGuiCol_ButtonActive];

		std::shared_ptr<Texture2D> icon = m_SceneState == SceneState::Play ? m_IconStop : m_IconPlay;

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x - size) * 0.5f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x - size) * 0.5f);

		ImVec4 buttonColor = m_SceneState == SceneState::Play ? ImVec4(0.8f, 0.2f, 0.2f, 1.0f) : ImVec4(0.2f, 0.8f, 0.2f, 1.0f);

		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), ImVec2{ size, size }, { 0, 0 }, { 1, 1 }, 0, {0, 0, 0, 0}, buttonColor))
		{
			if (m_SceneState == SceneState::Edit)
			{
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play)
			{
				OnSceneStop();
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}
}
