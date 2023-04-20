#pragma once
#include "Entity.h"

namespace Hazel
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetCompnent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}
