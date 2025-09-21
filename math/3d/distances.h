#pragma once

////////////////////////////////////////////////////////////////////////////////////
// point class

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{
			/*
			////////////////////////////////////////////////////////////////////////////////////
			// Containment

			static const unsigned int POINT_OUTSIDE = 0;
			static const unsigned int POINT_INSIDE = 1;
			*/
			////////////////////////////////////////////////////////////////////////////////////
			// Distances functions for points

			// ---------------------------------------------------------------------------------
			// get squared distance from point

			static [[nodiscard]] float SquaredDistanceBetweenPoints(const vml::math::vec3f& p, const vml::math::vec3f& c)
			{
				return vml::math::SquaredLength(p - c);
			}

			// ---------------------------------------------------------------------------------
			// get distance from point

			static [[nodiscard]] float DistanceBetweenPoints(const vml::math::vec3f& p, const vml::math::vec3f& c)
			{
				return vml::math::Length(p - c);
			}

			// ---------------------------------------------------------------------------------
			// Checks to see if the three points given are
			// collinear by checking to see if each element of the cross
			// product is zero.

			static bool ArePointsCollinear(const vml::math::vec3f& v1, const vml::math::vec3f& v2, vml::math::vec3f& v3,const float eps = vml::math::EPSILON)
			{
				float u = (v3.z - v1.z) * (v2.y - v1.y) - (v2.z - v1.z) * (v3.y - v1.y);
				float v = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
				float w = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
				return((u > -eps && u < eps) &&
					   (v > -eps && v < eps) &&
					   (w > -eps && w < eps));
			}

			// ---------------------------------------------------------------------------------
			// test inclusion between point and
			// sphere

			static bool ArePointsInRange(const vml::math::vec3f& p, const vml::math::vec3f& c, const float radius, const float eps = vml::math::EPSILON)
			{
				return vml::math::SquaredLength(p - c) - radius * radius < eps;
			}

			/*
			// ---------------------------------------------------------------------------------
			// inclusion of point in an axis aligned bounding box

			static int IsPointInAABBox(const glm::vec3& bmin, const glm::vec3& bmax, const glm::vec3& p)
			{
				if (p.x >= bmin.x && p.x <= bmax.x)
					if (p.y >= bmin.y && p.y <= bmax.y)
						if (p.z >= bmin.z && p.z <= bmax.z)
							return POINT_INSIDE;
				return POINT_OUTSIDE;
			}

			// ---------------------------------------------------------------------------------
			// Vertices must be given in this order
			// or function won't work correctly
			//  
			//          d0 ---------------- c0
			//          | \					| \
			//          |  \				|  \
			//          |	h0 ---------------- g0
			//          |	 |				|    |
			//			a0 ---------------- b0   |
			//			  \	 |				  \  |
			//			   \ |			       \ |
			//              e0 ---------------- f0

			static int IsPointInAOBB(const glm::vec3& p,
									 const glm::vec3& a0, const glm::vec3& b0, const glm::vec3& c0, const glm::vec3& d0,
									 const glm::vec3& e0, const glm::vec3& f0, const glm::vec3& g0, const glm::vec3& h0)
			{
				if (glm::dot(a0 - p, b0 - a0) < 0 &&
					glm::dot(p - b0, b0 - a0) < 0 &&
					glm::dot(a0 - p, d0 - a0) < 0 &&
					glm::dot(p - d0, d0 - a0) < 0 &&
					glm::dot(a0 - p, e0 - a0) < 0 &&
					glm::dot(p - e0, e0 - a0) < 0)
						return POINT_INSIDE;
				return POINT_OUTSIDE;
			}

			// ---------------------------------------------------------------------------------
			// Compute barycentric coordinates (u, v, w) for
			// point p with respect to triangle (a, b, c)

			static int IsPointInTriangle(const glm::vec3& p,
										 const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
										 const float& eps = vml::math::EPSILON)
			{
				glm::vec3 v0 = b - a;
				glm::vec3 v1 = c - a;
				glm::vec3 v2 = p - a;
				float d00 = glm::dot(v0, v0);
				float d01 = glm::dot(v0, v1);
				float d11 = glm::dot(v1, v1);
				float d20 = glm::dot(v2, v0);
				float d21 = glm::dot(v2, v1);
				float denom = 1.0f / (d00 * d11 - d01 * d01);
				float v = (d11 * d20 - d01 * d21) * denom;
				float w = (d00 * d21 - d01 * d20) * denom;
				float u = 1.0f - v - w;
				if (u > -eps && u < 1 + eps && v > -eps && v < 1 + eps && w > -eps && w < 1 + eps)
					return POINT_INSIDE;
				return POINT_OUTSIDE;
			}

			*/
		}	// end of distances namespace
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{
			/*
			/////////////////////////////////////////////////////////////////////////////
			// Computes the distance between a point p and an AABB b

			static float SquaredClosestDistanceFromPointToAABB(const glm::vec3 &p, const glm::vec3 &bmin, const glm::vec3 &bmax)
			{
				float sqDist = 0.0f;
				float v;

				// For each axis count any excess distance outside box extents

				v = p.x;
				if (v < bmin.x) sqDist += (bmin.x - v) * (bmin.x - v);
				if (v > bmax.x) sqDist += (v - bmax.x) * (v - bmax.x);

				v = p.y;
				if (v < bmin.y) sqDist += (bmin.y - v) * (bmin.y - v);
				if (v > bmax.y) sqDist += (v - bmax.y) * (v - bmax.y);

				v = p.z;
				if (v < bmin.z) sqDist += (bmin.z - v) * (bmin.z - v);
				if (v > bmax.z) sqDist += (v - bmax.z) * (v - bmax.z);

				return sqDist;
			}
			*/
		}	// end of distances namespace
	}	// end of geo3d namespace
}	// end of vml namespace

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{
			/*
			/////////////////////////////////////////////////////////////////////////////////////////
			// Returns the squared distance between point c and segment ab

			static float SquaredDistanceFromPointToLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
			{
				glm::vec3 ab = b - a;
				glm::vec3 ac = c - a;
				glm::vec3 bc = c - b;
				float e = glm::dot(ac, ab);
				// Handle cases where c projects outside ab
				if (e <= 0.0f) return glm::dot(ac, ac);
				float f = glm::dot(ab, ab);
				if (e >= f) return glm::dot(bc, bc);
				// Handle cases where c projects onto ab
				return glm::dot(ac, ac) - e * e / f;
			}

			/////////////////////////////////////////////////////////////////////////////////////////
			// Closest point to segment

			static void ClosestPointToLine(const glm::vec3& c, const glm::vec3& a, const glm::vec3& b, float& t, glm::vec3& d)
			{
				glm::vec3 ab = b - a;

				// Project c onto ab, but deferring divide by Dot(ab, ab)

				t = glm::dot(c - a, ab);

				if (t <= 0.0f)
				{
					// c projects outside the [a,b] interval, on the a side; clamp to a

					t = 0.0f;
					d = a;
				}
				else
				{

					float denom = glm::dot(ab, ab); // Always nonnegative since denom = ||ab||∧2

					if (t >= denom)
					{
						// c projects outside the [a,b] interval, on the b side; clamp to b

						t = 1.0f;
						d = b;
					}
					else
					{
						// c projects inside the [a,b] interval; must do deferred divide now

						t = t / denom;

						d = a + t * ab;

					}
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////
			// shortest distance between 3d lines

			static void LineLineShortestDistance(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& q0, const glm::vec3& q1,
												 glm::vec3& pa, glm::vec3& pb, float& dist)
			{
				glm::vec3   u = p1 - p0;
				glm::vec3   v = q1 - q0;
				glm::vec3   w = p0 - q0;
				float    a = dot(u, u);         // always >= 0
				float    b = dot(u, v);
				float    c = dot(v, v);         // always >= 0
				float    d = glm::dot(u, w);
				float    e = glm::dot(v, w);
				float    D = a * c - b * b;     // always >= 0
				float    sc, sN, sD = D;		// sc = sN / sD, default sD = D >= 0
				float    tc, tN, tD = D;		// tc = tN / tD, default tD = D >= 0

				// compute the line parameters of the two closest points

				if (D < vml::math::EPSILON)
				{
					// the lines are almost parallel

					sN = 0.0f;         // force using point P0 on segment S1
					sD = 1.0f;         // to prevent possible division by 0.0 later
					tN = e;
					tD = c;

				}
				else
				{
					// get the closest points on the infinite lines

					sN = (b * e - c * d);
					tN = (a * e - b * d);

					if (sN < 0.0f)
					{
						// sc < 0 => the s=0 edge is visible

						sN = 0.0f;
						tN = e;
						tD = c;

					}
					else if (sN > sD)
					{
						// sc > 1  => the s=1 edge is visible

						sN = sD;
						tN = e + b;
						tD = c;

					}

				}

				if (tN < 0.0f)
				{
					// tc < 0 => the t=0 edge is visible

					tN = 0.0f;

					// recompute sc for this edge

					if (-d < 0.0f)
					{
						sN = 0.0f;
					}
					else if (-d > a)
					{
						sN = sD;
					}
					else
					{
						sN = -d;
						sD = a;
					}
				}
				else if (tN > tD)
				{
					// tc > 1  => the t=1 edge is visible

					tN = tD;

					// recompute sc for this edge

					if ((-d + b) < 0.0f)
					{
						sN = 0.0f;
					}
					else if ((-d + b) > a)
					{
						sN = sD;
					}
					else
					{
						sN = (-d + b);
						sD = a;
					}
				}

				// finally do the division to get sc and tc

				sc = (abs(sN) < vml::math::EPSILON ? 0.0f : sN / sD);
				tc = (abs(tN) < vml::math::EPSILON ? 0.0f : tN / tD);

				// get the difference of the two closest points

				dist = glm::length(w + (sc * u) - (tc * v));

				pa = p0 + (sc * u);
				pb = q0 + (tc * v);

			}
			*/
		}	// end of distances namespace
	}	// end of geo3d namespace
}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// distance functions for plane class

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{
			/*
			////////////////////////////////////////////////////////////////////////////////////
			// distance from point to plane assumes plane is normalized

			static float PlaneDistanceFromPoint(const glm::vec3& p0, const glm::vec3& normal,
												const glm::vec3& p)
			{
				return fabs(glm::dot(p - p0, normal));
			}

			////////////////////////////////////////////////////////////////////////////////////
			// Signed distance from point to plane assumes plane is normalized

			static float SignedPlaneDistanceFromPoint(const glm::vec3& p0, const glm::vec3& normal,
													  const glm::vec3& p)
			{
				return (glm::dot(p - p0, normal));
			}
			*/
		} // end of namespace planes
	} // end of namespace goe3d
}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{
		namespace distances
		{
			
			/////////////////////////////////////////////////////////////////////////////////////////
			// Closest point to triangle

			static float ClosestPointToTriangle2(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc, 
				                                const glm::vec3& x, glm::vec3& pt, glm::vec2& t, const float eps = vml::math::EPSILON)
			{
				// source: real time collision detection
				// check if x in vertex region outside pa

				glm::vec3 ab = pb - pa;
				glm::vec3 ac = pc - pa;
				glm::vec3 ax = x - pa;

				float d1 = glm::dot(ab, ax);
				float d2 = glm::dot(ac, ax);

				if (d1 < -eps && d2 < -eps)
				{
					// barycentric coordinates (1, 0, 0)

					t[0] = 1.0f;
					t[1] = 0.0f;
					pt = pa;

					return glm::length(x - pt);
				}

				// check if x in vertex region outside pb

				glm::vec3 bx = x - pb;

				float d3 = glm::dot(ab, bx);
				float d4 = glm::dot(ac, bx);

				if (d3 > eps && d4 <= d3)
				{

					// barycentric coordinates (0, 1, 0)

					t[0] = 0.0f;
					t[1] = 1.0f;
					pt = pb;

					return glm::length(x - pt);
				}

				// check if x in vertex region outside pc

				glm::vec3 cx = x - pc;

				float d5 = glm::dot(ab, cx);
				float d6 = glm::dot(ac, cx);

				if (d6 > eps && d5 <= d6)
				{

					// barycentric coordinates (0, 0, 1)

					t[0] = 0.0f;
					t[1] = 0.0f;
					pt = pc;

					return glm::length(x - pt);
				}

				// check if x in edge region of ab, if so return projection of x onto ab

				float vc = d1 * d4 - d3 * d2;

				if (vc < -eps && d1 > eps && d3 < -eps)
				{
					// barycentric coordinates (1 - v, v, 0)

					float v = d1 / (d1 - d3);

					t[0] = 1.0f - v;
					t[1] = v;
					pt = pa + ab * v;

					return glm::length(x - pt);
				}

				// check if x in edge region of ac, if so return projection of x onto ac

				float vb = d5 * d2 - d1 * d6;

				if (vb < -eps && d2 > eps && d6 < -eps)
				{
					// barycentric coordinates (1 - w, 0, w)

					float w = d2 / (d2 - d6);

					t[0] = 1.0f - w;
					t[1] = 0.0f;
					pt = pa + ac * w;

					return glm::length(x - pt);
				}

				// check if x in edge region of bc, if so return projection of x onto bc

				float va = d3 * d6 - d5 * d4;

				if (va < -eps && (d4 - d3) > eps && (d5 - d6) > eps)
				{
					// barycentric coordinates (0, 1 - w, w)

					float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));

					t[0] = 0.0f;
					t[1] = 1.0f - w;
					pt = pb + (pc - pb) * w;

					return glm::length(x - pt);
				}

				// x inside face region. Compute pt through its barycentric coordinates (u, v, w)

				float denom = 1.0f / (va + vb + vc);
				float v = vb * denom;
				float w = vc * denom;
				t[0] = 1.0f - v - w;
				t[1] = v;

				pt = pa + ab * v + ac * w; //= u*a + v*b + w*c, u = va*denom = 1.0f - v - w

				return glm::length(x - pt);
			}
			
			/////////////////////////////////////////////////////////////////////////////////////////
			// Closest point to triangle
			// same as above but without the uv baricentric out parameters

			static glm::vec3 ClosestPointToTriangle2(const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc, const glm::vec3& x, const float eps = vml::math::EPSILON)
			{
				// source: real time collision detection
				// check if x in vertex region outside pa

				glm::vec3 ab = pb - pa;
				glm::vec3 ac = pc - pa;
				glm::vec3 ax = x - pa;

				float d1 = glm::dot(ab, ax);
				float d2 = glm::dot(ac, ax);

				if (d1 < -eps && d2 < -eps)
				{
					// barycentric coordinates (1, 0, 0)

					return pa;
				}

				// check if x in vertex region outside pb

				glm::vec3 bx = x - pb;

				float d3 = glm::dot(ab, bx);
				float d4 = glm::dot(ac, bx);

				if (d3 > eps && d4 <= d3)
				{

					// barycentric coordinates (0, 1, 0)

					return pb;
				}

				// check if x in vertex region outside pc

				glm::vec3 cx = x - pc;

				float d5 = glm::dot(ab, cx);
				float d6 = glm::dot(ac, cx);

				if (d6 > eps && d5 <= d6)
				{

					// barycentric coordinates (0, 0, 1)

					return pc;
				}

				// check if x in edge region of ab, if so return projection of x onto ab

				float vc = d1 * d4 - d3 * d2;

				if (vc < -eps && d1 > eps && d3 < -eps)
				{
					// barycentric coordinates (1 - v, v, 0)

					return pa + ab * (d1 / (d1 - d3));

				}

				// check if x in edge region of ac, if so return projection of x onto ac

				float vb = d5 * d2 - d1 * d6;

				if (vb <-eps && d2 > eps && d6 < -eps)
				{
					// barycentric coordinates (1 - w, 0, w)

					return pa + ac * (d2 / (d2 - d6));

				}

				// check if x in edge region of bc, if so return projection of x onto bc

				float va = d3 * d6 - d5 * d4;

				if (va < -eps && (d4 - d3) > eps && (d5 - d6) > eps)
				{
					// barycentric coordinates (0, 1 - w, w)

					return pb + (pc - pb) * ((d4 - d3) / ((d4 - d3) + (d5 - d6)));

				}

				// x inside face region. Compute pt through its barycentric coordinates (u, v, w)

				float denom = 1.0f / (va + vb + vc);
				float v = vb * denom;
				float w = vc * denom;

				//= u*a + v*b + w*c, u = va*denom = 1.0f - v - w

				return pa + ab * v + ac * w;

			}
			/*
			////////////////////////////////////////////////////////////////////////////////
			// Nearest point on triangle v0,v1,v2, from point 

			static glm::vec3 ClosestPointToTriangle(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const float eps = vml::math::EPSILON)
			{
				glm::vec3 diff = point - v0;
				glm::vec3 edge0 = v1 - v0;
				glm::vec3 edge1 = v2 - v0;
				float a00 = glm::dot(edge0, edge0);
				float a01 = glm::dot(edge0, edge1);
				float a11 = glm::dot(edge1, edge1);
				float b0 = -glm::dot(diff, edge0);
				float b1 = -glm::dot(diff, edge1);
				float zero = -eps;
				float one = 1.0f + eps;
				float det = a00 * a11 - a01 * a01;
				float t0 = a01 * b1 - a11 * b0;
				float t1 = a01 * b0 - a00 * b1;

				if (t0 + t1 <= det)
				{
					if (t0 < zero)
					{
						if (t1 < zero)  // region 4
						{
							if (b0 < zero)
							{
								t1 = zero;
								if (-b0 >= a00)  // V1
								{
									t0 = one;
								}
								else  // E01
								{
									t0 = -b0 / a00;
								}
							}
							else
							{
								t0 = zero;
								if (b1 >= zero)  // V0
								{
									t1 = zero;
								}
								else if (-b1 >= a11)  // V2
								{
									t1 = one;
								}
								else  // E20
								{
									t1 = -b1 / a11;
								}
							}
						}
						else  // region 3
						{
							t0 = zero;
							if (b1 >= zero)  // V0
							{
								t1 = zero;
							}
							else if (-b1 >= a11)  // V2
							{
								t1 = one;
							}
							else  // E20
							{
								t1 = -b1 / a11;
							}
						}
					}
					else if (t1 < zero)  // region 5
					{
						t1 = zero;
						if (b0 >= zero)  // V0
						{
							t0 = zero;
						}
						else if (-b0 >= a00)  // V1
						{
							t0 = one;
						}
						else  // E01
						{
							t0 = -b0 / a00;
						}
					}
					else  // region 0, interior
					{
						float invDet = one / det;
						t0 *= invDet;
						t1 *= invDet;
					}
				}
				else
				{
					float tmp0, tmp1, numer, denom;

					if (t0 < zero)  // region 2
					{
						tmp0 = a01 + b0;
						tmp1 = a11 + b1;
						if (tmp1 > tmp0)
						{
							numer = tmp1 - tmp0;
							denom = a00 - (float)2 * a01 + a11;
							if (numer >= denom)  // V1
							{
								t0 = one;
								t1 = zero;
							}
							else  // E12
							{
								t0 = numer / denom;
								t1 = one - t0;
							}
						}
						else
						{
							t0 = zero;
							if (tmp1 <= zero)  // V2
							{
								t1 = one;
							}
							else if (b1 >= zero)  // V0
							{
								t1 = zero;
							}
							else  // E20
							{
								t1 = -b1 / a11;
							}
						}
					}
					else if (t1 < zero)  // region 6
					{
						tmp0 = a01 + b1;
						tmp1 = a00 + b0;
						if (tmp1 > tmp0)
						{
							numer = tmp1 - tmp0;
							denom = a00 - (float)2 * a01 + a11;
							if (numer >= denom)  // V2
							{
								t1 = one;
								t0 = zero;
							}
							else  // E12
							{
								t1 = numer / denom;
								t0 = one - t1;
							}
						}
						else
						{
							t1 = zero;
							if (tmp1 <= zero)  // V1
							{
								t0 = one;
							}
							else if (b0 >= zero)  // V0
							{
								t0 = zero;
							}
							else  // E01
							{
								t0 = -b0 / a00;
							}
						}
					}
					else  // region 1
					{
						numer = a11 + b1 - a01 - b0;
						if (numer <= zero)  // V2
						{
							t0 = zero;
							t1 = one;
						}
						else
						{
							denom = a00 - (float)2 * a01 + a11;
							if (numer >= denom)  // V1
							{
								t0 = one;
								t1 = zero;
							}
							else  // 12
							{
								t0 = numer / denom;
								t1 = one - t0;
							}
						}
					}
				}

				// paraqmetric coordiantes

				// u = one - t0 - t1;
				// v = t0;
				// w = t1;
				// distance = lenght of	point - result.closest

				return v0 + t0 * edge0 + t1 * edge1;
			}
			*/
		} // end of namespace planes
	} // end of namespace goe3d
}	// end of vml namespace
