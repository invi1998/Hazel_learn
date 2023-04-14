#pragma once

#include "Hazel.h"
#include "Platform/OpenGL/OpenGLShader.h"

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

		std::shared_ptr<Shader> m_FlatShader;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Texture2D> m_BackgroundTexture;
		std::shared_ptr<FrameBuffer> m_FrameBuffer;

		glm::vec4 m_SquareColor = { 0.1f, 0.23f, 0.88f, 1.0f };
	};
}

