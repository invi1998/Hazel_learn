#include <Hazel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
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
			{Hazel::ShaderDataType::Float3, "a_Position"},		// λ��
			{Hazel::ShaderDataType::Float4, "a_Color"}		// ��ɫ
			// {ShaderDataType::Float3, "a_Normal"}		// ����
		};

		vertexB->SetLayout(layouts);

		m_VertexArray->AddVertexBuffer(vertexB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> indexB;
		indexB.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexB);

		m_SquareVA.reset(Hazel::VertexArray::Create());
		float squareVertices[3 * 4] = {
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

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.5, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Hazel::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

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

		Hazel::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotate);

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
		Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
	}

	void OnEvent(Hazel::Event& event) override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnkeyPressedEvent));
	}

	bool OnkeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		if (event.GetKeyCode() == HZ_KEY_LEFT)
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}

		if (event.GetKeyCode() == HZ_KEY_RIGHT)
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}

		if (event.GetKeyCode() == HZ_KEY_UP)
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}

		if (event.GetKeyCode() == HZ_KEY_DOWN)
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}

		return false;
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_BlueShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;

	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotate = 0.0f;
	glm::vec3 m_CameraPosition;
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
