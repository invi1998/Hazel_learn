#pragma once

#include "entt.hpp"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		// temp
		entt::registry& Reg() { return m_Registry; }

		entt::entity CreateEntity();

	private:
		entt::registry m_Registry;
	};
}
