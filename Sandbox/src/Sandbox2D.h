#pragma once

#include "Hazel.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "ParticleSystem.h"

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
	glm::vec4 m_ParticleColorBegin = { 0.058f, 0.917f, 0.412f, 1.0f };
	glm::vec4 m_ParticleColorEnd = { 0.917f, 0.053f, 0.438f, 1.0f };

	float m_TilingFactor = 1.0f;
	float m_Rotation1 = 0.0f;
	float m_Rotation2 = 0.0f;
	float m_ParticleLifeTime = 1.0f;
	float m_ParticleBeginSize = 0.2f;
	float m_ParticleEndSize = 0.0f;
	float m_ParticleSizeVariation = 0.6f;

	std::shared_ptr<Hazel::Texture2D> m_BackgroundTexture;
	std::shared_ptr<Hazel::Texture2D> m_FrontTexture;
	std::shared_ptr<Hazel::Texture2D> m_SpriteSheet;
	std::shared_ptr<Hazel::SubTexture2D> m_TextureStairs;

	std::shared_ptr<Hazel::FrameBuffer> m_FrameBuffer;

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, std::shared_ptr<Hazel::SubTexture2D>> m_TextureMap;

};
