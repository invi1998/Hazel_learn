#include "Sandbox2D.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>


Sandbox2D::Sandbox2D():Layer("Sandbox2D"), m_CameraController(1920.f/1080.f, true)
{
	HZ_PROFILE_FUNCTION();

	m_BackgroundTexture = Hazel::Texture2D::Create("assets/textures/batthern.png");
	m_FrontTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	Layer::OnAttach();

}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

	Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Hazel::Timestep timeStep)
{
	HZ_PROFILE_FUNCTION();

	Layer::OnUpdate(timeStep);

	m_Particle.ColorBegin = m_ParticleColorBegin;
	m_Particle.ColorEnd = m_ParticleColorEnd;
	m_Particle.SizeBegin = m_ParticleBeginSize;
	m_Particle.SizeEnd = m_ParticleEndSize;
	m_Particle.SizeVariation = m_ParticleSizeVariation;

	// Update
	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(timeStep);
	}


	// Renderer

	Hazel::Renderer2D::ResetStarts();

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Hazel::RenderCommand::Clear();
	}

	/*{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f , -0.5f}, { 0.5f, 0.75f }, m_SquareColor1);
		Hazel::Renderer2D::DrawQuad({ -0.5f, 0.1f }, { 0.75f, 0.35f }, m_SquareColor2);
		Hazel::Renderer2D::DrawQuad({ -0.0f, -0.0f, -0.7f }, { 10.5f, 10.5f }, m_BackgroundTexture, m_TilingFactor);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 0.8f, 0.8f }, m_FrontTexture, m_TilingFactor);

		Hazel::Renderer2D::DrawQuad( { -2.5f, 0.0f, -0.1f }, { 1.5f, 1.5f }, m_Rotation2, m_BackgroundTexture, m_TilingFactor);

		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x+=0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.23f, (y + 5.0f) / 10.0f, 1.0f};
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.49f, 0.49f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}*/

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 20; i++)
		{
			m_ParticleSystem.Emit(m_Particle);
		}

	}

	m_ParticleSystem.OnUpdate(timeStep);
	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	m_ParticleSystem.OnRender();
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	Layer::OnImGuiRender();

	HZ_PROFILE_FUNCTION();

	/*ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor1));
	ImGui::ColorEdit4("Square Color2", glm::value_ptr(m_SquareColor2));
	ImGui::DragFloat("TilingFactor",&m_TilingFactor, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Rotation1",&m_Rotation1, 1.0f, 0.0f, 100.0f);
	ImGui::DragFloat("Rotation2",&m_Rotation2, 1.0f, 0.0f, 360.0f);
	ImGui::ColorEdit4("Square Color3", glm::value_ptr(m_SquareColor3));
	ImGui::ColorEdit4("Square Color4", glm::value_ptr(m_SquareColor4));

	ImGui::NewLine();

	auto stats = Hazel::Renderer2D::GetStarts();
	ImGui::Text("Renderer2D Starts:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("QuadCounts: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();*/

	ImGui::Begin("Particle System");

	ImGui::ColorEdit4("ParticleColorBegin", glm::value_ptr(m_ParticleColorBegin));
	ImGui::NewLine();
	ImGui::ColorEdit4("ParticleColorEnd", glm::value_ptr(m_ParticleColorEnd));
	ImGui::NewLine();
	ImGui::DragFloat("ParticleLifeTime", &m_ParticleLifeTime, 0.1f, 0.0f, 10.0f);
	ImGui::NewLine();
	ImGui::DragFloat("ParticleBeginSize", &m_ParticleBeginSize, 0.1f, 0.0f, 10.0f);
	ImGui::NewLine();
	ImGui::DragFloat("ParticleEndSize", &m_ParticleEndSize, 0.1f, 0.0f, 10.0f);
	ImGui::NewLine();
	ImGui::DragFloat("ParticleSizeVariation", &m_ParticleSizeVariation, 0.1f, 0.0f, 1.0f);

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	Layer::OnEvent(event);

	m_CameraController.OnEvent(event);
}
