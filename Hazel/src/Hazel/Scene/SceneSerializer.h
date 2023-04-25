#pragma once

namespace Hazel
{
	class Scene;

	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene>& scene);

		bool Serialize(const std::string& filepath);
		bool SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		std::shared_ptr<Scene> m_Scene;
	};
}
