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
	class Hazelnut : public Application
	{
	public:
		Hazelnut() : Application("Hazel Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Hazelnut() {}
	};

	Application *CreateApplication()
	{
		return new Hazelnut;
	}
}
