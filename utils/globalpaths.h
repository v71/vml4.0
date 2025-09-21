#pragma once

////////////////////////////////////////////////////////////////////////////
// 
// 	class Session

#include <unordered_map>
#include <filesystem>

namespace vml
{
	namespace utils
	{
		
		///////////////////////////////////////////////////////////////////////////////////////
		// Thread safe logger

		class GlobalPaths
		{

			private:

				// ---------------------------------------------
				// singleton design pattern
				// A singlton is a class instantiated only once.
				// Usually it is globally accessiblae,
				// constructor and destructors are kept private to the 
				// final user. User has the responsability to delete
				// memory associated with the class explicitly at
				// main entry point closure

				static GlobalPaths*							  Singleton;		// Singleton pointer
				std::string								      UserName;			// user name
				std::string									  ProjectPath;		// project path
				std::string								      ProjectName;		// project name
				std::unordered_map<std::string, std::string>  ProjectDirs;		// store generated directories	

				// ---------------------------------------------
				// private ctor / dtor

				GlobalPaths()
				{
					Singleton = nullptr;
				}

			public:

				//---------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				GlobalPaths(GlobalPaths& other) = delete;

				//---------------------------------------------------------------------
				// overload operator is private, 
				// no copies allowed since classes
				// are referenced

				GlobalPaths operator=(const GlobalPaths&) = delete;

				//---------------------------------------------------------------------
				// get instance of singleton , if singleton is null , which is mandatory at
				// application startup, it will be allocated only once
				// Eventually, to reallocate , user must call the delete member function

				static GlobalPaths *GetInstance()
				{
					if (!Singleton)
						Singleton = new GlobalPaths();
					return Singleton;
				}

				// -----------------------------------------------------------------------------

				void Init()
				{
				}

				// -----------------------------------------------------------------------------
				// sets name for current user

				void SetCurrentUserName(const std::string& name)
				{
					if (name.empty())
						vml::os::Message::Error("Paths : UserName is empty");
					if (!UserName.empty())
						vml::os::Message::Error("Paths : UserName is already set");
					
					// set user name

					UserName = name;
					
					vml::utils::Logger::GetInstance()->Info("Paths : Setting UserName As : " + UserName);
				}

				// -----------------------------------------------------------------------------
				// set wrking directory and paths

				void SetProjectPath(const std::string& mainpath)
				{
					if (mainpath.empty())
						vml::os::Message::Error("Paths : Project Path is empty");
					if (!ProjectPath.empty())
						vml::os::Message::Error("Paths : Project Path is already set");

					// setting main path

					ProjectPath = std::string(std::filesystem::path({ mainpath }).string());

					// checks if main path exists

					if (std::filesystem::status_known(std::filesystem::file_status{}) ? std::filesystem::exists(std::filesystem::file_status{}) : std::filesystem::exists(mainpath))
						vml::utils::Logger::GetInstance()->Info("Paths : Installation Path = '" + ProjectPath + "'");
					else 
						vml::os::Message::Error("Paths : Missing Project Path");
				}

				// -----------------------------------------------------------------------------
				// set project name

				void SetProjectName(const std::string& projectname)
				{
					if (projectname.empty())
						vml::os::Message::Error("Paths : Project name is empty");
					if (!ProjectName.empty())
						vml::os::Message::Error("Paths : Project Name is already set");
			
					// set project name

					ProjectName = projectname;

					// log dirs creation 

					vml::utils::Logger::GetInstance()->Info("Paths : Project Name : " + ProjectName);
				}

				// -----------------------------------------------------------------------------
				// create projectdir

				void CreateProjectDirs(const std::vector<std::string>& paths) 
				{
					
					if (paths.empty())
						vml::os::Message::Error("Paths : Dirs list is empty");
					if (GetProjectPath().empty())
						vml::os::Message::Error("Paths : Project path is missing");
					if (GetProjectName().empty())
						vml::os::Message::Error("Paths : Project Name is missing");
					
					// create directories for the project

					for (auto it = paths.begin(); it != paths.end(); ++it)
					{
						std::string fulldir = GetProjectPath() + "\\" + GetProjectName() + "\\" + vml::strings::StringUtils::MakeLower((*it)+"\\");

						// create the pair and insert into map

						ProjectDirs.insert(std::make_pair((*it), fulldir));
						
						// create physiucal dir

						std::filesystem::create_directories(fulldir);
						
					}
					
					// log dirs creation 

					vml::utils::Logger::GetInstance()->Info("Paths : Project directories Created");
					
					// enumeratoe dirs

					std::vector<std::string> dirs = GetProjectDirs();

					for (size_t i = 0; i < dirs.size(); ++i)
						vml::utils::Logger::GetInstance()->Info("Paths : Dir " + std::to_string(i) + " = '" + dirs[i] + "'");
					
				}
		
				// -----------------------------------------------------------------------------
				// getters

				const std::string GetUserName()			const { return UserName; }
				const std::string GetProjectPath()		const { return ProjectPath; }
				const std::string GetProjectName()		const { return ProjectName; }
				const std::string GetFullProjectPath()	const { return ProjectPath + "\\" + ProjectName; }
				const std::string GetFullDebugPath()	const { return ProjectPath + "\\debug"; }

				// -----------------------------------------------------------------------------
				// list recursively all subdirs given a path

				std::vector<std::string> GetProjectDirs() const
				{
					if (GetProjectPath().empty())
						vml::os::Message::Error("Paths : Project path is missing");
					if (GetProjectName().empty())
						vml::os::Message::Error("Paths : Project Name is missing");

					// enumerate project directories and store them 
					// int a vector

					std::filesystem::path path = vml::utils::GlobalPaths::GetInstance()->GetProjectPath() + "\\" + GetProjectName()+"\\";

					std::vector<std::string> dirs;
					
					for (const auto& p : std::filesystem::recursive_directory_iterator(path))
					{
						if (std::filesystem::is_directory(p))
						{
							dirs.emplace_back(p.path().generic_string());
						}
					}
					
					return dirs;
				}

				// ------------------------------------------------------------
				// this functions return the required dir
				// note that the string is returned as a COPY
			
				const std::string GetProjectDir(const std::string& dir)
				{
					std::unordered_map<std::string, std::string>::const_iterator projdir = ProjectDirs.find(dir);
					if (projdir != ProjectDirs.end())
						return projdir->second;
					return std::string();
				}

				// ------------------------------------------------------------
				// closes stream

				void Close()
				{
					vml::os::SafeDelete(Singleton);
				}

				// ------------------------------------------------------------
				// ctor / dtor

				~GlobalPaths() {};

		};
	}
}

