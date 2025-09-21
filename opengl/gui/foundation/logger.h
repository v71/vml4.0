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

#include <vml4.0/opengl/gui/foundation/common.h>

namespace vml
{
	namespace utils
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////
		/// Log line message

		class LogMessage
		{
			public:

				// ----------------------------------------------------------------------------------
				// log level flags

				enum class LogLevel : uint8_t
				{
					LEVEL_INFO    = 0,
					LEVEL_WARNING = 1,
					LEVEL_ERROR   = 2
				};

			private:

				std::string  FileNamePos;
				std::string  LinePos;
				size_t       LogNr;
				std::string  Text;
				std::string  TimeStamp;
				std::string  DateString;
				LogLevel     Level;
				bool		 Colored;
				
				// ---------------------------------------------------------------------------------
				// Format text for log string, e.g. [1] [2024-04-04 04:04:04.040] [W] A message to log [LogMessage.h:41].

				[[nodiscard]] std::string GetFormattedColoredLine() const
				{
					std::string color;
					std::string message = "[ " + std::to_string(LogNr) + " ] [ " + DateString + " ] ";
					if (Level == LogLevel::LEVEL_INFO)
					{
						color = "\033[39m";
						message += "[I] ";
					}
					else if (Level == LogLevel::LEVEL_WARNING)
					{
						color = "\033[33m";
						message += "[W] ";
					}
					else if (Level == LogLevel::LEVEL_ERROR)
					{
						color = "\033[31m";
						message += "[E] ";
					}
					message += Text + " [ " + FileNamePos + " ] [ " + LinePos + " ]";
					return color + message + "\033[39m";
				}

				// ---------------------------------------------------------------------------------
				// Format text for log string, e.g. [1] [2024-04-04 04:04:04.040] [W] A message to log [LogMessage.h:41].

				[[nodiscard]] std::string GetFormattedUncoloredLine() const
				{
					std::string message = "[ " + std::to_string(LogNr) + " ] [ " + DateString + " ] ";
					if (Level == LogLevel::LEVEL_INFO)
						message += "[I] ";
					else if (Level == LogLevel::LEVEL_WARNING)
						message += "[W] ";
					else if (Level == LogLevel::LEVEL_ERROR)
						message += "[E] ";
					message += Text + " [ " + FileNamePos + " ] [ " + LinePos += " ]";
					return message;
				}

				// ---------------------------------------------------------------------------------
				// formatted string holding timing

				[[nodiscard]] const std::string GetFormatTime() const
				{
					const std::chrono::time_point<std::chrono::system_clock>& time = std::chrono::system_clock::now();
					const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(time);
					const auto fraction = time - seconds;
					std::tm t{};
					const std::time_t timer = std::chrono::system_clock::to_time_t(time);
					#if defined(__unix__)
						localtime_r(&timer, &t);
					#elif defined(_MSC_VER)
						localtime_s(&t, &timer);
					#else
						static_assert(false)
					#endif
						const long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(fraction).count();
					if (t.tm_hour == 1 && t.tm_min == 0 && t.tm_sec == 0 && millis == 0)
						return "Not available";
					const std::string hour = t.tm_hour > 9 ? std::to_string(t.tm_hour) : "0" + std::to_string(t.tm_hour);
					const std::string min = t.tm_min > 9 ? std::to_string(t.tm_min) : "0" + std::to_string(t.tm_min);
					const std::string sec = t.tm_sec > 9 ? std::to_string(t.tm_sec) : "0" + std::to_string(t.tm_sec);
					std::string millisecond;
					if (millis >= 0 && millis < 10) {
						millisecond = "00" + std::to_string(millis);
					}
					else if (millis > 9 && millis < 100) {
						millisecond = "0" + std::to_string(millis);
					}
					else {
						millisecond = std::to_string(millis);
					}
					const int mon = t.tm_mon + 1;
					const std::string year = std::to_string(t.tm_year + 1900);
					const std::string month = mon > 9 ? std::to_string(mon) : "0" + std::to_string(mon);
					const std::string day = t.tm_mday > 9 ? std::to_string(t.tm_mday) : "0" + std::to_string(t.tm_mday);
					return year + "-" + month + "-" + day + " " + hour + ":" + min + ":" + sec + ":" + millisecond;
				}

			public:

				// ---------------------------------------------------------------------------------
				// getters

				[[nodiscard]] const LogLevel GetLevel() const noexcept { return Level; }
				[[nodiscard]] const std::string& GetFileNamePos() const noexcept { return FileNamePos; }
				[[nodiscard]] const std::string& GetText() const noexcept { return Text; }
				[[nodiscard]] const std::string& GetLinePos() const noexcept { return LinePos; }
				[[nodiscard]] const std::string& GetDateString() const noexcept { return DateString; }
				[[nodiscard]] size_t GetLogNr() const noexcept { return LogNr; }

				// ---------------------------------------------------------------------------------
				// Format text for log string, e.g. [1] [2024-04-04 04:04:04.040] [W] A message to log [LogMessage.h:41].

				[[nodiscard]] std::string GetFormattedLine() 
				{
					if (Colored) {
						return GetFormattedColoredLine();
					}
					return GetFormattedUncoloredLine();
				}

				// ---------------------------------------------------------------------------------
				// ctor / dtor

				LogMessage(const LogLevel level,
							const std::string& filenamepos,
							const size_t linepos,
							const std::string& text,
							const size_t logNr,
							bool color)
				{
					Level       = level;
					FileNamePos = filenamepos;
					LinePos     = std::to_string(linepos);
					LogNr       = logNr;
					Text        = text;
					Colored     = color;
					DateString = GetFormatTime();
				}

				~LogMessage()
				{
				}
		};

		///////////////////////////////////////////////////////////////////////////////////////
		// Thread safe logger

		class Logger
		{
			public:
		
				// log mode flags

				enum class LogMode : uint8_t
				{
					TO_STD  = 0,
					TO_MEM  = 1,
					TO_FILE = 2
				};

			private:
				
				// ---------------------------------------------
				// singleton design pattern
				// A singlton is a class instantiated only once.
				// Usually it is globally accessiblae,
				// constructor and destructors are kept private to the 
				// final user. User has the responsability to delete
				// memory associated with the class explicitly at
				// main entry point closure
				
				static Logger*		   Singleton;		  // Singleton pointer
				std::string			   FileName;		  // string holding log's filename
				std::ofstream		   FileStream;		  // file stream
				bool				   Initted;			  // is logger initted 
				std::deque<LogMessage> History;           // deque for messages
				size_t                 Lines;             // numer of log lines
				LogMode 	           Mode;              // preferences flags 
				std::mutex             Lock;              // mutex lock
				uint32_t               MaxHistory;        // maximum number of lines
				bool				   Colored;

				// ---------------------------------------------
				// private ctor / dtor

				Logger()
				{
					Singleton     = nullptr;
					FileName      = "";
					Initted		  = false;
					Mode		  = LogMode::TO_STD;
					Lines		  = 0;
					MaxHistory	  = 65536;
					Colored		  = false;
				}

				// ---------------------------------------------------------------------------------

				void WriteToStd(const LogMessage::LogLevel lvl, const std::string& message, const std::string& filename, const int linepos)
				{
					LogMessage logmsg(lvl, filename, linepos, message, Lines,Colored);
					std::cout << logmsg.GetFormattedLine() << std::endl;
					Lines++;
				}

				// ---------------------------------------------------------------------------------

				void WriteToMem(const LogMessage::LogLevel lvl, const std::string& message, const std::string& filename, const int linepos)
				{
					Colored = false;
					LogMessage logmsg(lvl, filename, linepos, message, Lines, Colored);
					Lock.lock();
					History.push_front(logmsg);
					if (History.size() > MaxHistory)
						History.pop_back();
					Lock.unlock();
					Lines++;
				}

				// ---------------------------------------------------------------------------------

				void WriteToFile(const LogMessage::LogLevel lvl, const std::string& message, const std::string& filename, const int linepos)
				{
					Colored = false;
					LogMessage logmsg(lvl, filename, linepos, message, Lines, Colored);
					Lock.lock();
					FileStream << logmsg.GetFormattedLine() << "\n";
					FileStream.flush();
					Lock.unlock();
					Lines++;
				}

				// ---------------------------------------------------------------------------------

				void Log(const LogMessage::LogLevel lvl, const std::string& message, const std::string& filename = __builtin_FILE(), const int linepos = __builtin_LINE())
				{
					if (!IsInitialized())
						vml::os::Message::Error("Logger : Logger not initted");

					switch (Mode)
					{
						case LogMode::TO_STD  : WriteToStd(lvl, message, filename, linepos);  break;
						case LogMode::TO_MEM  : WriteToMem(lvl, message, filename, linepos);  break;
						case LogMode::TO_FILE : WriteToFile(lvl, message, filename, linepos); break;
					}
				}

			public:

				//---------------------------------------------------------------------
				// copy constructor is private
				// no copies allowed since classes
				// are referenced

				Logger(Logger& other) = delete;

				//---------------------------------------------------------------------
				// overload operator is private, 
				// no copies allowed since classes
				// are referenced

				Logger operator=(const Logger&) = delete;

				//---------------------------------------------------------------------
				// get instance of singleton , if singleton is null , which is mandatory at
				// application startup, it will be allocated only once
				// Eventually, to reallocate , user must call the delete member function

				static Logger* GetInstance()
				{
					if (Singleton == nullptr)
						Singleton = new Logger();
					return Singleton;
				}

				//---------------------------------------------------------------------

				void Init(LogMode logmodeflags, const std::string& filename = "")
				{
					if (IsInitialized())
						vml::os::Message::Error("Logger : Logger already initted, delete and restart");

					Initted = false;
					Lines	= 0;
					Mode	= logmodeflags;

					switch (Mode)
					{
						case LogMode::TO_STD:
							if (!filename.empty())
								vml::os::Message::Error("Logger : Logger is set to std out, but you provided a filename");
						break;

						case LogMode::TO_MEM:
							if (!filename.empty())
								vml::os::Message::Error("Logger : Logger is set to mem, but you provided a filename");
						break;

						case LogMode::TO_FILE:
						{
							if (filename.empty())
								vml::os::Message::Error("Logger : Logger is set to file but FileName is null");
							if (filename.length() > 255)
								vml::os::Message::Error("Logger : Logger is set to file but FileName length is greater than 255");
							// check for valid file format
							std::regex filenamePattern(R"(^[a-zA-Z0-9_\-\.]+$)");
							if (!std::regex_match(filename, filenamePattern))
								vml::os::Message::Error("Logger : Logger is set to file but FileName has invalid format");
							FileName = filename;
							// open file stream
							FileStream.open(filename.c_str(), std::ios::out);
							// check file stream validity
							if (!FileStream.is_open())
								vml::os::Message::Error("Logger : Cannot open file ", filename.c_str());
						}
						break;

						default:
							vml::os::Message::Error("Logger : Unknown logger mode");
						break;
					}

					// set initialized flag to true
					Initted = true;

				}

				// ---------------------------------------------------------------------------------

				void SetMaxHistory(const uint32_t maxhistory)
				{
					if (!IsInitialized())
						vml::os::Message::Error("Logger : Logger not initted");
					if (maxhistory < 16 || maxhistory < 65536)
						vml::os::Message::Error("Logger : Invalid MaxHistory value");
					MaxHistory = maxhistory;
				}

				// ---------------------------------------------------------------------------------

				void SetColored( bool val)
				{
					Colored = val;
				}

				// ---------------------------------------------------------------------------------

				const std::deque<LogMessage>& GetHistory()
				{
					if (!IsInitialized())
						vml::os::Message::Error("Logger : Logger not initted");
					if (Mode != LogMode::TO_MEM)
						vml::os::Message::Error("Logger : Logger is not set to log to mem");
					return History;
				}

				// ---------------------------------------------------------------------------------

				const std::string GetLogModeString() const
				{
					switch (Mode)
					{
						case LogMode::TO_STD: return"Log to Stdout"; break;
						case LogMode::TO_FILE: return"Log to File";   break;
						case LogMode::TO_MEM: return"Log to Memory"; break;
					}
					return "unknown log mode";
				}

				// ---------------------------------------------------------------------------------

				size_t GetLinesCount() const
				{
					return Lines;
				}

				// ---------------------------------------------------------------------------------

				LogMode GetLogMode() const
				{
					return Mode;
				}

				// ---------------------------------------------------------------------------------

				const std::string& GetFileName() const
				{
					return FileName;
				}

				// ---------------------------------------------------------------------------------
				// returns true if logger is initialized  

				bool IsInitialized() const
				{
					return Initted;
				}

				// ---------------------------------------------------------------------------------
				
				bool IsColored() const
				{
					return Colored;
				}

				// ---------------------------------------------------------------------------------

				void Info(const std::string& message, const std::string& file = __builtin_FILE(), const int line = __builtin_LINE())
				{
					Log(LogMessage::LogLevel::LEVEL_INFO, message, file, line);
				}

				// ---------------------------------------------------------------------------------

				void Warning(const std::string& message, const std::string& file = __builtin_FILE(), const int line = __builtin_LINE())
				{
					Log(LogMessage::LogLevel::LEVEL_WARNING, message, file, line);
				}

				// ---------------------------------------------------------------------------------

				void Error(const std::string& message, const std::string& file = __builtin_FILE(), const int line = __builtin_LINE())
				{
					Log(LogMessage::LogLevel::LEVEL_ERROR, message, file, line);
				}

				// ------------------------------------------------------------
				// closes stream

				void Close()
				{
					if (!IsInitialized())
						vml::os::Message::Error("Logger : Logger not initted");
					
					// clear member data

					Initted		  = false;
					Mode		  = LogMode::TO_STD;
					Lines		  = 0;
					FileName	  = "";
					MaxHistory	  = 65536;
					Colored		  = false;

					// close file stream
					if (Mode == LogMode::TO_FILE)
						FileStream.close();

					// delete singleton
					vml::os::SafeDelete(Singleton);
				}

				// ------------------------------------------------------------
				// ctor / dtor

				~Logger() {};

		};

	}
}
