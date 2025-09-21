#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

//////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL and GLEW Header Files and Libraries

#include <GL\glew.h>
#include <GL\wglew.h>
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>

///////////////////////////////////////////////////////////////////////////////////////////
// key handle for detecting single keypress ,hopefully this will
// be included in some new glfw version one day or another

static char glfkeyOnce[GLFW_KEY_LAST + 1] = { 0 };

static bool glfwGetKeyOnce(GLFWwindow* window, unsigned int key)
{
	if (glfwGetKey(window, key))
	{
		if (!glfkeyOnce[key]) 
		{
			glfkeyOnce[key] = true;
			return true;
		}
		return false;
	}
//	else 
	{
		glfkeyOnce[key] = false;
		return false;
	}
}

static std::chrono::time_point<std::chrono::system_clock> glfwKeyStartClock[GLFW_KEY_LAST + 1];
static char glfwKeyPressed[GLFW_KEY_LAST + 1] = { 0 };

static bool glfwKeyRepeat(GLFWwindow* glwindow, unsigned int key, unsigned int limit=250)
{
	if (!glfwKeyPressed)
	{
		glfwKeyStartClock[key] = std::chrono::system_clock::now();
		glfwKeyPressed[key] = 1;
	}

	if (glfwGetKey(glwindow, key))
	{
		unsigned int diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - glfwKeyStartClock[key]).count();

		if (diff > limit)
		{
			glfwKeyStartClock[key] = std::chrono::system_clock::now();
			glfwKeyPressed[key] = 0;
			return true;
		}
	}
	else
	{
		glfwKeyPressed[key] = 0;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

namespace vml
{
	
	class OpenGLContextWindow
	{
		
		private:
					
			//------------------------------------------------------------------------------
			// private data

			unsigned int	Flags;
			GLFWwindow	   *Window;
			bool			Running;
			bool			ExitOnEsc;
			std::string		RendererString;
			std::string		VersionString;
			unsigned int	GlFallBackVersion;
			int				ScreenWidth;
			int				ScreenHeight;
			double			MouseX;
			double			MouseY;
			double			MouseXDelta;
			double			MouseYDelta;
			int				LeftButtonState;
			int				RightButtonState;
			int				MiddleButtonState;
			
			// ---------------------------------------------------------------------------
			// gets opengl fall back string

			bool ExtractFallBackString(unsigned int flags)
			{
				if (vml::utils::bits32::Get(flags, OPENGL_4_0)) { vml::utils::bits32::SetToTrue(Flags, OPENGL_4_0); GlWindowHint(4, 0); GlFallBackVersion = OPENGL_4_0; return true; }
				if (vml::utils::bits32::Get(flags, OPENGL_4_1)) { vml::utils::bits32::SetToTrue(Flags, OPENGL_4_1); GlWindowHint(4, 1);	GlFallBackVersion = OPENGL_4_1;	return true; }
				if (vml::utils::bits32::Get(flags, OPENGL_4_2))	{ vml::utils::bits32::SetToTrue(Flags, OPENGL_4_2); GlWindowHint(4, 2); GlFallBackVersion = OPENGL_4_2;	return true; }
				if (vml::utils::bits32::Get(flags, OPENGL_4_3))	{ vml::utils::bits32::SetToTrue(Flags, OPENGL_4_3); GlWindowHint(4, 3);	GlFallBackVersion = OPENGL_4_3;	return true; }
				if (vml::utils::bits32::Get(flags, OPENGL_4_4)) { vml::utils::bits32::SetToTrue(Flags, OPENGL_4_4); GlWindowHint(4, 4);	GlFallBackVersion = OPENGL_4_4;	return true; }
				if (vml::utils::bits32::Get(flags, OPENGL_4_5))	{ vml::utils::bits32::SetToTrue(Flags, OPENGL_4_5); GlWindowHint(4, 5);	GlFallBackVersion = OPENGL_4_5;	return true; }
				if (vml::utils::bits32::Get(flags, OPENGL_4_6)) { vml::utils::bits32::SetToTrue(Flags, OPENGL_4_6); GlWindowHint(4, 6);	GlFallBackVersion = OPENGL_4_6;	return true; }
				return false;
			}
			
			// ---------------------------------------------------------------------------
			// checks opengl errors

			void CheckGLError(const std::string &tag)
			{
				std::string text;

				switch (glGetError())
				{
					case GL_NO_ERROR:																						break;
					case GL_INVALID_ENUM:					text = tag + " : GL error GL_INVALID_ENUM";						break;
					case GL_INVALID_VALUE:					text = tag + " : GL error GL_INVALID_VALUE ";					break;
					case GL_INVALID_OPERATION:				text = tag + " : GL error GL_INVALID_OPERATION";				break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:	text = tag + " : GL error GL_INVALID_FRAMEBUFFER_OPERATION";	break;
					case GL_OUT_OF_MEMORY:					text = tag + " : GL error GL_OUT_OF_MEMORY";					break;
					case GL_STACK_OVERFLOW:					text = tag + " : GL_error GL_STACK_OVERFLOW";					break;
					case GL_STACK_UNDERFLOW:				text = tag + " : GL_error GL_STACK_UNDERFLOW";					break;
				}

				vml::os::Message::Error("GlWindow : ", text.c_str());
			}
			
			// ---------------------------------------------------------------------------

			void GlWindowHint(unsigned int major, unsigned int minor)
			{
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , major);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , minor);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT , GL_TRUE);
				glfwWindowHint(GLFW_OPENGL_PROFILE		  , GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT , GL_FALSE);
			}

		public:
			
			// ---------------------------------------------------------------------------
			// preferences flags

			static constexpr unsigned int	WINDOWED   = vml::utils::bits32::BIT0;
			static constexpr unsigned int	FULLSCREEN = vml::utils::bits32::BIT1;
			static constexpr unsigned int	HIDECURSOR = vml::utils::bits32::BIT2;
			static constexpr unsigned int	MAXIMIZED  = vml::utils::bits32::BIT3;
			static constexpr unsigned int	LOCKSWAP   = vml::utils::bits32::BIT4;
			static constexpr unsigned int	EXITONESC  = vml::utils::bits32::BIT5;

			// ---------------------------------------------------------------------------
			// opengl preferences flags

			static constexpr unsigned int	OPENGL_4_0 = vml::utils::bits32::BIT11;
			static constexpr unsigned int	OPENGL_4_1 = vml::utils::bits32::BIT12;
			static constexpr unsigned int	OPENGL_4_2 = vml::utils::bits32::BIT13;
			static constexpr unsigned int	OPENGL_4_3 = vml::utils::bits32::BIT14;
			static constexpr unsigned int	OPENGL_4_4 = vml::utils::bits32::BIT15;
			static constexpr unsigned int	OPENGL_4_5 = vml::utils::bits32::BIT16;
			static constexpr unsigned int	OPENGL_4_6 = vml::utils::bits32::BIT17;
			
			// ---------------------------------------------------------------------------
			// This initializes our window and creates the OpenGL context

			bool Init(int width, int height, const std::string &strTitle, unsigned int flags)
			{
				
				// get flags

				Flags = flags;
				
				// sts window's dimensions

				ScreenWidth  = width;
				ScreenHeight = height;

				// This tries to first init the GLFW library and make sure it is available

				if (!glfwInit())
				{
					glfwTerminate();
					vml::os::Message::Error("GlWindow : ", "cannot init glfw library");
				}
				
				// gets fallback string for opengl versioning
				
				if (!ExtractFallBackString(Flags))
				{
					glfwTerminate();
					vml::os::Message::Error("GlWindow : ", "gl fallback version error, minimum version is 4.0");
				}

				//		vml::os::Message::Trace(L"Opengl version supported : %s\n", vml::strings::StringConverter::FromStringToWString(GetGlFallBackVersionString()).c_str());
						
				// check if window is at fullscreen or not
				
				bool windowed   = vml::utils::bits32::Get(Flags, WINDOWED);
				bool fullscreen = vml::utils::bits32::Get(Flags, FULLSCREEN);

				if ((windowed && fullscreen) || (!windowed && !fullscreen))
				{
					glfwTerminate();
					vml::os::Message::Error("GlWindow : ", "couldn't set up screen mode choose fullscreen or windowed");
				}
				
				// exit on esc pressed ?
				
				ExitOnEsc = vml::utils::bits32::Get(Flags, EXITONESC);

				// Create a window either in full screen 

				if (fullscreen && !windowed)
				{
					const GLFWvidmode *mode = glfwGetVideoMode( glfwGetPrimaryMonitor());

					glfwWindowHint(GLFW_RED_BITS     , mode->redBits);
					glfwWindowHint(GLFW_GREEN_BITS   , mode->greenBits);
					glfwWindowHint(GLFW_BLUE_BITS    , mode->blueBits);
					glfwWindowHint(GLFW_REFRESH_RATE , mode->refreshRate);

					Window  = glfwCreateWindow(mode->width, mode->height, strTitle.c_str(),  glfwGetPrimaryMonitor(), nullptr);

					// exit with error if context cannot be created

					if (!Window)
					{
						glfwTerminate();
						vml::os::Message::Error("GlWindow : ", "Can't init glfw window context");
					}
				}
				
				// create a windowed window

				if (windowed && !fullscreen)
				{
					Window = glfwCreateWindow(ScreenWidth, ScreenHeight, strTitle.c_str(), nullptr, nullptr);

					// exit with error if context cannot be created

					if (!Window)
					{
						glfwTerminate();
						vml::os::Message::Error("GlWindow : ", "Can't init glfw window context");
					}
					
					// windows specific
					// window is maximized only if mode is windowed

					if (vml::utils::bits32::Get(Flags, MAXIMIZED))
						ShowWindow(glfwGetWin32Window(Window), SW_SHOWMAXIMIZED);

					// gets new window extents

					glfwGetFramebufferSize(Window, &ScreenWidth, &ScreenHeight);
				}

				// Create the OpenGL context from the window and settings specified

				glfwMakeContextCurrent(Window);

				// get version info

				RendererString = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
				VersionString  = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

				// This turns off the vertical sync to your monitor so it renders as fast as possible

				bool lockswap = vml::utils::bits32::Get(Flags, LOCKSWAP);

				if ( lockswap )
					glfwSwapInterval(1);
				else
					glfwSwapInterval(0);

				// This turns on STICKY_KEYS for keyboard input

				glfwSetInputMode(Window, GLFW_STICKY_KEYS, GLFW_TRUE);
				glfwSetInputMode(Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

				// We want to hide the mouse since it will be used to move the camera's view around
				// and don't want to see it being pushed up into the top left corner.

				if (vml::utils::bits32::Get(Flags, HIDECURSOR))
					glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				// Initialize the GLEW library and attach all the OpenGL functions and extensions
				// Tell GLEW to grab all the OpenGL functions and extensions even if "experimental"

				glewExperimental = GL_TRUE;

				GLenum err = glewInit();

				// If we had an error, return -1.  Be sure to see if glewExperimental isn't true, this worked for me.

				if (err!=GLEW_OK)
				{
					glfwTerminate();
					vml::os::Message::Error("GlWindow : ", "Failed to initialize glew");
				}
			
				vml::utils::Logger::GetInstance()->Info("OpenGl Context : Initting " + RendererString + " " + VersionString + " : Done");
				
				return true;
			}
			
			// ---------------------------------------------------------------------------
			// This function processes all the application's input 
			// and returns a bool to tell us if we should continue

			void InitEvents()
			{
				
				// Use the GLFW function to check for the user pressing the Escape button, as well as a window close event.
				
				if (ExitOnEsc)
				{
					if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(Window) != 0)
						Running = false;
				}
				else
				{
					if ( glfwWindowShouldClose(Window) != 0)
						Running = false;
				}

				// detect if window resizes and changes screen dimensions accordingly

				glfwGetFramebufferSize(Window, &ScreenWidth, &ScreenHeight);

				// Grab the current mouse position from our window

				glfwGetCursorPos(Window, &MouseX, &MouseY);

				// compute mouse deltas

				MouseXDelta -= MouseX;
				MouseYDelta -= MouseY;

				LeftButtonState   = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_1);
				RightButtonState  = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_2);
				MiddleButtonState = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_3);
				
			}

			// ---------------------------------------------------------------------------

			void SetWindowTitle(const std::string &str)
			{
				if (vml::utils::bits32::Get(Flags, WINDOWED))
					glfwSetWindowTitle(Window,str.c_str());
			}
			
			// ---------------------------------------------------------------------------

			void SwapBuffers()
			{
				glfwSwapBuffers(Window);
			}
			
			// ---------------------------------------------------------------------------

			void PollEvents()
			{
				// reset mouse deltas for the next cycle

				MouseXDelta = MouseX;
				MouseYDelta = MouseY;

				// Poll the input events to see if the user quit or closes the window

				glfwPollEvents();
			}
			
			// ---------------------------------------------------------------------------
			// checks opengl errors

			const std::string GetGlFallBackVersionString()
			{
				switch (GlFallBackVersion)
				{
					case OPENGL_4_0: return "version40"; break;
					case OPENGL_4_1: return "version41"; break;
					case OPENGL_4_2: return "version42"; break;
					case OPENGL_4_3: return "version43"; break;
					case OPENGL_4_4: return "version44"; break;
					case OPENGL_4_5: return "version45"; break;
					case OPENGL_4_6: return "version46"; break;
				}

				// we never get here

				return "No versioning";
			}
			
			//------------------------------------------------------------------------------
			// getters
 
			bool		       IsFullscreen()		  const { return vml::utils::bits32::Get(Flags, FULLSCREEN); }
			bool		       IsWindowed()			  const { return vml::utils::bits32::Get(Flags, WINDOWED); }
			bool	   	       IsMaxmized()			  const { return vml::utils::bits32::Get(Flags, MAXIMIZED); }
			bool		       IsMouseVisible()		  const { return vml::utils::bits32::Get(Flags, HIDECURSOR); }
			unsigned int       GetFlags()			  const { return Flags; }
			int			       GetScreenWidth()		  const { return ScreenWidth; }
			int			       GetScreenHeight()	  const { return ScreenHeight; }
			double		       GetMouseX()			  const { return MouseX; }
			double		       GetMouseY()			  const { return MouseY; }
			double		       GetMouseXDelta()       const { return MouseXDelta; };
			double		       GetMouseYDelta()       const { return MouseYDelta; }
			int			       GetLeftButtonState()   const { return LeftButtonState; }
			int			       GetRightButtonState()  const { return RightButtonState;}
			int			       GetMiddleButtonState() const { return MiddleButtonState; }
			bool		       IsRunning()		      const { return Running; }
			GLFWwindow		  *GetWindow()            const { return Window; }
			const std::string &GetRenderString()      const { return RendererString; }
			const std::string &GetVersionString()     const { return VersionString; }
			int				   GetFallBackVersion()   const { return GlFallBackVersion; }
			
			// -----------------------------------------------------------------------
			// copy constructor is private
			// no copies allowed since classes
			// are referenced

			OpenGLContextWindow(const OpenGLContextWindow& glwindow) = delete;

			// -----------------------------------------------------------------------
			// overload operator = is private
			// no copies allowed since classes
			// are referenced

			OpenGLContextWindow& operator = (const OpenGLContextWindow& glwindow) = delete;

			//------------------------------------------------------------------------------
			// ctor / dtor

			OpenGLContextWindow()
			{
				Flags				= 0;
				Window			    = nullptr;
				ScreenWidth		    = 0;
				ScreenHeight	    = 0;
				RightButtonState    = 0;
				LeftButtonState     = 0;
				MiddleButtonState	= 0;
				MouseX		 	    = 0;
				MouseY			    = 0;
				MouseXDelta		    = 0;
				MouseYDelta		    = 0;
				Running			    = true;
				ExitOnEsc			= false;
				GlFallBackVersion   = OPENGL_4_0;
			}

			~OpenGLContextWindow()
			{
				glfwDestroyWindow(Window);
				glfwTerminate();
				vml::utils::Logger::GetInstance()->Info("Opengl Context : Closing : Done");
			}
			
			
			
	};
	
} // end of vml namespace


