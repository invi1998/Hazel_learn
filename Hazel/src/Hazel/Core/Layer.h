#pragma once

#include "Hazel/Core/Timestep.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel
{

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}


