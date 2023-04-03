#include "Sandbox2D.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D():Layer("Sandbox2D"), m_CameraController(1920.f/1080.f, true)
{
	// Vertex Array
	// Vertex Buffer
	// Index Buffer
	// Shader
	m_SquareVA = Hazel::VertexArray::Create();
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	std::shared_ptr<Hazel::VertexBuffer> squaredVB;
	squaredVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squaredVB->SetLayout({
		{Hazel::ShaderDataType::Float3, "a_Position"}
		});
	m_SquareVA->AddVertexBuffer(squaredVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<Hazel::IndexBuffer> squaredIB;
	squaredIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squaredIB);

	m_FlatShader = Hazel::Shader::Create("assets/shaders/FlatColorShader.glsl");
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

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Hazel::Renderer::Submit(m_FlatShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	Layer::OnImGuiRender();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	Layer::OnEvent(event);

	m_CameraController.OnEvent(event);
}
