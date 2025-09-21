#pragma once

/////////////////////////////////////////////////////////////////////////////
// flags for spheres

namespace vml
{
	namespace geo3d
	{

		class Spline
		{

			private:
				
				// -----------------------------------------------
				//
				
				std::vector<glm::vec3> ControlPoints;
				float				   DeltaT;

				// -----------------------------------------------
				// Static method for computing the Catmull-Rom parametric equation
				// Solve the Catmull-Rom parametric equation for a given time(t) and vector quadruple (p1,p2,p3,p4)
				// given a time (t) and a vector quadruple (p1,p2,p3,p4).

				glm::vec3 Eq(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
				{
					float t2 = t * t;
					float t3 = t2 * t;
					float b1 = .5 * (-t3 + 2 * t2 - t);
					float b2 = .5 * (3 * t3 - 5 * t2 + 2);
					float b3 = .5 * (-3 * t3 + 4 * t2 + t);
					float b4 = .5 * (t3 - t2);
					return (p1 * b1 + p2 * b2 + p3 * b3 + p4 * b4);
				}

			public:
								
				// -----------------------------------------------
				// Operations

				// -----------------------------------------------

				void ClearSplinePoint()
				{
					ControlPoints.clear();
				}

				void AddSplinePoint(const glm::vec3& v)
				{
					ControlPoints.push_back(v);
					DeltaT = (float)1 / (float)ControlPoints.size();
				}

				// -----------------------------------------------
				//

				glm::vec3 GetInterpolatedSplinePoint(float t)   // t = 0...1; 0=vp[0] ... 1=vp[max]
				{
					// Find out in which interval we are on the spline

					int p = (int)(t / DeltaT);

					// Compute local control point indices

					int p0 = p - 1;
					int p1 = p;
					int p2 = p + 1;
					int p3 = p + 2;

					int n = (int)ControlPoints.size() - 1;

					if (p0 < 0) p0 = 0; else if (p0 >= n) p0 = n;
					if (p1 < 0) p1 = 0; else if (p1 >= n) p1 = n;
					if (p2 < 0) p2 = 0; else if (p2 >= n) p2 = n;
					if (p3 < 0) p3 = 0; else if (p3 >= n) p3 = n;

					// Relative (local) time 

					float lt = (t - DeltaT * (float)p) / DeltaT;

					// Interpolate

					return Eq(lt, ControlPoints[p0], ControlPoints[p1], ControlPoints[p2], ControlPoints[p3]);
				}

				// -----------------------------------------------
				//

				int GetNumPoints()
				{
					return (int)ControlPoints.size();
				}

				// -----------------------------------------------
				//

				glm::vec3& GetNthPoint(int n)
				{
					return ControlPoints[n];
				}

				// -----------------------------------------------
				// Constructors and destructor

				Spline() : ControlPoints(), DeltaT(0)
				{
				}

				~Spline()
				{}

		};
		
	}	// end of geo3d namespace

}	// end of vml namespace

