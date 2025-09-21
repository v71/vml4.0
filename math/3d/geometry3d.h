#pragma once

namespace vml
{
	namespace geo3d
	{

		struct Results
		{

			// intersections flags

			static const uint32_t DOES_INTERSECT	  = vml::utils::bits32::BIT0;
			static const uint32_t DOES_NOT_INTERSECT  = vml::utils::bits32::BIT1;
			static const uint32_t COLLINEAR			  = vml::utils::bits32::BIT2;
			static const uint32_t PARALLEL			  = vml::utils::bits32::BIT3;

			// containment flags

			static const uint32_t INSIDE			  = vml::utils::bits32::BIT4;
			static const uint32_t OUTSIDE			  = vml::utils::bits32::BIT5;
			static const uint32_t SECOND_INSIDE_FIRST = vml::utils::bits32::BIT6;
			static const uint32_t FIRST_INSIDE_SECOND = vml::utils::bits32::BIT7;
		
			// get bit desciptors

			static const std::string ToString(const unsigned int bitvalue)
			{
				switch (bitvalue)
				{

					case DOES_NOT_INTERSECT  : return "Do Not Intersect";	 break;
					case DOES_INTERSECT      : return "Do Intersect";		 break;
					case COLLINEAR		     : return "Collinear";			 break;
					case PARALLEL		     : return "Parallel";			 break;
					case INSIDE			     : return "Inside";				 break;
					case OUTSIDE			 : return "Outside";			 break;
					case SECOND_INSIDE_FIRST : return "Second Inside First"; break;
					case FIRST_INSIDE_SECOND : return "First Inside Second"; break;

					case vml::utils::bits32::BIT8 : return "Nodesc08";			    break;
					case vml::utils::bits32::BIT9 : return "NoDesc09";		        break;
					case vml::utils::bits32::BIT10: return "NoDesc10";			    break;
					case vml::utils::bits32::BIT11: return "NoDesc11";				break;
					case vml::utils::bits32::BIT12: return "NoDesc12";				break;
					case vml::utils::bits32::BIT13: return "NoDesc13";			    break;
					case vml::utils::bits32::BIT14: return "NoDesc14";		        break;
					case vml::utils::bits32::BIT15: return "NoDesc15";				break;
					case vml::utils::bits32::BIT16: return "NoDesc16";				break;
					case vml::utils::bits32::BIT17: return "NoDesc17";				break;
					case vml::utils::bits32::BIT18: return "NoDesc18";				break;
					case vml::utils::bits32::BIT19: return "NoDesc19";				break;
					case vml::utils::bits32::BIT20: return "NoDesc20";				break;
					case vml::utils::bits32::BIT21: return "NoDesc21";		        break;
					case vml::utils::bits32::BIT22: return "NoDesc22";		        break;
					case vml::utils::bits32::BIT23: return "NoDesc23";		        break;
					case vml::utils::bits32::BIT24: return "NoDesc24";			    break;
					case vml::utils::bits32::BIT25: return "NoDesc25";			    break;
					case vml::utils::bits32::BIT26: return "NoDesc26";			    break;
					case vml::utils::bits32::BIT27: return "NoDesc27";			    break;
					case vml::utils::bits32::BIT28: return "NoDesc28";			    break;
					case vml::utils::bits32::BIT29: return "NoDesc29";			    break;
					case vml::utils::bits32::BIT30: return "NoDesc30";			    break;
					case vml::utils::bits32::BIT31: return "NoDesc31";			    break;

				}

				return "Undefined BitValue";
			}

		};

	} // end of geo2d namespace

} // end of vml namepsace

///////////////////////////////////////////////////////////////////////////////////////
// 3d geometry predicsates

#include <vml4.0/math/3d/vec3.h>
#include <vml4.0/math/3d/predicates3d.h>
#include <vml4.0/math/3d/metrics.h>
#include <vml4.0/math/3d/distances.h>
#include <vml4.0/math/3d/intersections.h>
#include <vml4.0/math/3d/clippers.h>
#include <vml4.0/math/3d/collisions.h>



