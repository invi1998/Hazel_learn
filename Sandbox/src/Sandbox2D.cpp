#include "Sandbox2D.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

static const uint32_t s_MapWidth = 50;

static const char* s_MapTiles = 
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwddddddddddddt"
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwdddddddtdddd"
"wwwwwwwwwwwwwwwwdddddwwwwwwwwwwwwwwwwwwwdddtdddddd"
"wwwwwwwwwwwwdddtdddddddwwwwwwwwwwwwwwwwwwdddddddww"
"wwwwwwwwwwddddddddddtddddwwwwwwwwwwwwwwwwwddddtddw"
"wwwwwwwwwddtddddddddddddddwwwwwwwwwwwwwwwddddddddd"
"wwwwwwwddddtwwwdddddtdddddddddwwwwwwwwwwwwdddddddd"
"wwwwwddddddddwwddddddddtddddddwwwwwwwwwwwwwwwdddww"
"wwwwdddddddddddddddddddddddddddwwwwwwwwwwwwwwwwddd"
"wwwwddddddddddddddddddddwwwdddddwwwwwwwwwwwwwwwwww"
"wwwwwwddttddddddddddddwwwdddddddwwwwwwwwwwwwwwwwww"
"wwwwwddddddddddddddddddwwdddttddwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwdddddttdddddtddddwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwdddddtddddddwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
;

Sandbox2D::Sandbox2D():Layer("Sandbox2D"), m_CameraController(1920.f/1080.f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	Layer::OnAttach();

	m_BackgroundTexture = Hazel::Texture2D::Create("assets/textures/batthern.png");
	m_FrontTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
	//m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	//m_MapWidth = s_MapWidth;
	//m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	//m_TextureStairs = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128.0f, 128.0f }, { 1.0f, 1.0f });

	//// 水
	//m_TextureMap['w'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128.0f, 128.0f }, { 1.0f, 1.0f });
	//// 陆地
	//m_TextureMap['d'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128.0f, 128.0f }, { 1.0f, 1.0f });
	//// 树
	//m_TextureMap['t'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 128.0f, 128.0f }, { 1.0f, 1.0f });

	//m_Particle.Velocity = { 0.0f, 0.0f };
	//m_Particle.VelocityVariation = { 3.0f, 1.0f };
	//m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f);

	Hazel::FrameBufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_FrameBuffer = Hazel::FrameBuffer::Create(fbSpec);
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

	/*m_Particle.ColorBegin = m_ParticleColorBegin;
	m_Particle.ColorEnd = m_ParticleColorEnd;
	m_Particle.SizeBegin = m_ParticleBeginSize;
	m_Particle.SizeEnd = m_ParticleEndSize;
	m_Particle.SizeVariation = m_ParticleSizeVariation;*/

	// Update
	m_CameraController.OnUpdate(timeStep);


	// Renderer
	Hazel::Renderer2D::ResetStarts();

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		m_FrameBuffer->Bind();

		Hazel::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f , -0.5f}, { 0.5f, 0.75f }, m_SquareColor1);
		Hazel::Renderer2D::DrawQuad({ -0.5f, 0.1f }, { 0.75f, 0.35f }, m_SquareColor2);
		Hazel::Renderer2D::DrawQuad({ -0.0f, -0.0f, -0.7f }, { 10.5f, 10.5f }, m_BackgroundTexture, m_TilingFactor);
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 0.8f, 0.8f }, m_FrontTexture, m_TilingFactor);

		Hazel::Renderer2D::DrawQuad( { -2.5f, 0.0f, -0.1f }, { 1.5f, 1.5f }, m_Rotation2, m_BackgroundTexture, m_TilingFactor);

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x+=0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.23f, (y + 5.0f) / 10.0f, 1.0f};
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.49f, 0.49f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
		m_FrameBuffer->UnBind();
	}

	///////////////////////////////////粒子绘制////////////////////////////////////////////////////
	/*if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
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
	Hazel::Renderer2D::EndScene();*/
	//////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////地图绘制/////////////////////////////////////////////////////
	//Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	//Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_TextureStairs);

	//for (uint32_t y = 0; y < m_MapHeight; y++)
	//{
	//	for (uint32_t x = 0; x < m_MapWidth; x++)
	//	{
	//		char tileType = s_MapTiles[x + y * m_MapWidth];
	//		std::shared_ptr<Hazel::SubTexture2D> texture;
	//		if (m_TextureMap.contains(tileType))
	//		{
	//			texture = m_TextureMap[tileType];
	//		}
	//		else
	//		{
	//			texture = m_TextureStairs;
	//		}

	//		Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.5f }, { 1.0f, 1.0f }, texture);
	//	}
	//}

	//Hazel::Renderer2D::EndScene();
	//////////////////////////////////////////////////////////////////////////////////////////
}

void Sandbox2D::OnImGuiRender()
{
	Layer::OnImGuiRender();

	HZ_PROFILE_FUNCTION();

	/*ImGui::Begin("Particle System");

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

	ImGui::End();*/

	static bool dockingEnabled = true;

	if (dockingEnabled)
	{
		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
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
		ImGuiIO& io = ImGui::GetIO();
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
					Hazel::Application::Get().Close();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor1));
		ImGui::ColorEdit4("Square Color2", glm::value_ptr(m_SquareColor2));
		ImGui::DragFloat("TilingFactor", &m_TilingFactor, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("Rotation1", &m_Rotation1, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("Rotation2", &m_Rotation2, 1.0f, 0.0f, 360.0f);
		ImGui::ColorEdit4("Square Color3", glm::value_ptr(m_SquareColor3));
		ImGui::ColorEdit4("Square Color4", glm::value_ptr(m_SquareColor4));

		ImGui::NewLine();

		auto stats = Hazel::Renderer2D::GetStarts();
		ImGui::Text("Renderer2D Starts:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("QuadCounts: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		const uint32_t textureID = m_FrameBuffer->GetColorAttachmentRenderer2D();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 1280, 720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");
		
		ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor1));
		ImGui::ColorEdit4("Square Color2", glm::value_ptr(m_SquareColor2));
		ImGui::DragFloat("TilingFactor", &m_TilingFactor, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("Rotation1", &m_Rotation1, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("Rotation2", &m_Rotation2, 1.0f, 0.0f, 360.0f);
		ImGui::ColorEdit4("Square Color3", glm::value_ptr(m_SquareColor3));
		ImGui::ColorEdit4("Square Color4", glm::value_ptr(m_SquareColor4));

		ImGui::NewLine();

		auto stats = Hazel::Renderer2D::GetStarts();
		ImGui::Text("Renderer2D Starts:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("QuadCounts: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalIndexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		const uint32_t textureID = m_BackgroundTexture->GetRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 1280, 720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();
	}
	

}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	Layer::OnEvent(event);

	m_CameraController.OnEvent(event);
}
