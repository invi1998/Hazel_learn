#include "hzpch.h"
#include "Project.h"

namespace Hazel
{
	std::shared_ptr<Project> Project::New()
	{
		s_ActiveProject = std::make_shared<Project>();
		return s_ActiveProject;
	}

	std::shared_ptr<Project> Project::Load(const std::filesystem::path& filepath)
	{
		s_ActiveProject = std::make_shared<Project>();
		return s_ActiveProject;
	}

	bool Project::SaveActive(const std::filesystem::path& filepath)
	{
		
		return false;
	}

}

