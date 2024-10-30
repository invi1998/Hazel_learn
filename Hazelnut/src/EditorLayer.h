#pragma once

#include "Hazel.h"
#include "Hazel/Scene/SceneSerializer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep timeStep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveAs();

		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

	private:
		OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		std::shared_ptr<Shader> m_FlatShader;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Texture2D> m_BackgroundTexture;
		std::shared_ptr<FrameBuffer> m_FrameBuffer;

		std::shared_ptr<Scene> m_ActiveScene;
		Entity m_SquaredEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = false;

		EditorCamera m_EditorCamera;

		glm::vec2 m_ViewportSize = { 0.0, 0.0f };
		glm::vec2 m_ViewportBounds[2]{};

		int m_GizmoType = - 1;

		// panels
		SceneHierarchyPanel m_SceneHierarchyPanel;		// 场景层次面板
		ContentBrowserPanel m_ContentBrowserPanel;		// 内容浏览面板

		// Drop BG
		std::shared_ptr<Texture2D> m_DropTargetTexture;	// 拖拽背景
		
	};
}

