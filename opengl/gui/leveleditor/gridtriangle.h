#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////
//

namespace fa2040
{
	namespace tools
	{
		class GridTriangle
		{
			private:

			public:
				
				// -----------------------------------------------------------------
				//

				glm::vec2    P0;
				glm::vec2    P1;
				glm::vec2    P2;
				glm::vec2    N0;
				glm::vec2    N1;
				glm::vec2    N2;
				glm::vec2	 BMin;
				glm::vec2	 BMax;
				glm::vec2	 Center;
				unsigned int Type;
				int			 Id;
				int			 I0;
				int			 I1;
				int			 I2;

				std::vector<GridTriangle*> Links;

				// --------------------------------------------------
				//

				static const unsigned int INTERNAL_TRIANGLE            = 0;
				static const unsigned int EXTERNAL_TRIANGLE            = 1;
				static const unsigned int BORDER_TRIANGLE              = 2;
				static const unsigned int TOP_TRIANGLE                 = 3;
				static const unsigned int TOP_RIGHT_CORNER_TRIANGLE    = 4;
				static const unsigned int RIGHT_UPPER_CORNER_TRIANGLE  = 5;
				static const unsigned int RIGHT_TRIANGLE               = 6;
				static const unsigned int BOTTOM_RIGHT_CORNER_TRIANGLE = 7;
				static const unsigned int RIGHT_LOWER_CORNER_TRIANGLE  = 8;
				static const unsigned int BOTTOM_TRIANGLE              = 9;
				static const unsigned int BOTTOM_LEFT_CORNER_TRIANGLE  = 10;
				static const unsigned int LEFT_LOWER_CORNER_TRIANGLE   = 11;
				static const unsigned int LEFT_TRIANGLE                = 12;
				static const unsigned int LEFT_UPPER_CORNER_TRIANGLE   = 13;
				static const unsigned int TOP_LEFT_CORNER_TRIANGLE     = 14;
				static const unsigned int RIGHT_LEFT_CORNER_TRIANGLE   = 15;
				static const unsigned int LEFT_RIGHT_CORNER_TRIANGLE   = 16;

				// -----------------------------------------------------------------
				//

				const glm::vec2& GetP0() const { return P0; }
				const glm::vec2& GetP1() const { return P1; }
				const glm::vec2& GetP2() const { return P2; }
				constexpr unsigned int GetType() const { return Type; }
				constexpr int GetId() const { return Id; }
				constexpr int GetI0() const { return I0; }
				constexpr int GetI1() const { return I1; }
				constexpr int GetI2() const { return I2; }

				// --------------------------------------------------
				//

				const std::string GetTypeString() const
				{
					switch (Type)
					{
						case INTERNAL_TRIANGLE            : return "Internal triangle";
						case EXTERNAL_TRIANGLE            : return "External triangle";
						case BORDER_TRIANGLE              : return "Border Triangle";
						case TOP_TRIANGLE                 : return "Top Triangle";
						case TOP_RIGHT_CORNER_TRIANGLE    : return "Top Right Corner triangle";
						case RIGHT_UPPER_CORNER_TRIANGLE  : return "Right Upper Corner Triangle";
						case RIGHT_TRIANGLE               : return "Right Triangle";
						case BOTTOM_RIGHT_CORNER_TRIANGLE : return "Bottom Right Corner Triangle";
						case RIGHT_LOWER_CORNER_TRIANGLE  : return "Right Lower Corner Triaangle";
						case BOTTOM_TRIANGLE              : return "Bottom Triangle";
						case BOTTOM_LEFT_CORNER_TRIANGLE  : return "Bottom Left Corner Triangle";
						case LEFT_LOWER_CORNER_TRIANGLE   : return "Left Lower Corner Triangle";
						case LEFT_TRIANGLE                : return "Left Triangle";
						case LEFT_UPPER_CORNER_TRIANGLE   : return "left Upper Corner triangle";
						case TOP_LEFT_CORNER_TRIANGLE     : return "Top Left Corner Triangle";
						case RIGHT_LEFT_CORNER_TRIANGLE   : return "Right Left Corner Triangle";
						case LEFT_RIGHT_CORNER_TRIANGLE   : return "Left Right Corner Triangle";
					}

					return "undefined";
				}

				// -----------------------------------------------------------------
				//

				void ComputeMetrics()
				{

					Center = (P0 + P1 + P2) * 0.333f;

					glm::vec2 u = glm::vec2(P1.x - P0.x, P1.y - P0.y);
					glm::vec2 v = glm::vec2(P2.x - P1.x, P2.y - P1.y);
					glm::vec2 w = glm::vec2(P0.x - P2.x, P0.y - P2.y);

					N0 = glm::normalize(glm::vec2(u.y, -u.x));
					N1 = glm::normalize(glm::vec2(v.y, -v.x));
					N2 = glm::normalize(glm::vec2(w.y, -w.x));

					// find minimum value between p0.x, p1.x, p2.x

					BMin.x = P0.x;
					if (P1.x < BMin.x) BMin.x = P1.x;
					if (P2.x < BMin.x) BMin.x = P2.x;

					// find minimum value between p0.y, p1.y, p2.y

					BMin.y = P0.y;
					if (P1.y < BMin.y) BMin.y = P1.y;
					if (P2.y < BMin.y) BMin.y = P2.y;

					// find maximum value between p0.x, p1.x, p2.x

					BMax.x = P0.x;
					if (P1.x > BMax.x) BMax.x = P1.x;
					if (P2.x > BMax.x) BMax.x = P2.x;

					// find maximum value between p0.y, p1.y, p2.y

					BMax.y = P0.y;
					if (P1.y > BMax.y) BMax.y = P1.y;
					if (P2.y > BMax.y) BMax.y = P2.y;
				}

				// -----------------------------------------------------------------
				//

				void ClearLinks()
				{
					Links.clear();
				}

				// -----------------------------------------------------------------
				//

				void AddTriangleToSide(GridTriangle* tri) { Links.emplace_back(tri); }
				
				// -----------------------------------------------------------------
				// ctor / dtor

				GridTriangle()
				{
					
					P0 = glm::vec2(0, 0);
					P1 = glm::vec2(0, 0);
					P2 = glm::vec2(0, 0);

					N0 = glm::vec2(0, 0);
					N1 = glm::vec2(0, 0);
					N2 = glm::vec2(0, 0);

					BMin   = glm::vec2(0, 0);
					BMax   = glm::vec2(0, 0);
					Center = glm::vec2(0, 0);

					Id = -1;
					I0 = -1;
					I1 = -1;
					I2 = -1;

					Type = -1;
					
				}

				GridTriangle(int id, int i0, int i1, int i2, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, int type)
				{
					
					P0 = p0;
					P1 = p1;
					P2 = p2;
					Id = id;
					I0 = i0;
					I1 = i1;
					I2 = i2;
					Type = type;
					ComputeMetrics();
					
				}

				GridTriangle(const GridTriangle&) = default;
				GridTriangle& operator=(const GridTriangle&) = default;

				~GridTriangle()
				{}

		};
	}
}
