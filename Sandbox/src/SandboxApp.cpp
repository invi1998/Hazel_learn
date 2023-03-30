#include <Hazel.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		// Vertex Array
		// Vertex Buffer
		// Index Buffer
		// Shader

		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.3f, 0.7f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.3f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f
		};

		std::shared_ptr<Hazel::VertexBuffer> vertexB;
		vertexB.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		const Hazel::BufferLayout layouts = {
			{Hazel::ShaderDataType::Float3, "a_Position"},		// 位置
			{Hazel::ShaderDataType::Float4, "a_Color"}		// 颜色
			// {ShaderDataType::Float3, "a_Normal"}		// 法线
		};

		vertexB->SetLayout(layouts);

		m_VertexArray->AddVertexBuffer(vertexB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> indexB;
		indexB.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexB);

		m_SquareVA.reset(Hazel::VertexArray::Create());
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> squaredVB;
		squaredVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squaredVB->SetLayout({
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVA->AddVertexBuffer(squaredVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squaredIB;
		squaredIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squaredIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.4, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";

		m_FlatShader = Hazel::Shader::Create("FlatColor", flatShaderVertexSrc, flatShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/h.jpg");
		m_LogoTexture = Hazel::Texture2D::Create("assets/textures/mian.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep timeStep) override
	{

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * timeStep;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * timeStep;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * timeStep;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * timeStep;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraRotate += m_CameraRotationSpeed * timeStep;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraRotate -= m_CameraRotationSpeed * timeStep;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
		{
			m_SquarePosition.y += m_SquareMoveSpeed * timeStep;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
		{
			m_SquarePosition.y -= m_SquareMoveSpeed * timeStep;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
		{
			m_SquarePosition.x -= m_SquareMoveSpeed * timeStep;
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
		{
			m_SquarePosition.x += m_SquareMoveSpeed * timeStep;
		}

		Hazel::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotate);

		Hazel::Renderer::BeginScene(m_Camera);

		//glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, m_SquarePosition);
		glm::mat4 scale = glm::scale(glm::mat4{ 1.0f }, glm::vec3{0.1f});

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->Bind();

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos{ x * 0.11f, y*0.11f, 0.0f};
				glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, pos) * scale;
				if (x%2 == 0 && y%2 != 0 || x%2 != 0 && y%2 == 0)
				{
					std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_SquareColor1);
				}
				else
				{
					std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_SquareColor2);
				}
				Hazel::Renderer::Submit(m_FlatShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(0.0f), glm::vec3(0.5f)));

		m_LogoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)));
		// Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color1", glm::value_ptr(m_SquareColor1));
		ImGui::ColorEdit4("Square Color2", glm::value_ptr(m_SquareColor2));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
	}

	bool OnkeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
	}

private:
	Hazel::ShaderLibrary m_ShaderLibrary;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_FlatShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	// 纹理
	std::shared_ptr<Hazel::Texture2D> m_Texture, m_LogoTexture;

	// 相机
	Hazel::OrthographicCamera m_Camera;

	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotate = 0.0f;
	glm::vec3 m_CameraPosition;

	// 变换
	glm::vec3 m_SquarePosition;
	glm::mat4 m_SquareTransform;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec4 m_SquareColor1;
	glm::vec4 m_SquareColor2;

};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox(){}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox;
}
