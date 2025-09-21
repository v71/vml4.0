#pragma once

namespace fa2040
{
	namespace tools
	{

		class GridBlock
		{
			private:

				// --------------------------------------------------
				// private data

				int Width, Height;			// width and blocks' height
				int I1, J1, I2, J2;			// block cell coordinates
				int Id;
				int Area;

			public:

				// --------------------------------------------------
				// getters

				constexpr int GetWidth() const { return Width; }
				constexpr int GetHeight() const { return Height; }
				constexpr int GetI1() const { return I1; }
				constexpr int GetJ1() const { return J1; }
				constexpr int GetI2() const { return I2; }
				constexpr int GetJ2() const { return J2; }
				constexpr int GetArea() const { return Area; }

				// --------------------------------------------------
				// ctor / dtor

				GridBlock()
				{
					Id = -1;
					I1 = -1;
					J1 = -1;
					I2 = -1;
					J2 = -1;
					Width = 0;
					Height = 0;
					Area = 0.0f;
				}

				GridBlock(const int id, const int i1, const int j1, const int i2, const int j2)
				{
					Id = id;
					I1 = i1;
					J1 = j1;
					I2 = i2;
					J2 = j2;
					Width = i2 - i1;
					Height = j2 - j1;
					Area = (Width + 1) * (Height + 1);
				}

				GridBlock(const GridBlock&) = default;
				GridBlock& operator=(const GridBlock&) = default;

				~GridBlock()
				{}

		};
	}
}
