#include "Sandbox2D.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D():Layer("Sandbox2D"), m_CameraController(1920.f/1080.f, true)
{
	m_BackgroundTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_FrontTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
}

void Sandbox2D::OnAttach()
{
	Layer::OnAttach();
}

void Sandbox2D::OnDetach()
{
	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Hazel::Timestep timeStep)
{
	Layer::OnUpdate(timeStep);

	m_CameraController.OnUpdate(timeStep);

	Hazel::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());


	Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor1);
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, m_SquareColor2);

	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 0.5f, 0.5f }, m_BackgroundTexture);
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.4f, 0.4f }, m_FrontTexture);

	Hazel::Renderer2D::EndScene();

	//std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->Bind();
	//std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	//Hazel::Renderer::Submit(m_FlatShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
}

void Sandbox2D::OnImGuiRender()
{
	Layer::OnImGuiRender();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor1));
	ImGui::ColorEdit4("Square Color2", glm::value_ptr(m_SquareColor2));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	Layer::OnEvent(event);

	m_CameraController.OnEvent(event);
}
