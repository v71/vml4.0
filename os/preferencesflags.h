#pragma once

/////////////////////////////////////////////////////////////////////
// internal flags

namespace vml
{
	namespace utils
	{
		struct PreferencesFlags
		{

			// bit flags

			const static uint32_t VERBOSE		  = vml::utils::bits32::BIT0;
			const static uint32_t QUIET			  = vml::utils::bits32::BIT1;

			const static uint32_t LOG_TO_STDOUT   = vml::utils::bits32::BIT29;
			const static uint32_t LOG_TO_MEMORY	  = vml::utils::bits32::BIT30;
			const static uint32_t LOG_TO_FILE     = vml::utils::bits32::BIT31;

			// get bit desciptors

			static const std::string ToString(const unsigned int bitvalue)
			{
				switch (bitvalue)
				{
					case VERBOSE       : return "Verbose";		 break;
					case QUIET         : return "Quiet";		 break;
					case LOG_TO_STDOUT : return "Log to stdout"; break;
					case LOG_TO_MEMORY : return "Log to memory"; break;
					case LOG_TO_FILE   : return "Log to File";   break;
				}
				return "PreferencesFlags : Undefined BitValue";
			}
		};

	} // end of os namespace

} // end of vml namespace
