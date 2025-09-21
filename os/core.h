#pragma once

namespace vml
{

	class Core
	{

		private:

			// -----------------------------------------------------------------------------
			// private data

			uint32_t								InternalFlags;			// internal flags
			uint32_t								PreferencesFlags;		// prefernces flags
			std::clock_t							ClockStart;				// session time
			vml::os::Timer							Timer;					// timer class
			vml::OpenGLContextWindow*				OpenGLContextWindow;	// opengl context window
			vml::debugrendering::OpenglDebugRender* OpenglDebugRender;		// opengl debug renderer
			
			//-----------------------------------------------------------------------------------
			// validate verbose mode

			bool [[nodiscard]] ValidateVerboseMode() const
			{
				if (IsVerbose() && IsQuiet())
					return false;
				if (!IsVerbose() && !IsQuiet())
					return false;
				return true;
			}

			//-----------------------------------------------------------------------------------
			// validate log mode

			bool [[nodiscard]] ValidateLogMode() const
			{
				if (IsDebugLogToStdout() && !IsDebugLogToMem() && !IsDebugLogToFile()) return true;
				if (IsDebugLogToFile() && !IsDebugLogToMem() && !IsDebugLogToStdout()) return true;
				if (IsDebugLogToMem() && !IsDebugLogToStdout() && !IsDebugLogToFile()) return true;
				return false;
			}

		public:

			//-----------------------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed 

			Core(Core& core) = delete;

			//-----------------------------------------------------------------------------------
			// overload operator is private,
			// no copies allowed since classes
			// are referenced

			Core &operator=(const Core&) = delete;

			// -----------------------------------------------------------------------------
			// getters

			bool			   IsInitialized()						 const { return vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED); }
			bool			   IsVerbose()							 const { return vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::VERBOSE); }
			bool			   IsQuiet()							 const { return vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::QUIET); }
			bool			   IsDebugLogToFile()					 const { return vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::LOG_TO_FILE); }
			bool			   IsDebugLogToStdout()				     const { return vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::LOG_TO_STDOUT); }
			bool			   IsDebugLogToMem()					 const { return vml::utils::bits32::Get(PreferencesFlags, vml::utils::PreferencesFlags::LOG_TO_MEMORY); }
			bool			   IsUserNameValid()				     const { return !vml::utils::GlobalPaths::GetInstance()->GetUserName().empty(); }
			bool			   IsMainProjectValid()					 const { return !vml::utils::GlobalPaths::GetInstance()->GetProjectPath().empty(); }
			const std::string  GettUserName()						 const { return vml::utils::GlobalPaths::GetInstance()->GetUserName(); }
			const std::string  GetProjectPath()						 const { return vml::utils::GlobalPaths::GetInstance()->GetProjectPath(); }
			const std::string  GetProjectName()						 const { return vml::utils::GlobalPaths::GetInstance()->GetProjectName(); }
			const std::string  GetFullProjectPath()					 const { return vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath(); }
			const std::string  GetFullDebugPath()					 const { return vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath(); }
			float			   GetTime()							 const { return std::clock(); }
			float			   GetFps()								       { return Timer.GetFPS(); }
			const std::string  GetFpsString()							   { return std::to_string(Timer.GetFPS()); }
			const std::string  GetProjectDir(const std::string& dir) const { return vml::utils::GlobalPaths::GetInstance()->GetProjectDir(dir); }

			// -----------------------------------------------------------------------------
			// init core

			void Init(unsigned int preferencesflags)
			{
				// check if class has been initialised

				if (IsInitialized())
					vml::os::Message::Error("Core : Cannot restart context before it has not been closed");
		
				// set preferences flags

				PreferencesFlags = preferencesflags;

				// validate preferences flags

				if (!ValidateVerboseMode())
					vml::os::Message::Error("Core : Verbose Mode mismatch/not set");

				// check if debug flag is set

				if (!ValidateLogMode())
					vml::os::Message::Error("Core : Debug mode mismatch/not set");

				// Inits logger accoridng to log mode

				if (IsDebugLogToStdout()) 
					vml::utils::Logger::GetInstance()->Init(vml::utils::Logger::LogMode::TO_STD);
				if (IsDebugLogToMem())
					vml::utils::Logger::GetInstance()->Init(vml::utils::Logger::LogMode::TO_MEM);
				if (IsDebugLogToFile())   
					vml::utils::Logger::GetInstance()->Init(vml::utils::Logger::LogMode::TO_FILE, "log.txt");
			
				// log out that core has been initted correctly

				vml::utils::Logger::GetInstance()->Info("Core : Initting Main : In progress");

				// init global paths
				
				vml::utils::GlobalPaths::GetInstance()->Init();
				vml::utils::Logger::GetInstance()->Info("Core : Initting Global Paths : Done");

				// log out verbose mode

				if (IsVerbose()) 
					vml::utils::Logger::GetInstance()->Info("Core : Verbose flag set to 'True'");
				if (IsQuiet())	 
					vml::utils::Logger::GetInstance()->Info("Core : Verbose flag set to 'False'");

				// log logger mode
			
				if (IsDebugLogToStdout()) 
					vml::utils::Logger::GetInstance()->Info("Core : Logger output set to 'Stdout'");
				if (IsDebugLogToMem())    
					vml::utils::Logger::GetInstance()->Info("Core : Logger output set to 'Memory'");
				if (IsDebugLogToFile())   
					vml::utils::Logger::GetInstance()->Info("Core : Logger output set to 'File'");

				// init timer

				Timer.Init();

				vml::utils::Logger::GetInstance()->Info("Core : Initting performance timer : Done");

				//start measuring

				if (IsVerbose())
				{
					vml::os::SystemInfo SystemInfo;

					SystemInfo.Measure();
					
					vml::utils::Logger::GetInstance()->Info("SysInfo : Vendor: "					   + SystemInfo.GetCpuVendorString());
					vml::utils::Logger::GetInstance()->Info("SysInfo : CodeName: "					   + SystemInfo.GetCpuCodeNameString());
					vml::utils::Logger::GetInstance()->Info("SysInfo : Brand : "					   + SystemInfo.GetCpuBrandString());
					vml::utils::Logger::GetInstance()->Info("SysInfo : VendorId : "					   + std::to_string(SystemInfo.GetVendorId()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : CodeNameId : "				   + std::to_string(SystemInfo.GetCodeNameId()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Stepping : "					   + std::to_string(SystemInfo.GetStepping()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Family : "					   + std::to_string(SystemInfo.GetFamily()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Model : "					   + std::to_string(SystemInfo.GetModel()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : CPUMaxFunction : "			   + std::to_string(SystemInfo.GetCPUIdMaxFunction()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Processr Type : "			   + std::to_string(SystemInfo.GetProcessorType()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Extended Model : "			   + std::to_string(SystemInfo.GetExtendedModel()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Extended Family : "			   + std::to_string(SystemInfo.GetExtendedFamily()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Brand Index : "				   + std::to_string(SystemInfo.GetBrandIndex()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : CLFLUSH Cache line size : "	   + std::to_string(SystemInfo.GetCLFLUSHCacheLineSize()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : API Physical ID : "			   + std::to_string(SystemInfo.GetAPICPhysicalID()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Cache line size : "			   + std::to_string(SystemInfo.GetCacheLineSize()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : L2 Associativity : "            + std::to_string(SystemInfo.GetL2Associativity()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Cache size : "                  + std::to_string(SystemInfo.GetCacheSize()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Physical address : "            + std::to_string(SystemInfo.GetPhysicalAddress()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Virtual address : "             + std::to_string(SystemInfo.GetVirtualAddress()));
					vml::utils::Logger::GetInstance()->Info("SysInfo : Logical processor(s) number : " + std::to_string(SystemInfo.GetLogicalProcessorsNumber()));
					
				}

				vml::utils::Logger::GetInstance()->Info("Core : Retrieving system info : Done");

				// log out that core has been initted correctly

				vml::utils::Logger::GetInstance()->Info("Core : Initting Main : Done");
				
				// set context as started
				
				vml::utils::bits32::SetToTrue(InternalFlags, vml::utils::InternalFlags::INITTED);
			}

			// -----------------------------------------------------------------------------
			// Initting stores
			
			void InitStores() const
			{
				if (!IsInitialized())
					vml::os::Message::Error("Core : Not initted when setting MainPath");
				if (GetProjectPath().empty())
					vml::os::Message::Error("Core : Project path is missing");
				if (GetProjectName().empty())
					vml::os::Message::Error("Core : Project Name is missing");
				
				// init stores

				vml::utils::Logger::GetInstance()->Info("Core : Initting Stores : In progress");

				vml::stores::ShaderStore  = new vml::utils::SharedResourceStore("ShaderHandler");
				vml::stores::MeshStore    = new vml::utils::SharedResourceStore("MeshHandler");
				vml::stores::TextureStore = new vml::utils::SharedResourceStore("TextureHandler");

				vml::utils::Logger::GetInstance()->Info("Core : Initting Stores : Done");

			}

			// -----------------------------------------------------------------------------
			// closes Core

			void Close()
			{
				
				if (!IsInitialized())
					vml::os::Message::Error("Core : Cannot close context before it has not been started");
				
				// log closes by itself in its destructor

				vml::utils::Logger::GetInstance()->Info("Core : Shutting Down Main : In progress");
				
				// close opengl debugger

				vml::os::SafeDelete(OpenglDebugRender);

				vml::utils::Logger::GetInstance()->Info("Core : Closing Opengl Debugger : Done");

				// clear stores
				// we need to delete stores
				// the store might address some
				// rendering classes such as
				// shaders and textures which need
				// an active in the rendering context to be
				// properly deleted
												
				vml::os::SafeDelete(vml::stores::MeshStore);
				vml::os::SafeDelete(vml::stores::ShaderStore);
				vml::os::SafeDelete(vml::stores::TextureStore);

				vml::utils::Logger::GetInstance()->Info("Core : Shutting Down Stores : Done");

				// close opengl context

				vml::os::SafeDelete(OpenGLContextWindow);

				vml::utils::Logger::GetInstance()->Info("Core : Closing OpenglContext Winodw : Done");

				// close global paths singleton

				vml::utils::GlobalPaths::GetInstance()->Close();
				
				vml::utils::Logger::GetInstance()->Info("Core : Shutting Down GlobalPaths : Done");
				
				// log out results

				vml::utils::Logger::GetInstance()->Info("Core : Shutting Down Main : Done");

				// close logger 

				vml::utils::Logger::GetInstance()->Close();
				
			}

			// -----------------------------------------------------------------------------
			// set wrking directory and paths

			void SetProjectPath(const std::string& mainpath) const
			{
				if (!IsInitialized())
					vml::os::Message::Error("Core : Not initted when setting MainPath");
				vml::utils::GlobalPaths::GetInstance()->SetProjectPath(mainpath);
			}

			// -----------------------------------------------------------------------------
			// set project name
			
			void SetProjectName(const std::string& projectname) const
			{
				if (!IsInitialized())
					vml::os::Message::Error("Core : Not initted when setting MainPath");
				vml::utils::GlobalPaths::GetInstance()->SetProjectName(projectname);
			}

			// -----------------------------------------------------------------------------
			// create projectdir

			void CreateProjectDirs(const std::vector<std::string>& paths) const
			{
				if (!IsInitialized())
					vml::os::Message::Error("Core : Not initted when setting MainPath");
				vml::utils::GlobalPaths::GetInstance()->CreateProjectDirs(paths);
			}

			// -----------------------------------------------------------------------------
			// sets name for current user

			void SetCurrentUserName(const std::string& name) const
			{
				if (!IsInitialized())
					vml::os::Message::Error("Core : Not initted when setting UserName");
				vml::utils::GlobalPaths::GetInstance()->SetCurrentUserName(name);
			}

			// -----------------------------------------------------------
			// initting rendering context

			void InitGLWindow(int width, int height, const std::string& title, unsigned int flags)
			{
				OpenGLContextWindow = new vml::OpenGLContextWindow();
				OpenglDebugRender = new vml::debugrendering::OpenglDebugRender();
				OpenGLContextWindow->Init(width, height, title, flags);
				OpenglDebugRender->Init();
			}

			// -----------------------------------------------------------
			// rendering context functions
			
			__forceinline void									  SwapBuffers()							 const { OpenGLContextWindow->SwapBuffers(); }
			__forceinline void									  PollEvents()							 const { OpenGLContextWindow->PollEvents(); }
			__forceinline void									  SetWindowTitle(const std::string& str) const { OpenGLContextWindow->SetWindowTitle(str); }
			__forceinline void									  InitEvents()							 const { OpenGLContextWindow->InitEvents(); }
			__forceinline bool									  IsWindowRunning()						 const { return OpenGLContextWindow->IsRunning(); }
			__forceinline GLFWwindow*							  GetGLFWindow()						 const { return OpenGLContextWindow->GetWindow(); }
			__forceinline vml::OpenGLContextWindow*				  GetOpenGLContextWindow()				 const { return OpenGLContextWindow; }
			__forceinline vml::debugrendering::OpenglDebugRender* GetOpenglDebugRender()				 const { return OpenglDebugRender; }
			
			// -----------------------------------------------------------------------------
			// virtual function members

			virtual void LoadResources()    = 0;
			virtual void DisposeResources() = 0;
			virtual void KeyBindings()		= 0;

			// -----------------------------------------------------------------------------
			// ctor / dtor

			Core()
			{
				InternalFlags	    = 0;
				PreferencesFlags    = 0;
				ClockStart		    = std::clock();
				OpenglDebugRender   = nullptr;
				OpenGLContextWindow = nullptr;
			}

			virtual ~Core()
			{
			}

	};


} // end of vml namespace
