#pragma once


namespace fa2040
{
	namespace tools
	{

		// panel for level2d inspector

		class Level2dPanel
		{

		private:

			enum ContentsType
			{
				CT_Text,
				CT_Button,
				CT_SmallButton,
				CT_FillButton,
				CT_Selectable,
				CT_SelectableSpanRow
			};

			// enums for triangle layer debuf window

			enum TriangleItemColumnID
			{
				TriangleItemColumnID_TRIANGLE_ID,
				TriangleItemColumnID_TRIANGLE_P0,
				TriangleItemColumnID_TRIANGLE_P1,
				TriangleItemColumnID_TRIANGLE_P2,
				TriangleItemColumnID_TRIANGLE_LINKS,
			};

			// class for storing info about triangle layer debug window

			class TriangleItem
			{
			public:

				int  ID;
				int	 P0, P1, P2;
				char IDString[32] = { 0 };
				char P0String[32] = { 0 };
				char P1String[32] = { 0 };
				char P2String[32] = { 0 };

				TriangleItem(int id, int p0, int p1, int p2) : ID(id), P0(p0), P1(p1), P2(p2)
				{
					sprintf_s(IDString, 32, "%d", id);
					sprintf_s(P0String, 32, "%d", p0);
					sprintf_s(P1String, 32, "%d", p1);
					sprintf_s(P2String, 32, "%d", p2);
				}

				~TriangleItem()
				{
				}

			};

			// class for storing info about triangle layer debug window

			class BorderItem
			{
			public:

				int  ID;
				int	 P0, P1;
				char IDString[32] = { 0 };
				char P0String[32] = { 0 };
				char P1String[32] = { 0 };

				BorderItem(int id, int p0, int p1) : ID(id), P0(p0), P1(p1)
				{
					sprintf_s(IDString, 32, "%d", id);
					sprintf_s(P0String, 32, "%d", p0);
					sprintf_s(P1String, 32, "%d", p1);
				}

				~BorderItem()
				{
				}

			};

			ImVector<int>			  MainTriangleSelection;
			ImVector<int>			  ExternalTriangleSelection;
			ImVector<int>			  BorderSelection;
			std::vector<TriangleItem> MainTriangleItems;
			std::vector<TriangleItem> ExternalTriangleItems;
			std::vector<BorderItem>   BorderItems;

			// ---------------------------------------------------------------

			void TextCentered(const std::string& text)
			{
				int windowWidth = ImGui::GetWindowSize().x;
				int textWidth = ImGui::CalcTextSize(text.c_str()).x;
				ImGui::SetCursorPosX((float)(windowWidth - textWidth) / 2);
				ImGui::Text(text.c_str());
			}

		public:

			bool WindowOpen;

			// ---------------------------------------------------------------

			void Draw(const std::string& title, fa2040::tools::Level2dMap* level2dmap)
			{

				ImGui::Begin(&title[0]);

				// texture info

				if (ImGui::CollapsingHeader("Texture Info"))
				{

					const vml::textures::Texture* texture = level2dmap->GetTexture();

					if (texture)
					{
						unsigned int flags = ImGuiTableFlags_Resizable + ImGuiTableFlags_Borders;

						if (ImGui::BeginTable("table1", 2, flags))
						{

							ImGui::TableSetupColumn("Attributes");
							ImGui::TableSetupColumn("Value");
							ImGui::TableHeadersRow();

							std::string TextureWidthString = std::to_string(texture->GetWidth());
							std::string TextureHeightString = std::to_string(texture->GetHeight());
							std::string TextureAspectRatioString = std::to_string((float)texture->GetHeight() / (float)texture->GetWidth());
							std::string TextureBppString = std::to_string(texture->GetBPP());
							std::string TextureIdString = std::to_string(texture->GetID());
							std::string TextureSizeString = std::to_string(texture->GetWidth() * texture->GetHeight());
							std::string TextureMinificationString;
							std::string TextureMagnificationString;
							std::string TextureFormatString;
							std::string RepeatSString;
							std::string RepeatTString;
							std::string ReleaseDataString;

							int mini = texture->GetMinificationFilter();
							int maxi = texture->GetMagnificationFilter();
							int format = texture->GetFormat();
							int repeats = texture->GetRepeatS();
							int repeatt = texture->GetRepeatT();
							int releasedata = texture->IsDataPreserved();

							std::string fullpathfilename = texture->GetResourceFileName();
							std::string filename = vml::strings::SplitPath::GetName(texture->GetResourceFileName());

							if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST)				 TextureMinificationString = "TEXTURE_FILTER_MIN_NEAREST";
							if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR)				 TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR";
							if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST_MIPMAP)		 TextureMinificationString = "TEXTURE_FILTER_MIN_NEAREST_MIPMAP";
							if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR_MIPMAP)		 TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR_MIPMAP";
							if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR) TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR";
							if (maxi == vml::textures::Texture::TEXTURE_FILTER_MAG_NEAREST) TextureMagnificationString = "TEXTURE_FILTER_MAG_NEAREST";
							if (maxi == vml::textures::Texture::TEXTURE_FILTER_MAG_LINEAR)  TextureMagnificationString = "TEXTURE_FILTER_MAG_LINEAR";
							if (format == vml::textures::Texture::TEXTURE_RED)		  TextureFormatString = "GL_RED";
							if (format == vml::textures::Texture::TEXTURE_RGB)		  TextureFormatString = "GL_RGB";
							if (format == vml::textures::Texture::TEXTURE_SRGB)	 	  TextureFormatString = "GL_SRGB";
							if (format == vml::textures::Texture::TEXTURE_RGBA)		  TextureFormatString = "GL_RGBA";
							if (format == vml::textures::Texture::TEXTURE_SRGB_ALPHA) TextureFormatString = "GL_SRGB_ALPHA";
							if (repeats == vml::textures::Texture::TEXTURE_REPEAT_S)          RepeatSString = "TEXTURE_REPEAT_S";
							if (repeats == vml::textures::Texture::TEXTURE_MIRRORED_S)        RepeatSString = "TEXTURE_MIRRORED_S";
							if (repeats == vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S)	  RepeatSString = "TEXTURE_CLAMP_TO_EDGE_S";
							if (repeats == vml::textures::Texture::TEXTURE_CLAMP_TO_BORDER_S) RepeatSString = "TEXTURE_CLAMP_TO_BORDER_S";
							if (repeatt == vml::textures::Texture::TEXTURE_REPEAT_S)          RepeatTString = "TEXTURE_REPEAT_S";
							if (repeatt == vml::textures::Texture::TEXTURE_MIRRORED_S)        RepeatTString = "TEXTURE_MIRRORED_S";
							if (repeatt == vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S)	  RepeatTString = "TEXTURE_CLAMP_TO_EDGE_S";
							if (repeatt == vml::textures::Texture::TEXTURE_CLAMP_TO_BORDER_S) RepeatTString = "TEXTURE_CLAMP_TO_BORDER_S";
							if (releasedata == vml::textures::Texture::TEXTURE_RELEASE_DATA_TRUE) ReleaseDataString = "YES";
							else ReleaseDataString = "NO";

							ImGui::TableNextColumn();
							ImGui::Text("Bitmap");
							ImGui::TableNextColumn();
							ImGui::Image((void*)(intptr_t)texture->GetID(), { 128,128 }, { 0, 1 }, { 1, 0 });

							ImGui::TableNextColumn();
							ImGui::Text("FileName");
							ImGui::TableNextColumn();
							ImGui::Text(&filename[0]);

							ImGui::TableNextColumn();
							ImGui::Text("FullPath");
							ImGui::TableNextColumn();
							ImGui::Text(&fullpathfilename[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Width in pixels");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureWidthString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Heigth in pixels");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureHeightString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Aspect Ratio");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureAspectRatioString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Size in bytes");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureSizeString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Byte per pixel");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureBppString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("TextureId");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureIdString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Minification Filter");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureMinificationString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Magnification Filter");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureMagnificationString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Format");
							ImGui::TableNextColumn();
							ImGui::Text(&TextureFormatString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("RepeatS");
							ImGui::TableNextColumn();
							ImGui::Text(&RepeatSString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("RepeatT");
							ImGui::TableNextColumn();
							ImGui::Text(&RepeatTString[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Texture Data Released");
							ImGui::TableNextColumn();
							ImGui::Text(&ReleaseDataString[0]);

							ImGui::EndTable();
						}
					}
					else
					{
						TextCentered("Texture has not been loaded");
					}

				}

				// main triangle layer

				if (ImGui::CollapsingHeader("Main Triangle Layer"))
				{

					bool levelcompiled = false;
					if (level2dmap->Level2dGenerator)
						if (level2dmap->Level2dGenerator->IsCompiled()) levelcompiled = true;

					if (levelcompiled)
					{

						fa2040::tools::TriangleLayer& trilayer = level2dmap->Level2dGenerator->TriangleLayer;

						unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

						if (ImGui::BeginTable("table_0", 2, flags))
						{
							std::string tmp;

							ImGui::TableSetupColumn("Attributes");
							ImGui::TableSetupColumn("Value");
							ImGui::TableHeadersRow();

							ImGui::TableNextColumn();
							ImGui::Text("Vertices");
							ImGui::TableNextColumn();
							tmp = std::to_string((int)trilayer.GetVerticesCount());
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Triangles");
							ImGui::TableNextColumn();
							tmp = std::to_string((int)trilayer.GetTrianglesCount());
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxMin");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxMin().x) + " , " + std::to_string(trilayer.GetBoundingBoxMin().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxMax");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxMax().x) + " , " + std::to_string(trilayer.GetBoundingBoxMax().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxExtents");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxExtents().x) + " , " + std::to_string(trilayer.GetBoundingBoxExtents().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBox Aspect Ratio");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxExtents().y / trilayer.GetBoundingBoxExtents().x);
							ImGui::Text(&tmp[0]);

							ImGui::EndTable();
						}

						// Using those as a base value to create width/height that are factor of the size of our font

						float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
						float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
						ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
							ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
							ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

						bool outer_size_enabled = true;
						float inner_width_with_scroll = 0.0f;
						float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
						float row_min_height = 0;
						ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

						// main triangle layer

						//	if (ImGui::CollapsingHeader("Indexed Triangle List"))

						if (ImGui::BeginTable("table_advanced_0", 4, mflags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
						{

							// Declare columns

							ImGui::TableSetupColumn("Triangle ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_ID);
							ImGui::TableSetupColumn("P0", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P0);
							ImGui::TableSetupColumn("P1", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P1);
							ImGui::TableSetupColumn("P2", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P2);

							ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
							ImGui::TableHeadersRow();
							int contents_type = CT_SelectableSpanRow;
							ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;

							// Demonstrate using clipper for large vertical lists

							ImGuiListClipper clipper;

							clipper.Begin((int)MainTriangleItems.size());

							while (clipper.Step())
							{
								for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
								{

									TriangleItem* item = &MainTriangleItems[row_n];

									ImGui::PushID(item->ID);
									ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

									ImGui::TableSetColumnIndex(0);
									//sprintf_s(label, 32, "%d", item->TriangleID);

									const bool item_is_selected = MainTriangleSelection.contains(item->ID);

									if (ImGui::Selectable(item->IDString, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
									{
										if (ImGui::GetIO().KeyCtrl)
										{
											if (item_is_selected)
												MainTriangleSelection.find_erase_unsorted(item->ID);
											else
												MainTriangleSelection.push_back(item->ID);
										}
										else
										{
											MainTriangleSelection.clear();
											MainTriangleSelection.push_back(item->ID);
										}
									}

									ImGui::TableSetColumnIndex(1);
									ImGui::TextUnformatted(item->P0String);

									ImGui::TableSetColumnIndex(2);
									ImGui::TextUnformatted(item->P1String);

									ImGui::TableSetColumnIndex(3);
									ImGui::TextUnformatted(item->P2String);

									ImGui::PopID();

								}
							}

							ImGui::EndTable();
						}

					}
					else
					{
						if (!level2dmap->Level2dGenerator)
							TextCentered("Map has not been loaded");
						else if (!level2dmap->Level2dGenerator->IsCompiled())
							TextCentered("Map has not been compiled");
					}

				}

				// external triangle layer

				if (ImGui::CollapsingHeader("External Triangle Layer"))
				{
					bool levelcompiled = false;
					if (level2dmap->Level2dGenerator)
						if (level2dmap->Level2dGenerator->IsCompiled()) levelcompiled = true;

					if (levelcompiled)
					{

						fa2040::tools::TriangleLayer& trilayer = level2dmap->Level2dGenerator->ExternalTriangleLayer2;

						unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

						if (ImGui::BeginTable("table_1", 2, flags))
						{
							std::string tmp;

							ImGui::TableSetupColumn("Attributes");
							ImGui::TableSetupColumn("Value");
							ImGui::TableHeadersRow();

							ImGui::TableNextColumn();
							ImGui::Text("Vertices");
							ImGui::TableNextColumn();
							tmp = std::to_string((int)trilayer.GetVerticesCount());
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Triangles");
							ImGui::TableNextColumn();
							tmp = std::to_string((int)trilayer.GetTrianglesCount());
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxMin");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxMin().x) + " , " + std::to_string(trilayer.GetBoundingBoxMin().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxMax");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxMax().x) + " , " + std::to_string(trilayer.GetBoundingBoxMax().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxExtents");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxExtents().x) + " , " + std::to_string(trilayer.GetBoundingBoxExtents().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBox Aspect Ratio");
							ImGui::TableNextColumn();
							tmp = std::to_string(trilayer.GetBoundingBoxExtents().y / trilayer.GetBoundingBoxExtents().x);
							ImGui::Text(&tmp[0]);

							ImGui::EndTable();
						}

						// Using those as a base value to create width/height that are factor of the size of our font

						float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
						float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
						ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
							ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
							ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

						bool outer_size_enabled = true;
						float inner_width_with_scroll = 0.0f;
						float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
						float row_min_height = 0;
						ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

						// main triangle layer

						if (ImGui::BeginTable("table_advanced_1", 4, mflags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
						{

							// Declare columns

							ImGui::TableSetupColumn("Triangle ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_ID);
							ImGui::TableSetupColumn("P0", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P0);
							ImGui::TableSetupColumn("P1", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P1);
							ImGui::TableSetupColumn("P2", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P2);

							ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
							ImGui::TableHeadersRow();
							int contents_type = CT_SelectableSpanRow;
							ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;

							// Demonstrate using clipper for large vertical lists

							ImGuiListClipper clipper;

							clipper.Begin((int)ExternalTriangleItems.size());

							while (clipper.Step())
							{
								for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
								{

									TriangleItem* item = &ExternalTriangleItems[row_n];

									ImGui::PushID(item->ID);
									ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

									ImGui::TableSetColumnIndex(0);

									const bool item_is_selected = ExternalTriangleSelection.contains(item->ID);

									if (ImGui::Selectable(item->IDString, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
									{
										if (ImGui::GetIO().KeyCtrl)
										{
											if (item_is_selected)
												ExternalTriangleSelection.find_erase_unsorted(item->ID);
											else
												ExternalTriangleSelection.push_back(item->ID);
										}
										else
										{
											ExternalTriangleSelection.clear();
											ExternalTriangleSelection.push_back(item->ID);
										}
									}

									ImGui::TableSetColumnIndex(1);
									ImGui::TextUnformatted(item->P0String);

									ImGui::TableSetColumnIndex(2);
									ImGui::TextUnformatted(item->P1String);

									ImGui::TableSetColumnIndex(3);
									ImGui::TextUnformatted(item->P2String);

									ImGui::PopID();

								}
							}

							ImGui::EndTable();
						}

					}
					else
					{
						if (!level2dmap->Level2dGenerator)
							TextCentered("Map has not been loaded");
						else if (!level2dmap->Level2dGenerator->IsCompiled())
							TextCentered("Map has not been compiled");
					}

				}

				// border triangle layer

				if (ImGui::CollapsingHeader("Border Triangle Layer"))
				{
					bool levelcompiled = false;
					if (level2dmap->Level2dGenerator)
						if (level2dmap->Level2dGenerator->IsCompiled()) levelcompiled = true;

					if (levelcompiled)
					{

						fa2040::tools::BorderLayer& borderlayer = level2dmap->Level2dGenerator->BorderLayer;

						unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

						if (ImGui::BeginTable("table_2", 2, flags))
						{

							std::string tmp;

							ImGui::TableSetupColumn("Attributes");
							ImGui::TableSetupColumn("Value");
							ImGui::TableHeadersRow();

							ImGui::TableNextColumn();
							ImGui::Text("Vertices");
							ImGui::TableNextColumn();
							tmp = std::to_string((int)borderlayer.GetVerticesCount());
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("Edges");
							ImGui::TableNextColumn();
							tmp = std::to_string((int)borderlayer.GetEdgesCount());
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxMin");
							ImGui::TableNextColumn();
							tmp = std::to_string(borderlayer.GetBoundingBoxMin().x) + " , " + std::to_string(borderlayer.GetBoundingBoxMin().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxMax");
							ImGui::TableNextColumn();
							tmp = std::to_string(borderlayer.GetBoundingBoxMax().x) + " , " + std::to_string(borderlayer.GetBoundingBoxMax().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBoxExtents");
							ImGui::TableNextColumn();
							tmp = std::to_string(borderlayer.GetBoundingBoxExtents().x) + " , " + std::to_string(borderlayer.GetBoundingBoxExtents().y);
							ImGui::Text(&tmp[0]);

							ImGui::TableNextColumn();
							ImGui::Text("BoundingBox Aspect Ratio");
							ImGui::TableNextColumn();
							tmp = std::to_string(borderlayer.GetBoundingBoxExtents().y / borderlayer.GetBoundingBoxExtents().x);
							ImGui::Text(&tmp[0]);

							ImGui::EndTable();

						}

						// Using those as a base value to create width/height that are factor of the size of our font

						float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
						float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
						ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
							ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
							ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

						bool outer_size_enabled = true;
						float inner_width_with_scroll = 0.0f;
						float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
						float row_min_height = 0;
						ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);

						// main triangle layer

						if (ImGui::BeginTable("table_advanced_2", 3, mflags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
						{

							// Declare columns

							ImGui::TableSetupColumn("Edge ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_ID);
							ImGui::TableSetupColumn("P0", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P0);
							ImGui::TableSetupColumn("P1", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, TriangleItemColumnID_TRIANGLE_P1);

							ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
							ImGui::TableHeadersRow();
							int contents_type = CT_SelectableSpanRow;
							ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap : ImGuiSelectableFlags_None;

							// Demonstrate using clipper for large vertical lists

							ImGuiListClipper clipper;

							clipper.Begin((int)BorderItems.size());

							while (clipper.Step())
							{
								for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
								{

									BorderItem* item = &BorderItems[row_n];

									ImGui::PushID(item->ID);
									ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

									ImGui::TableSetColumnIndex(0);

									const bool item_is_selected = BorderSelection.contains(item->ID);

									if (ImGui::Selectable(item->IDString, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
									{
										if (ImGui::GetIO().KeyCtrl)
										{
											if (item_is_selected)
												BorderSelection.find_erase_unsorted(item->ID);
											else
												BorderSelection.push_back(item->ID);
										}
										else
										{
											BorderSelection.clear();
											BorderSelection.push_back(item->ID);
										}
									}

									ImGui::TableSetColumnIndex(1);
									ImGui::TextUnformatted(item->P0String);

									ImGui::TableSetColumnIndex(2);
									ImGui::TextUnformatted(item->P1String);

									ImGui::PopID();

								}
							}
							ImGui::EndTable();
						}

					}
					else
					{
						if (!level2dmap->Level2dGenerator)
							TextCentered("Map has not been loaded");
						else if (!level2dmap->Level2dGenerator->IsCompiled())
							TextCentered("Map has not been compiled");
					}
				}

				ImGui::End();
			}

			// --------------------------------------------------------------------------

			void InitData(fa2040::tools::Level2dMap* level2dmap)
			{
				MainTriangleItems.clear();
				ExternalTriangleItems.clear();
				BorderItems.clear();

				if (!level2dmap->Level2dGenerator)
					return;

				// cache maintriangle layer

				fa2040::tools::TriangleLayer& maintrilayer = level2dmap->Level2dGenerator->TriangleLayer;

				for (int i = 0; i < maintrilayer.GetTrianglesCount(); ++i)
				{
					fa2040::tools::GridTriangle tri = maintrilayer.GetTriangleAt(i);
					MainTriangleItems.emplace_back(TriangleItem(i, tri.GetI0(), tri.GetI1(), tri.GetI2()));
				}

				// cache external triangle layer

				fa2040::tools::TriangleLayer& externaltrilayer = level2dmap->Level2dGenerator->ExternalTriangleLayer2;

				for (int i = 0; i < externaltrilayer.GetTrianglesCount(); ++i)
				{
					fa2040::tools::GridTriangle tri = externaltrilayer.GetTriangleAt(i);
					ExternalTriangleItems.emplace_back(TriangleItem(i, tri.GetI0(), tri.GetI1(), tri.GetI2()));
				}

				// cache border layer

				fa2040::tools::BorderLayer& borderlayer = level2dmap->Level2dGenerator->BorderLayer;

				for (int i = 0; i < borderlayer.GetEdgesCount(); ++i)
				{
					BorderItems.emplace_back(BorderItem(i, borderlayer.GetI0(i), borderlayer.GetI1(i)));
				}

			}

			// --------------------------------------------------------
			// ctor / dtor

			Level2dPanel()
			{
				WindowOpen = false;
			}

			~Level2dPanel()
			{
			}

		};
	}
}
