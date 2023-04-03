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

	glm::vec4 m_SquareColor = { 0.1f, 0.23f, 0.88f, 1.0f };
};