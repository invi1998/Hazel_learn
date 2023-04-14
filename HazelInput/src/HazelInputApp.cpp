#include <Hazel.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include "EditorLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

// ---------entry point-----------

#include "Hazel/Core/EntryPoint.h"

// -------------------------------

namespace Hazel
{
	class HazelInput : public Application
	{
	public:
		HazelInput() : Application("Hazel Editor")
		{
			PushLayer(new EditorLayer());
		}

		~HazelInput() {}
	};

	Application* CreateApplication()
	{
		return new HazelInput;
	}
}

