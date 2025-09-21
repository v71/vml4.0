#pragma once

////////////////////////////////////////////////////////////////////////////////////
// Intersection functions for plane class

namespace vml
{
	namespace geo3d
	{
		namespace intersections
		{
			/*
			static const unsigned int LINE_DO_NOT_INTERSECTS_PLANE = 0;
			static const unsigned int LINE_DO_INTERSECTS_PLANE		   = 1;

			////////////////////////////////////////////////////////////////////////////////////
			// intersection beetween plane and ray returns point of intersection

			static glm::vec3 RayVsPlane(const glm::vec3 &p0, const glm::vec3 &normal, 
										const glm::vec3 &a, const glm::vec3 &b,
										float eps=vml::math::EPSILON)
			{
				glm::vec3 d = b - a;
				float denum = glm::dot(normal, d);
				if (denum > -eps && denum < eps)
					denum = eps;
				float intr = glm::dot(normal, p0 - a);
				intr /= denum;
				return a + intr * d;
			}

			////////////////////////////////////////////////////////////////////////////////
			// intersection beetween plane and line returns point of intersection

			static int LineVsPlane(const glm::vec3 &a, const glm::vec3 &b,
								   const glm::vec3 &p0, const glm::vec3 &n,
								   glm::vec3 &q,
								   float eps = vml::math::EPSILON)
			{
				glm::vec3 d = b - a;
				float denum = glm::dot(n, d);
				if (denum > -eps && denum < eps)
					denum = eps;
				float Intr = glm::dot(n, p0 - a);
				Intr /= denum;
				if (Intr < -eps || Intr > 1.0f + eps) 
					return LINE_DO_NOT_INTERSECTS_PLANE;
				q = a + Intr * d;
				return  LINE_DO_INTERSECTS_PLANE;
			}
			*/
		} // end of namespace planes

	} // end of namespace goe3d

}	// end of vml namespace


