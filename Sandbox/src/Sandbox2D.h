#pragma once

#include "Hazel.h"
#include "Platform/OpenGL/OpenGLShader.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() override = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Hazel::Timestep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& event) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	std::shared_ptr<Hazel::Shader> m_FlatShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	glm::vec4 m_SquareColor1 = { 0.1f, 0.23f, 0.88f, 1.0f };
	glm::vec4 m_SquareColor2 = { 0.1f, 0.66f, 0.12f, 1.0f };
	glm::vec4 m_SquareColor3 = { 0.1f, 0.23f, 0.88f, 1.0f };
	glm::vec4 m_SquareColor4 = { 0.1f, 0.66f, 0.12f, 1.0f };

	float m_TilingFactor = 1.0f;
	float m_Rotation1 = 0.0f;
	float m_Rotation2 = 0.0f;

	std::shared_ptr<Hazel::Texture2D> m_BackgroundTexture;
	std::shared_ptr<Hazel::Texture2D> m_FrontTexture;

};
