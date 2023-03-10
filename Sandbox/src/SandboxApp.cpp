#include <Hazel.h>
#include <imgui/imgui.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("this is a test windows");
		ImGui::ColorButton("click this", ImVec4(0.1f, 0.6f, 1.0f, 0.4f));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		if(event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = dynamic_cast<Hazel::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == HZ_KEY_TAB)
			{
				HZ_INFO("Tab is preesed!");
			}
			HZ_TRACE("{0}", static_cast<char>(e.GetKeyCode()));
		}
	}
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
