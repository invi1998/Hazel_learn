#pragma once

#include "entt.hpp"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		// temp
		entt::registry& Reg() { return m_Registry; }

		Entity CreateEntity(const std::string& name = std::string());

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
