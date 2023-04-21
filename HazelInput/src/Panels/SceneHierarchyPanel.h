#pragma once
#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/Scene.h"

namespace Hazel
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& context);

		void SetContext(const std::shared_ptr<Scene>& context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		std::shared_ptr<Scene> m_Context;
		Entity m_SelectionContext;
	};
}
