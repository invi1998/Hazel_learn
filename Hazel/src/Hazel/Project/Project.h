#pragma once

#include <filesystem>

namespace Hazel
{
	struct ProjectConfig
	{
		std::string Name{ "Untitled" };
		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory = "assets";
		std::filesystem::path ScriptModuleDirectory = "scripts";
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()
		{
			HZ_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->s_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			HZ_CORE_ASSERT(s_ActiveProject, "No active project");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetAssetPath(const std::filesystem::path& path)
		{
			HZ_CORE_ASSERT(s_ActiveProject, "No active project");
			return GetAssetDirectory() / path;
		}

		ProjectConfig& GetConfig() { return m_Config; }
		static std::shared_ptr<Project> GetActiveProject() { return s_ActiveProject; }

		static std::shared_ptr<Project> New();
		static std::shared_ptr<Project> Load(const std::filesystem::path& filepath);
		static bool SaveActive(const std::filesystem::path& filepath);


	private:
		ProjectConfig m_Config;
		std::filesystem::path s_ProjectDirectory;

		static std::shared_ptr<Project> s_ActiveProject;
	};
}

