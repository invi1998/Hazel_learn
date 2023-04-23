#pragma once

#include "Hazel.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Panels/SceneHierarchyPanel.h"

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

		bool m_PrimaryCamera = false;

		glm::vec2 m_ViewportSize = { 0.0, 0.0f };

		// panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}

