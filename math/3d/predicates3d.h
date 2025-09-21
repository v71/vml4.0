#pragma once

////////////////////////////////////////////////////////////////////////////////////
// 3d predicates

////////////////////////////////////////////////////////////////////////////////////
// 3d points

namespace vml
{
	namespace geo3d
	{
		using Point = glm::vec3;
	
		////////////////////////////////////////////////////////////////////////////////////////
		//  constant vectors

		const glm::vec3 NullVec     = glm::vec3( 0,  0,  0);
		const glm::vec3 UnaryVec    = glm::vec3( 1,  1,  1);
		const glm::vec3 UpVec       = glm::vec3( 0,  1,  0);
		const glm::vec3 DownVec     = glm::vec3( 0, -1,  0);
		const glm::vec3 LeftVec     = glm::vec3( 1,  0,  0);
		const glm::vec3 RighttVec   = glm::vec3(-1,  0,  0);
		const glm::vec3 ForwardVec  = glm::vec3( 0,  0,  1);
		const glm::vec3 BackwardVec = glm::vec3( 0,  0, -1);

	}
}

//////////////////////////////////////////////////////////////////
// vertex class holding vertex attributes

namespace vml
{
	namespace geo3d
	{

		class Vertex
		{
			public:

				glm::vec4 Pos;		// homogeneous point coordinates
				glm::vec3 Normal;	// normal coordinates
				glm::vec2 UV;		// texture coordinates
				
				// ------------------------------------------
				// ctor / dtor

				Vertex(const glm::vec3& pos,
					   const glm::vec3& normal,
					   const glm::vec2& uv) : Pos(glm::vec4(pos.x, pos.y, pos.z, 1)), Normal(normal), UV(uv)
				{
				}

				Vertex() :Pos(glm::vec4(0, 0, 0, 0)), Normal(glm::vec3(0, 0, 0)), UV(glm::vec2(0, 0))
				{}

				~Vertex()
				{}

		};

	}
}

/////////////////////////////////////////////////////////////////////////////
// flags for spheres

namespace vml
{
	namespace geo3d
	{

		class Line
		{
				glm::vec3 P1;
				glm::vec3 P2;
				glm::vec3 Normal;

			public:

				//---------------------------------------------------------------
				//query functions

				const glm::vec3& GetP1()			const { return P1; }
				const glm::vec3& GetP2()			const { return P2; }
				const glm::vec2& GetNormal()		const { return Normal; }

				// ------------------------------------------------------------------
				// setting 

				void Set(const glm::vec3& p1, const glm::vec3& p2)
				{
					P1 = p1;
					P2 = p2;
					
					glm::vec3 dir = P2 - P1;

					float length = glm::length(dir);

					if (length > -vml::math::EPSILON && length < vml::math::EPSILON) length = vml::math::EPSILON;

					float invlenght = 1.0f / length;

					// compute dir cosines

					glm::vec3 dircosines(dir.x * invlenght,dir.y * invlenght,dir.z * invlenght);

					// compute normal

					float min = fabs(dir.x);

					glm::vec3 cardinalAxis(1, 0, 0);

					if (fabs(dir.y) < min)
					{
						min = fabs(dir.y);
						cardinalAxis = glm::vec3(0, 1, 0);
					}

					if (fabs(dir.z) < min)
					{
						cardinalAxis = glm::vec3(0, 0, 1);
					}

					Normal = glm::cross(dir, cardinalAxis);
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				Line(const glm::vec3& p1, const glm::vec3& p2)
				{
					Set(p1, p2);
				}

				Line() : P1(glm::vec3(0,0,0)),P2(glm::vec3(0,0,0)),Normal(glm::vec3(0,0,0))
				{
				}

				~Line()
				{}
		};

	}	// end of geo3d namespace

}	// end of vml namespace


////////////////////////////////////////////////////////////////////////////////////////////////////
// triangles

namespace vml
{
	namespace geo3d
	{

		class Triangle
		{

			private:

				glm::vec3 P0;
				glm::vec3 P1;
				glm::vec3 P2;
				glm::vec3 Center;
				glm::vec3 Normal;			// normal
				glm::vec3 EdgeNormal0;		// edge normal 
				glm::vec3 EdgeNormal1;		// edge normal
				glm::vec3 EdgeNormal2;		// edge normal
				float	  Area;

			public:

				// ------------------------------------------------------------------
				//

				void Set(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const float& eps = vml::math::EPSILON)
				{
					P0 = p0;
					P1 = p1;
					P2 = p2;
					Center = (P0 + P1 + P2) * 0.333f;
					Normal = (glm::cross(p1 - p0, p2 - p0));
					Area = fabs(glm::length(Normal) * 0.5f);
					Normal = glm::normalize(Normal);
					EdgeNormal0 = glm::normalize(glm::cross(Normal, p2 - p0));	// compute first edge normal
					EdgeNormal1 = glm::normalize(glm::cross(Normal, p0 - p1));	// compute second edge normal
					EdgeNormal2 = glm::normalize(glm::cross(p1 - p2 + Normal, p1 - p2));	// compute third edge normal

				}

				// ------------------------------------------

				const glm::vec3& GetCenter() const { return Center; }
				const glm::vec3& GetNormal() const { return Normal; }
				const glm::vec3& GetEdgeNromal0() const { return EdgeNormal0; }
				const glm::vec3& GetEdgeNromal1() const { return EdgeNormal1; }
				const glm::vec3& GetEdgeNromal2() const { return EdgeNormal2; }
				float GetArea() const { return Area; }

				// ------------------------------------------------------------------
				// ctor / dtor

				Triangle()
				{
					Set(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
				}

				Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
				{
					Set(p0, p1, p2);
				}

				~Triangle()
				{}

		};

	}
}


//////////////////////////////////////////////////////////////
//	octree

namespace vml
{
	namespace geo3d
	{
		
		//////////////////////////////////////////////////////////////////
		//	surface class

		class IndexedTriangle
		{
			private:

				int	Id;					// surface id

			public:

				int		  I0;			// i0 vertex index
				int		  I1;			// i1 vertex index
				int		  I2;			// i2 vertex index
				glm::vec3 Center;		// center
				glm::vec3 Normal;		// normal
				glm::vec3 EdgeNormal0;	// edge normal 
				glm::vec3 EdgeNormal1;	// edge normal
				glm::vec3 EdgeNormal2;	// edge normal
				float	  Area;

			public:

				// ------------------------------------------------------------------
				//

				void Set(int id, int i0, int i1, int i2, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
				{
					Id = id;
					I0 = i0;
					I1 = i1;
					I2 = i2;
					Center = (p0 + p1 + p2) * 0.333f;									// compute center
					Normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));				// compute normal
					EdgeNormal0 = glm::normalize(glm::cross(Normal, p2 - p0));				// compute first edge normal
					EdgeNormal1 = glm::normalize(glm::cross(Normal, p0 - p1));				// compute second edge normal
					EdgeNormal2 = glm::normalize(glm::cross(p1 - p2 + Normal, p1 - p2));	// compute third edge normal
					Area = fabs(glm::length(Normal) * 0.5f);
				}

				// ------------------------------------------

				const glm::vec3& GetCenter() const { return Center; }
				const glm::vec3& GetNormal() const { return Normal; }
				const glm::vec3& GetEdgeNromal0() const { return EdgeNormal0; }
				const glm::vec3& GetEdgeNromal1() const { return EdgeNormal1; }
				const glm::vec3& GetEdgeNromal2() const { return EdgeNormal2; }
				float GetArea() const { return Area; }

				// ------------------------------------------
				// ctro / dtro

				IndexedTriangle(int id, int i0, int i1, int i2, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
				{
					Set(id, i0, i1, i2, p0, p1, p2);
				}

				IndexedTriangle(int id, int i0, int i1, int i2)
				{
					Set(id, i0, i1, i2, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
				}

				IndexedTriangle()
				{
					Set(-1, -1, -1, -1, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
				}

				~IndexedTriangle()
				{}
		};
	}

}

////////////////////////////////////////////////////////////////////////////////////
// plane class

namespace vml
{
	namespace geo3d
	{

			////////////////////////////////////////////////////////////////////////////////////
			// plane class

			class Plane
			{

				private:

					// ------------------------------------------------------------------
					// private data 

					glm::vec3 P0;
					glm::vec3 P1;
					glm::vec3 P2;
					glm::vec3 P3;
					glm::vec3 Normal;
					float D;

				public:

					// ------------------------------------------------------------------
					// public function

					// ------------------------------------------------------------------
					// sets parameters for plane

					void Set(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
					{
						P0          = p0;
						P1          = p1;
						P2          = p2;
						P3          = p3;
						glm::vec3 u = P0 - P1;
						glm::vec3 v = P0 - P2;
						Normal      = glm::normalize(glm::cross(u, v));
						D           = -glm::dot(P0, Normal);
					}

					// ------------------------------------------------------------------
					// getters

					const glm::vec3& GetP0() const { return P0; }
					const glm::vec3& GetP1() const { return P1; }
					const glm::vec3& GetP2() const { return P2; }
					const glm::vec3& GetP3() const { return P3; }
					const glm::vec3& GetNormal() const { return Normal; }

					// ------------------------------------------------------------------
					// ctor / dtor

					Plane()
					{
						P0     = glm::vec3(0, 0, 0);
						P1     = glm::vec3(0, 0, 0);
						P2     = glm::vec3(0, 0, 0);
						P3     = glm::vec3(0, 0, 0);
						Normal = glm::vec3(0, 0, 0);
						D      = 0.0f;
					}

					Plane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
					{
						Set(p0, p1, p2, p3);
					}

					~Plane()
					{}

			};

	} // end of namespace goe3d

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// axis aligned bounding box class

namespace vml
{
	namespace geo3d
	{

		class AABBox
		{

			private:

				glm::vec3 Min;
				glm::vec3 Max;
				glm::vec3 Center;
				glm::vec3 Extents;
				glm::vec3 HalfExtents;

			public:

				// ------------------------------------------------------------------
				// getters

				const glm::vec3& GetMin()		  const { return Min; }
				const glm::vec3& GetMax()		  const { return Max; }
				const glm::vec3& GetCenter()	  const { return Center; }
				const glm::vec3& GetHalfExtents() const { return HalfExtents; }
				const glm::vec3& GetExtents()	  const { return Extents; }

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min			= bmin;
					Max			= bmax;
					Extents	    = Max - Min;
					Center	    = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				AABBox()
				{
					Min			= glm::vec3(0, 0, 0);
					Max		    = glm::vec3(0, 0, 0);
					Extents	    = glm::vec3(0, 0, 0);
					Center	    = glm::vec3(0, 0, 0);
					HalfExtents = glm::vec3(0, 0, 0);
				}

				AABBox(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min			= bmin;
					Max			= bmax;
					Extents		= Max - Min;
					Center	    = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;
				}

				~AABBox()
				{}

		};

	}	// end of geo3d namespace

}	// end of vml namespace

////////////////////////////////////////////////////////////////////////////////////
// axis aligned bounding box class

namespace vml
{
	namespace geo3d
	{

		class AOBBox
		{

			private:

				glm::vec3 Min;
				glm::vec3 Max;
				glm::vec3 Center;
				glm::vec3 Extents;						// Extents
				glm::vec3 HalfExtents;					// Half Extents
				glm::vec3 OrientedBoundingBox[8];		// Oriented bounding box vertices

			public:

				// ------------------------------------------------------------------
				// getters

				const glm::vec3& GetMin()		  const { return Min; }
				const glm::vec3& GetMax()		  const { return Max; }
				const glm::vec3& GetCenter()	  const { return Center; }
				const glm::vec3& GetHalfExtents() const { return HalfExtents; }
				const glm::vec3& GetExtents()	  const { return Extents; }

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& bmin, const glm::vec3& bmax, const float* m)
				{

					// cache transfomred vertices

					glm::vec3 minx(bmin.x * m[0], bmin.x * m[1], bmin.x * m[2]);
					glm::vec3 miny(bmin.y * m[4], bmin.y * m[5], bmin.y * m[6]);
					glm::vec3 minz(bmin.z * m[8], bmin.z * m[9], bmin.z * m[10]);

					glm::vec3 maxx(bmax.x * m[0], bmax.x * m[1], bmax.x * m[2]);
					glm::vec3 maxy(bmax.y * m[4], bmax.y * m[5], bmax.y * m[6]);
					glm::vec3 maxz(bmax.z * m[8], bmax.z * m[9], bmax.z * m[10]);

					// fill models 's axis oriented bounding box
					// the bounding box is composed of 8 vertices
					// organized in a 32 float array

					OrientedBoundingBox[0].x = minx[0] + miny[0] + minz[0] + m[12];
					OrientedBoundingBox[0].y = minx[1] + miny[1] + minz[1] + m[13];
					OrientedBoundingBox[0].z = minx[2] + miny[2] + minz[2] + m[14];

					OrientedBoundingBox[1].x = maxx[0] + miny[0] + minz[0] + m[12];
					OrientedBoundingBox[1].y = maxx[1] + miny[1] + minz[1] + m[13];
					OrientedBoundingBox[1].z = maxx[2] + miny[2] + minz[2] + m[14];

					OrientedBoundingBox[2].x = maxx[0] + maxy[0] + minz[0] + m[12];
					OrientedBoundingBox[2].y = maxx[1] + maxy[1] + minz[1] + m[13];
					OrientedBoundingBox[2].z = maxx[2] + maxy[2] + minz[2] + m[14];

					OrientedBoundingBox[3].x = minx[0] + maxy[0] + minz[0] + m[12];
					OrientedBoundingBox[3].y = minx[1] + maxy[1] + minz[1] + m[13];
					OrientedBoundingBox[3].z = minx[2] + maxy[2] + minz[2] + m[14];

					OrientedBoundingBox[4].x = minx[0] + miny[0] + maxz[0] + m[12];
					OrientedBoundingBox[4].y = minx[1] + miny[1] + maxz[1] + m[13];
					OrientedBoundingBox[4].z = minx[2] + miny[2] + maxz[2] + m[14];

					OrientedBoundingBox[5].x = maxx[0] + miny[0] + maxz[0] + m[12];
					OrientedBoundingBox[5].y = maxx[1] + miny[1] + maxz[1] + m[13];
					OrientedBoundingBox[5].z = maxx[2] + miny[2] + maxz[2] + m[14];

					OrientedBoundingBox[6].x = maxx[0] + maxy[0] + maxz[0] + m[12];
					OrientedBoundingBox[6].y = maxx[1] + maxy[1] + maxz[1] + m[13];
					OrientedBoundingBox[6].z = maxx[2] + maxy[2] + maxz[2] + m[14];

					OrientedBoundingBox[7].x = minx[0] + maxy[0] + maxz[0] + m[12];
					OrientedBoundingBox[7].y = minx[1] + maxy[1] + maxz[1] + m[13];
					OrientedBoundingBox[7].z = minx[2] + maxy[2] + maxz[2] + m[14];

					// compute axis aligned bounding box
					// the bounding box is computed searching
					// for the nearest and farthest vertices
					// in the oriented bounding box
					// accordint to vertex ordering, axis are :
					// 0-1 x axis
					// 0-3 y axis
					// 0-4 z axis

					// sets large values for lowest vertex

					Min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
					Max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

					// check for lowest x

					if (OrientedBoundingBox[0].x < Min.x) Min.x = OrientedBoundingBox[0].x;
					if (OrientedBoundingBox[1].x < Min.x) Min.x = OrientedBoundingBox[1].x;
					if (OrientedBoundingBox[2].x < Min.x) Min.x = OrientedBoundingBox[2].x;
					if (OrientedBoundingBox[3].x < Min.x) Min.x = OrientedBoundingBox[3].x;
					if (OrientedBoundingBox[4].x < Min.x) Min.x = OrientedBoundingBox[4].x;
					if (OrientedBoundingBox[5].x < Min.x) Min.x = OrientedBoundingBox[5].x;
					if (OrientedBoundingBox[6].x < Min.x) Min.x = OrientedBoundingBox[6].x;
					if (OrientedBoundingBox[7].x < Min.x) Min.x = OrientedBoundingBox[7].x;

					// check for lowest y

					if (OrientedBoundingBox[0].y < Min.y) Min.y = OrientedBoundingBox[0].y;
					if (OrientedBoundingBox[1].y < Min.y) Min.y = OrientedBoundingBox[1].y;
					if (OrientedBoundingBox[2].y < Min.y) Min.y = OrientedBoundingBox[2].y;
					if (OrientedBoundingBox[3].y < Min.y) Min.y = OrientedBoundingBox[3].y;
					if (OrientedBoundingBox[4].y < Min.y) Min.y = OrientedBoundingBox[4].y;
					if (OrientedBoundingBox[5].y < Min.y) Min.y = OrientedBoundingBox[5].y;
					if (OrientedBoundingBox[6].y < Min.y) Min.y = OrientedBoundingBox[6].y;
					if (OrientedBoundingBox[7].y < Min.y) Min.y = OrientedBoundingBox[7].y;

					// check for lowest z

					if (OrientedBoundingBox[0].z < Min.z) Min.z = OrientedBoundingBox[0].z;
					if (OrientedBoundingBox[1].z < Min.z) Min.z = OrientedBoundingBox[1].z;
					if (OrientedBoundingBox[2].z < Min.z) Min.z = OrientedBoundingBox[2].z;
					if (OrientedBoundingBox[3].z < Min.z) Min.z = OrientedBoundingBox[3].z;
					if (OrientedBoundingBox[4].z < Min.z) Min.z = OrientedBoundingBox[4].z;
					if (OrientedBoundingBox[5].z < Min.z) Min.z = OrientedBoundingBox[5].z;
					if (OrientedBoundingBox[6].z < Min.z) Min.z = OrientedBoundingBox[6].z;
					if (OrientedBoundingBox[7].z < Min.z) Min.z = OrientedBoundingBox[7].z;

					// check for highest x

					if (OrientedBoundingBox[0].x > Max.x) Max.x = OrientedBoundingBox[0].x;
					if (OrientedBoundingBox[1].x > Max.x) Max.x = OrientedBoundingBox[1].x;
					if (OrientedBoundingBox[2].x > Max.x) Max.x = OrientedBoundingBox[2].x;
					if (OrientedBoundingBox[3].x > Max.x) Max.x = OrientedBoundingBox[3].x;
					if (OrientedBoundingBox[4].x > Max.x) Max.x = OrientedBoundingBox[4].x;
					if (OrientedBoundingBox[5].x > Max.x) Max.x = OrientedBoundingBox[5].x;
					if (OrientedBoundingBox[6].x > Max.x) Max.x = OrientedBoundingBox[6].x;
					if (OrientedBoundingBox[7].x > Max.x) Max.x = OrientedBoundingBox[7].x;

					// check for highest y

					if (OrientedBoundingBox[0].y > Max.y) Max.y = OrientedBoundingBox[0].y;
					if (OrientedBoundingBox[1].y > Max.y) Max.y = OrientedBoundingBox[1].y;
					if (OrientedBoundingBox[2].y > Max.y) Max.y = OrientedBoundingBox[2].y;
					if (OrientedBoundingBox[3].y > Max.y) Max.y = OrientedBoundingBox[3].y;
					if (OrientedBoundingBox[4].y > Max.y) Max.y = OrientedBoundingBox[4].y;
					if (OrientedBoundingBox[5].y > Max.y) Max.y = OrientedBoundingBox[5].y;
					if (OrientedBoundingBox[6].y > Max.y) Max.y = OrientedBoundingBox[6].y;
					if (OrientedBoundingBox[7].y > Max.y) Max.y = OrientedBoundingBox[7].y;

					// check for highest z

					if (OrientedBoundingBox[0].z > Max.z) Max.z = OrientedBoundingBox[0].z;
					if (OrientedBoundingBox[1].z > Max.z) Max.z = OrientedBoundingBox[1].z;
					if (OrientedBoundingBox[2].z > Max.z) Max.z = OrientedBoundingBox[2].z;
					if (OrientedBoundingBox[3].z > Max.z) Max.z = OrientedBoundingBox[3].z;
					if (OrientedBoundingBox[4].z > Max.z) Max.z = OrientedBoundingBox[4].z;
					if (OrientedBoundingBox[5].z > Max.z) Max.z = OrientedBoundingBox[5].z;
					if (OrientedBoundingBox[6].z > Max.z) Max.z = OrientedBoundingBox[6].z;
					if (OrientedBoundingBox[7].z > Max.z) Max.z = OrientedBoundingBox[7].z;

					Extents     = Max - Min;
					Center      = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;

				}

				// ------------------------------------------------------------------
				// sets parameters for plane

				void Set(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
						 const glm::vec3& p4, const glm::vec3& p5, const glm::vec3& p6, const glm::vec3& p7)

				{

					Min         = p0;
					Max         = p6;
					Extents     = Max - Min;
					Center      = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;

					OrientedBoundingBox[0] = p0;
					OrientedBoundingBox[1] = p1;
					OrientedBoundingBox[2] = p2;
					OrientedBoundingBox[3] = p3;
					OrientedBoundingBox[4] = p4;
					OrientedBoundingBox[5] = p5;
					OrientedBoundingBox[6] = p6;
					OrientedBoundingBox[7] = p7;

				}

				// ------------------------------------------------------------------

				const glm::vec3& operator[](size_t pos) const 
				{
					return OrientedBoundingBox[pos]; 
				}

				// ------------------------------------------------------------------
				// ctor / dtor

				AOBBox()
				{
					Min         = glm::vec3(0, 0, 0);
					Max         = glm::vec3(0, 0, 0);
					Extents     = glm::vec3(0, 0, 0);
					Center      = glm::vec3(0, 0, 0);
					HalfExtents = glm::vec3(0, 0, 0);

					OrientedBoundingBox[0] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[1] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[2] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[3] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[4] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[5] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[6] = glm::vec3(0, 0, 0);
					OrientedBoundingBox[7] = glm::vec3(0, 0, 0);
				}

				AOBBox(const glm::vec3& bmin, const glm::vec3& bmax)
				{
					Min         = bmin;
					Max         = bmax;
					Extents     = Max - Min;
					Center      = (Max + Min) * 0.5f;
					HalfExtents = (Max - Min) * 0.5f;

					OrientedBoundingBox[0] = glm::vec3(bmin.x, bmin.y, bmin.z);
					OrientedBoundingBox[1] = glm::vec3(bmax.x, bmin.y, bmin.z);
					OrientedBoundingBox[2] = glm::vec3(bmax.x, bmax.y, bmin.z);
					OrientedBoundingBox[3] = glm::vec3(bmin.x, bmax.y, bmin.z);
					OrientedBoundingBox[4] = glm::vec3(bmin.x, bmin.y, bmax.z);
					OrientedBoundingBox[5] = glm::vec3(bmax.x, bmin.y, bmax.z);
					OrientedBoundingBox[6] = glm::vec3(bmax.x, bmax.y, bmax.z);
					OrientedBoundingBox[7] = glm::vec3(bmin.x, bmax.y, bmax.z);

				}

				~AOBBox()
				{}

		};

	}	// end of geo3d namespace

}	// end of vml namespace


////////////////////////////////////////////////////////////////////////////////////
// sphere class

namespace vml
{
	namespace geo3d
	{

			class Sphere
			{

				private:

					// ------------------------------------------------------------------
					// private data 

					glm::vec3 P0;
					float     Radius;
					float     Diameter;

				public:

					// ------------------------------------------------------------------
					// public function

					// ------------------------------------------------------------------
					// sets parameters for plane

					void Set(const glm::vec3& p0, const float radius)
					{
						P0       = p0;
						Radius   = radius;
						Diameter = 2.0f * Radius;
					}

					// ------------------------------------------------------------------
					// getters

					const glm::vec3& GetP0() const { return P0; }
					float GetRadius() const { return Radius; }
					float GetDiameter() const { return Diameter; }

					// ------------------------------------------------------------------
					// ctor / dtor

					Sphere()
					{
						P0       = glm::vec3(0, 0, 0);
						Radius   = 0.0f;
						Diameter = 0.0f;
					}

					Sphere(const glm::vec3& p0, const float radius)
					{
						Set(p0, radius);
					}

					~Sphere()
					{}

			};


	}	// end of geo3d namespace

}	// end of vml namespace

