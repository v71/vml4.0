#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vector metrics

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{

			// --------------------------------------------------------
			// Calculates reflection vector from entering ray direction 'v'
			// and surface normal 'w'.

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Reflect(const vml::math::vec3<T>& v, 
											                const vml::math::vec3<T>& n, 
												            const T eps=vml::math::EPSILON)
			{
				T d = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / d;
				T nx = n[0] * d;
				T ny = n[1] * d;
				T nz = n[2] * d;
				T num = T(2) * (v.x * nx + v.y * ny + v.z * nz);
				return vec3<T>(v.x - num * nx, v.y - num * ny, v.z - num * nz);
			}

			// --------------------------------------------------------
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.

			template <typename T>
			static void Orthogonalize(const vml::math::vec3<T>& v,
				                      const vml::math::vec3<T>& w, 
									  vml::math::vec3<T>& pv1, vml::math::vec3<T>& pv2,
									  const T eps = vml::math::EPSILON)
			{
				// v is the basis vector normalized

				pv1 = v;
				T d = sqrtf(pv1[0] * pv1[0] + pv1[1] * pv1[1] + pv1[2] * pv1[2]);
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / d;
				pv1[0] *= d;
				pv1[1] *= d;
				pv1[2] *= d;

				// compute w-Perpendicular(w,v)

				d = v.x * v.x + v.y * v.y + v.z * v.z;
				if (d > -eps && d < eps)
					d = eps;
				T num = (v.x * w.x + v.y * w.y + v.z * w.z) / d;
				pv2 = w - num * v;

				// normalize resulting vector

				d = sqrtf(pv2[0] * pv2[0] + pv2[1] * pv2[1] + pv2[2] * pv2[2]);
				if (d > -eps && d < eps)
					d = eps;
				d = T(1) / d;
				pv2[0] *= d;
				pv2[1] *= d;
				pv2[2] *= d;
			}

			// --------------------------------------------------------
			// Calculates the component of 'v' perpendicular to 'w'.

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Perpendicular(const vml::math::vec3<T>& v, 
				                                                  const vml::math::vec3<T>& w, 
				                                                  const T eps = vml::math::EPSILON)
			{
				T d = w.x * w.x + w.y * w.y + w.z * w.z;
				if (d > -eps && d < eps)
					d = eps;
				T num = (v.x * w.x + v.y * w.y + v.z * w.z) / d;
				return v - num * w;
			}

			// --------------------------------------------------------
			// reciprocal of a vector 

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Reciprocal(const vml::math::vec3<T>& v,
															   const T eps = vml::math::EPSILON)
			{
				if (v.x > -eps && v.x < eps) v.x = eps;
				if (v.y > -eps && v.y < eps) v.y = eps;
				if (v.z > -eps && v.z < eps) v.z = eps;
				return vec3<T>(T(1) / v.x, T(1) / v.y, T(1) / v.z);
			}

			// --------------------------------------------------------
			// Calculates the projection of 'v' onto 'w'.

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Project(const vml::math::vec3<T>& v, 
				                                            const vml::math::vec3<T>& w)
			{
				T d = w.x * w.x + w.y * w.y + w.z * w.z;
				if (d > -vml::math::EPSILON && d < vml::math::EPSILON)
					d = vml::math::EPSILON;
				T num = (v.x * w.x + v.y * w.y + v.z * w.z) / d;
				return num * w;
			}

			// --------------------------------------------------------
			// center value

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Mid(const vml::math::vec3<T>& v,
														const vml::math::vec3<T>& w)
			{
				return vec3<T>((v.x + w.x) * T(0.5), (v.y + w.y) * T(0.5), (v.z + w.z) * T(0.5));
			}

			// --------------------------------------------------------
			// Linera interpolation

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Lerp(const vml::math::vec3<T>& v, 
				                                         const vml::math::vec3<T>& w, 
				                                         const T s)
			{
				return vec3<T>(v.x + (w.x - v.x) * s, v.y + (w.y - v.y) * s, v.z + (w.z - v.z) * s);
			}

			// --------------------------------------------------------
			// Saturate in range 0..1

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Saturate(const vml::math::vec3<T>& v,
				                                             const T eps=vml::math::EPSILON)
			{
				T x = v.x;
				T y = v.y;
				T z = v.z;
				if (x > T(1) - eps) x = T(1);
				if (y > T(1) - eps) y = T(1);
				if (z > T(1) - eps) z = T(1);
				if (x < T(0) + eps) x = T(0);
				if (y < T(0) + eps) y = T(0);
				if (z < T(0) + eps) z = T(0);
				return vec3<T>(x, y, z);
			}

			// --------------------------------------------------------
			// Saturate to zero

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> SaturateToZero(const vml::math::vec3<T>& v,
				                                                   const T eps=vml::math::EPSILON)
			{
				T x = v.x;
				T y = v.y;
				T z = v.z;
				if (x < T(0) + eps) x = T(0);
				if (y < T(0) + eps) y = T(0);
				if (z < T(0) + eps) z = T(0);
				return vec3<T>(x, y, z);
			}

			// --------------------------------------------------------
			// Saturate a vector to one

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> SaturateToOne(const vml::math::vec3<T>& v, 
				                                                  const T eps = vml::math::EPSILON)
			{
				T x = v.x;
				T y = v.y;
				T z = v.z;
				if (x > T(1) - eps) x = T(1);
				if (y > T(1) - eps) y = T(1);
				if (z > T(1) - eps) z = T(1);
				return vec3<T>(x, y, z);
			}

			// --------------------------------------------------------
			// floors a vector

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Floor(const vml::math::vec3<T>& v)
			{
				return vec3<T>(floor(v.x), floor(v.y), floor(v.z));
			}

			// --------------------------------------------------------
			// ceils a vector

			template <typename T>
			static [[nodiscard]] vml::math::vec3<T> Ceil(const vml::math::vec3<T>& v)
			{
				return vec3<T>(ceil(v.x), ceil(v.y), ceil(v.z));
			}

		} // end of metrics namespace
	} // end of geo2d namespace
} // end of vml namespace

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// line metrics

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{
		
			// ------------------------------------------------------------------------
			// compute line length
		
			template<typename T>
			static [[nodiscard]] T LineLength(const vml::math::vec3<T>& p0, 
				                              const vml::math::vec3<T>& p1,
				                              const T eps=vml::math::EPSILON)
			{
				vml::math::vec3<T> dir = p1 - p0;
				T denum = sqrtf(dir.x * dir.x + dir.y * dir.y);
				if (denum > -eps && denum < eps)
					denum = eps;
				return denum;
			}

			// ------------------------------------------------------------------------
			// compute squared line length
			
			template<typename T>
			static [[nodiscard]] T SquaredLineLength(const vml::math::vec3<T>& p0, 
				                                     const vml::math::vec3<T>& p1, 
				                                     const T eps = vml::math::EPSILON)
			{
				vml::math::vec3<T> dir = p1 - p0;
				T denum = dir.x * dir.x + dir.y * dir.y;
				if (denum > -eps && denum < eps)
					denum = eps;
				return denum;
			}

			// ------------------------------------------------------------------------
			// compute direction vector given two points

			template<typename T>
			static [[nodiscard]] vml::math::vec3<T> LineDirection(const vml::math::vec3<T>& p0, 
				                                                  const vml::math::vec3<T>& p1, 
				                                                  const T eps = vml::math::EPSILON)
			{
				vml::math::vec3<T> dir = p1 - p0;
				T denum = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = (T)1 / denum;
				return vml::math::vec3f(dir.x * denum, dir.y * denum, dir.z * denum);
			}

			// ------------------------------------------------------------------------
			// returns normal to a direction, ray input vector must be normalized

			template<typename T>
			static [[nodiscard]] vml::math::vec3<T> LineNormal(const vml::math::vec3f& p0,
															   const vml::math::vec3f& p1, 
															   const T eps = vml::math::EPSILON)
			{
				vml::math::vec3<T> dir = p1 - p0;
				vml::math::vec3<T> ref(1, 0, 0);
				vml::math::vec3<T> n = vml::math::Cross(ref, dir);
				if (std::fabs(n.x) < eps && std::fabs(n.y) < eps && std::fabs(n.z) < eps)
				{
					ref=vml::math::vec3<T>(1, 0, 0);
					n = vml::math::Cross(ref, dir);
					if (std::fabs(n.x) < eps && std::fabs(n.y) < eps && std::fabs(n.z) < eps)
					{
						ref=vml::math::vec3<T>(0, 0, 1);
						n = vml::math::Cross(ref, dir);
					}
				}
				T denum = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
				if (denum > -eps && denum < eps)
					denum = eps;
				denum = 1.0f / denum;
				n.x *= denum;
				n.y *= denum;
				n.z *= denum;
				return n;
			}
			
			// ------------------------------------------------------------------------
			// returns ray slope

			template<typename T>
			static [[nodiscard]] vml::math::vec3<T> LineSlope(const vml::math::vec3<T>& p0,
															  const vml::math::vec3<T>& p1)
			{
				return vml::math::vec3<T>(vml::math::angles::Atan2(p1.x - p0.x, p1.y - p0.y, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f,
										  vml::math::angles::Atan2(p1.y - p0.y, p1.z - p0.z, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f,
										  vml::math::angles::Atan2(p1.x - p0.x, p1.z - p0.z, vml::math::EPSILON) / vml::math::PI * 180.0f + 180.0f);
			}

			// ------------------------------------------------------------------------
			// line center

			template<typename T>
			static [[nodiscard]] vml::math::vec3<T> LineCenter(const vml::math::vec3<T>& bmin,
								                               const vml::math::vec3<T>& bmax)
			{
				return vml::math::vec3<T>((bmin.x + bmax.x) * 0.5f, (bmin.y + bmax.y) * 0.5f, (bmin.z + bmax.z) * 0.5f);
			}

			/////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////
			// Transform point by a rotation matrix
			/*
			static glm::vec3 TransformPoint(glm::vec3& p, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
			{
				float sx = sin(rot.x * vml::math::DEGTORAD);
				float sy = sin(rot.y * vml::math::DEGTORAD);
				float sz = sin(rot.z * vml::math::DEGTORAD);
				float cx = cos(rot.x * vml::math::DEGTORAD);
				float cy = cos(rot.y * vml::math::DEGTORAD);
				float cz = cos(rot.z * vml::math::DEGTORAD);
				float M[16] = { 0 };

				M[0] = (cy * cz) * scale.x;
				M[1] = (cy * sz) * scale.x;
				M[2] = (-sy) * scale.x;
				M[3] = 0;
				M[4] = (sx * sy * cz - cx * sz) * scale.y;
				M[5] = (sx * sy * sz + cx * cz) * scale.y;
				M[6] = (sx * cy) * scale.y;
				M[7] = 0;
				M[8] = (cx * sy * cz + sx * sz) * scale.z;
				M[9] = (cx * sy * sz - sx * cz) * scale.z;
				M[10] = (cx * cy) * scale.z;
				M[11] = 0;
				M[12] = pos.x;
				M[13] = pos.y;
				M[14] = pos.z;
				M[15] = 1;

				return glm::vec3(p[0] * M[0] + p[1] * M[4] + p[2] * M[8] + M[12],
								 p[0] * M[1] + p[1] * M[5] + p[2] * M[9] + M[13],
								 p[0] * M[2] + p[1] * M[6] + p[2] * M[10] + M[14]);
			}
			*/
		} // end of metrics namespace
	} // end of geo2d namespace
} // end of vml namespace

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{
			/*
			// ------------------------------------------------------------------
			// return bounding box corner vertex

			static const glm::vec3 GetAABBoxExtremalPoint(const glm::vec3& min, const glm::vec3& max, const glm::vec3& direction) 
			{
				return glm::vec3((direction.x >= 0.f ? max.x : min.x),
								 (direction.y >= 0.f ? max.y : min.y),
								 (direction.z >= 0.f ? max.z : min.z));
			}

			// ------------------------------------------------------------------
			// return bounding box corner vertex

			static const glm::vec3 GetAABBoxCornerPoint(const glm::vec3& min, const glm::vec3& max, int cornerIndex)
			{
				switch (cornerIndex)
				{
					default:
						case 0: return glm::vec3(min.x, min.y, min.z); break;
						case 1: return glm::vec3(min.x, min.y, max.z); break;
						case 2: return glm::vec3(min.x, max.y, min.z); break;
						case 3: return glm::vec3(min.x, max.y, max.z); break;
						case 4: return glm::vec3(max.x, min.y, min.z); break;
						case 5: return glm::vec3(max.x, min.y, max.z); break;
						case 6: return glm::vec3(max.x, max.y, min.z); break;
						case 7: return glm::vec3(max.x, max.y, max.z); break;
					break;
				}
			}

			// ------------------------------------------------------------------
			// return boundng box edge in the form of a starting and ending vector

			static vml::geo3d::Line GetAABBoxEdge(const glm::vec3& min, const glm::vec3& max, int edgeIndex)
			{
				switch (edgeIndex)
				{
					default:
						case  0: return vml::geo3d::Line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z)); break;
						case  1: return vml::geo3d::Line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z)); break;
						case  2: return vml::geo3d::Line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z)); break;
						case  3: return vml::geo3d::Line(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z)); break;
						case  4: return vml::geo3d::Line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z)); break;
						case  5: return vml::geo3d::Line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z)); break;
						case  6: return vml::geo3d::Line(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z)); break;
						case  7: return vml::geo3d::Line(glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z)); break;
						case  8: return vml::geo3d::Line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z)); break;
						case  9: return vml::geo3d::Line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z)); break;
						case 10: return vml::geo3d::Line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z)); break;
						case 11: return vml::geo3d::Line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z)); break;
					break;
				}
			}
			*/
		} // end of metrics namespace
	} // end of geo2d namespace
} // end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// metrics functions for plane class

namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{

			/////////////////////////////////////////////////////////////////////////////
			// flags 
			/*
			static const unsigned int OUTSIDE   = 0;
			static const unsigned int INSIDE    = 1;
			static const unsigned int BOUNDARY  = 2;
			static const unsigned int INTERSECT = 3;

			////////////////////////////////////////////////////////////////////////////////////
			// classify point against plane

			static const int ClassifyPointVsPlane(const glm::vec3& p0, const glm::vec3& normal,
												  const glm::vec3& p,
												  float eps = vml::math::EPSILON)
			{
				float d = glm::dot(p - p0, normal);
				if (d > eps)
					return INSIDE;
				else if (d < -eps)
					return OUTSIDE;
				return BOUNDARY;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify triangle against a plane
			// returns which halfspace the triangle is
			// inside for positive half space
			// outside for negative halfspace
			// intersect if triangles intersects the plane

			static int ClassifyTriangleVsPlane(const glm::vec3& p0, const glm::vec3& normal,
											   const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
											   float eps = vml::math::EPSILON)
			{
				float num;
				int front, back, on;

				front = 0;
				back = 0;
				on = 0;

				num = glm::dot(normal, a - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
					on++;
				}

				num = glm::dot(normal, b - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
					on++;
				}

				num = glm::dot(normal, c - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
					on++;
				}

				if (on == 3)
					return BOUNDARY;
				else if (front == 3)
					return INSIDE;
				else if (back == 3)
					return OUTSIDE;

				return INTERSECT;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify axis aligned bounding box against a plane
			// returns which halfspace the axis aligned bounding box lies on
			// inside for positive half space
			// outside for negative halfspace
			// intersect if axis aligned box intersects the plane

			static int ClassifyAABBoxVsPlane(const glm::vec3& p0, const glm::vec3& normal,
											 const glm::vec3& bmin, const glm::vec3& bmax,
											 float eps = vml::math::EPSILON)
			{
				float num;
				int front, back;

				front = 0;
				back = 0;

				num = glm::dot(normal, bmin - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, bmax - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				if (front == 2)
					return INSIDE;
				else if (back == 2)
					return OUTSIDE;

				return INTERSECT;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify axis oriented bounding box against a plane
			// returns which halfspace the axis oriented bounding box lies on
			// inside for positive half space
			// outside for negative halfspace
			// intersect if axis oriented box intersects the plane

			static int ClassifyAOBBoxVsPlane(const glm::vec3& p0, const glm::vec3& normal,
											 const glm::vec3& a0,
											 const glm::vec3& b0,
											 const glm::vec3& c0,
											 const glm::vec3& d0,
											 const glm::vec3& e0,
											 const glm::vec3& f0,
											 const glm::vec3& g0,
											 const glm::vec3& h0,
											 float eps = vml::math::EPSILON)
			{
				float num;
				int front, back;

				front = 0;
				back = 0;

				num = glm::dot(normal, a0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, b0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, c0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, d0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, e0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, f0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, g0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				num = glm::dot(normal, h0 - p0);

				if (num > eps)
					front++;
				else if (num < -eps)
					back++;
				else
				{
					front++;
					back++;
				}

				if (front == 8)
					return INSIDE;
				else if (back == 8)
					return OUTSIDE;

				return INTERSECT;
			}

			////////////////////////////////////////////////////////////////////////////////////
			// classify sphere against a plane
			// returns which halfspace the sphere box lies on
			// inside for positive half space
			// outside for negative halfspace
			// intersect if axis oriented box intersects the plane

			static int ClassifySphereVsPlane(const glm::vec3& pos,
											 const float radius,
											 const glm::vec3& p0,
											 const glm::vec3& normal,
											 float eps = vml::math::EPSILON)
			{
				float dpmax = glm::dot(normal, pos - p0) - radius;

				if (dpmax > 0)
					return INSIDE;

				if (dpmax < -2.0f * radius)
					return OUTSIDE;

				return INTERSECT;

			}

			////////////////////////////////////////////////////////////////////////////////
			// Clips a segment against plane

			static int ClassifyLineVsPlane(const glm::vec3& a, const glm::vec3& b,
										   const glm::vec3& p0, const glm::vec3& n,
										   glm::vec3& q,
										   float eps = vml::math::EPSILON)
			{

				float b1 = glm::dot(p0 - a, n);
				float b2 = glm::dot(p0 - b, n);

				// segment is all below plane

				if (b1 < -eps && b2 < -eps)
					return OUTSIDE;

				// segment is all above plane

				if (b1 > -eps && b2 > -eps)
					return INSIDE;

				// segment is intersected

				float denum = b1 - b2;

				if (denum > -eps && denum < eps)
					denum = eps;

				// computing intersection point

				q = a + b1 * (b - a) / denum;

				return INTERSECT;
			}
			*/
		} // end of namespace planes
	} // end of namespace goe3d
}	// end of vml namespace


namespace vml
{
	namespace geo3d
	{
		namespace metrics
		{
			/*
			////////////////////////////////////////////////////////////////////////////////
			// Compute barycentric coordinates (u, v, w) for
			// point p with respect to triangle (a, b, c)

			static void Barycentric(const glm::vec3& p,
									const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
									float& u, float& v, float& w)
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
				v = (d11 * d20 - d01 * d21) * denom;
				w = (d00 * d21 - d01 * d20) * denom;
				u = 1.0f - v - w;
			}
			*/
		} // end of metrics namespace
	} // end of geo2d namespace
} // end of vml namespace
