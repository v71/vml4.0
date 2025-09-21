#pragma once

/////////////////////////////////////////////////////////////////////
// internal flags

namespace vml
{
	namespace utils
	{
		struct InternalFlags
		{

			// bit flags

			const static uint32_t INITTED   = vml::utils::bits32::BIT0;
			const static uint32_t FINALIZED = vml::utils::bits32::BIT1;

			// get bit desciptors

			static const std::string ToString(const uint32_t bitvalue)
			{
				switch (bitvalue)
				{
					case INITTED:            return "Initted";		break;
					case FINALIZED:          return "Finalized";	break;
				}
				return "Internal flags : Undefined BitValue";
			}
		};
	}
}
