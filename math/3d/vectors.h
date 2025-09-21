#pragma once

/////////////////////////////////////////////////////////////////////////////
// flags for spheres

namespace vml
{
	namespace geo3d
	{
		namespace vectors
		{

			/////////////////////////////////////////////////////////////////////////////////////////
			// quaternion orientation given two point in space
			// build rotation matrix

			static const glm::quat RotationBetweenVectors(const glm::vec3 &s, const glm::vec3 &d)
			{
				glm::vec3 start(s), dest(d);

				start = glm::normalize(start);
				dest = glm::normalize(dest);

				float cosTheta = glm::dot(start, dest);

				glm::vec3 rotationAxis;

				if (cosTheta < -1 + 0.001f)
				{
					rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);

					if (glm::length(rotationAxis) < 0.01f)
						rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

					rotationAxis = glm::normalize(rotationAxis);

					return glm::angleAxis(180.0f, rotationAxis);
				}

				rotationAxis = glm::cross(start, dest);

				float sq = sqrt((1 + cosTheta) * 2);
				float invs = 1.0f / sq;

				return glm::quat(sq * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
			}

			////////////////////////////////////////////////
			// Get angle described by two vectors
			// the routine always computes the SMALLER of the two angles between
			// two vectors.Thus, if the vectors make an(exterior) angle of 200
			// degrees, the(interior) angle of 160 is reported.

			static const float GetVectorAngle(const glm::vec3 &v1, const glm::vec3 &v2)
			{
				float cos = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
				float v1sqr = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z); //length of v1
				float v2sqr = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z); //length of v2
				cos /= v1sqr * v2sqr;
				if (cos < -1.0f) cos = -1.0f;
				else if (cos > 1.0f) cos = 1.0f;
				return acos(cos);
			}

			////////////////////////////////////////////////
			// Compute an orthonormal basis given a vector

			static glm::mat3 CalculateContactBasis(const glm::vec3 &contactNormal)
			{
				glm::vec3 contactTangent[2];

				// Check whether the Z axis is nearer to the X or Y axis.

				if (fabs(contactNormal.x) > fabs(contactNormal.y))
				{
					// Scaling factor to ensure the results are normalized.
					const float s = (float)1.0f / sqrtf(contactNormal.z*contactNormal.z + contactNormal.x*contactNormal.x);
					// The new X axis is at right angles to the world Y axis.
					contactTangent[0].x = contactNormal.z*s;
					contactTangent[0].y = 0;
					contactTangent[0].z = -contactNormal.x*s;
					// The new Y axis is at right angles to the new X and Z axes.
					contactTangent[1].x =  contactNormal.y*contactTangent[0].z;
					contactTangent[1].y =  contactNormal.z*contactTangent[0].x - contactNormal.x*contactTangent[0].z;
					contactTangent[1].z = -contactNormal.y*contactTangent[0].x;
				}
				
				else
				{
					// Scaling factor to ensure the results are normalized.
					const float s = (float)1.0f / sqrtf(contactNormal.z*contactNormal.z + contactNormal.y*contactNormal.y);
					// The new X axis is at right angles to the world X axis.
					contactTangent[0].x = 0;
					contactTangent[0].y = -contactNormal.z*s;
					contactTangent[0].z =  contactNormal.y*s;
					// The new Y axis is at right angles to the new X and Z axes.
					contactTangent[1].x =  contactNormal.y*contactTangent[0].z - contactNormal.z*contactTangent[0].y;
					contactTangent[1].y = -contactNormal.x*contactTangent[0].z;
					contactTangent[1].z =  contactNormal.x*contactTangent[0].y;


				}
				
				// Make a matrix from the three vectors.

				return glm::mat3(contactNormal.x, contactNormal.y, contactNormal.z,
								 contactTangent[0].x, contactTangent[0].y, contactTangent[0].z,
								 contactTangent[1].x, contactTangent[1].y, contactTangent[1].z);
			}

		} // end of vectors namespace

	}	// end of geo3d namespace

}	// end of vml namespace
