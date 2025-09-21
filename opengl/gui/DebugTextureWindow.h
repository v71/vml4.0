#pragma once

namespace vml
{
	namespace gui
	{

		class TextureDataBase
		{
			private:

				// -------------------------------------------------------------------------------

				std::unordered_map<vml::textures::Texture*, std::vector< std::string >> Cache;

				// -------------------------------------------------------------------------------

				void AddToDb(vml::textures::Texture* texname, const std::string& modelname)
				{
					auto it = Cache.find(texname);

					if (it == Cache.end())
					{
						std::vector<std::string> modelnamearray;
						modelnamearray.emplace_back(modelname);
						Cache.insert(std::pair<vml::textures::Texture*, std::vector<std::string>>(texname, modelnamearray));
					}
					else
					{
						(*it).second.emplace_back(modelname);
					}
				}

				// -------------------------------------------------------------------------------

				void DrawTextureInfo(vml::textures::Texture* texture, const std::string& tableid)
				{

					unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

					if (ImGui::BeginTable(tableid.c_str(), 2, flags))
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
						if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST)				 TextureMinificationString = "TEXTURE_FILTER_MIN_NEAREST";
						if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR)				 TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR";
						if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_NEAREST_MIPMAP)		 TextureMinificationString = "TEXTURE_FILTER_MIN_NEAREST_MIPMAP";
						if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR_MIPMAP)		 TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR_MIPMAP";
						if (mini == vml::textures::Texture::TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR) TextureMinificationString = "TEXTURE_FILTER_MIN_LINEAR_MIPMAP_LINEAR";
						int maxi = texture->GetMagnificationFilter();
						if (maxi == vml::textures::Texture::TEXTURE_FILTER_MAG_NEAREST) TextureMagnificationString = "TEXTURE_FILTER_MAG_NEAREST";
						if (maxi == vml::textures::Texture::TEXTURE_FILTER_MAG_LINEAR)  TextureMagnificationString = "TEXTURE_FILTER_MAG_LINEAR";
						int format = texture->GetFormat();
						if (format == vml::textures::Texture::TEXTURE_RED)		  TextureFormatString = "GL_RED";
						if (format == vml::textures::Texture::TEXTURE_RGB)		  TextureFormatString = "GL_RGB";
						if (format == vml::textures::Texture::TEXTURE_SRGB)	 	  TextureFormatString = "GL_SRGB";
						if (format == vml::textures::Texture::TEXTURE_RGBA)		  TextureFormatString = "GL_RGBA";
						if (format == vml::textures::Texture::TEXTURE_SRGB_ALPHA) TextureFormatString = "GL_SRGB_ALPHA";
						int repeats = texture->GetRepeatS();
						if (repeats == vml::textures::Texture::TEXTURE_REPEAT_S)          RepeatSString = "TEXTURE_REPEAT_S";
						if (repeats == vml::textures::Texture::TEXTURE_MIRRORED_S)        RepeatSString = "TEXTURE_MIRRORED_S";
						if (repeats == vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S)	  RepeatSString = "TEXTURE_CLAMP_TO_EDGE_S";
						if (repeats == vml::textures::Texture::TEXTURE_CLAMP_TO_BORDER_S) RepeatSString = "TEXTURE_CLAMP_TO_BORDER_S";
						int repeatt = texture->GetRepeatT();
						if (repeatt == vml::textures::Texture::TEXTURE_REPEAT_S)          RepeatTString = "TEXTURE_REPEAT_S";
						if (repeatt == vml::textures::Texture::TEXTURE_MIRRORED_S)        RepeatTString = "TEXTURE_MIRRORED_S";
						if (repeatt == vml::textures::Texture::TEXTURE_CLAMP_TO_EDGE_S)	  RepeatTString = "TEXTURE_CLAMP_TO_EDGE_S";
						if (repeatt == vml::textures::Texture::TEXTURE_CLAMP_TO_BORDER_S) RepeatTString = "TEXTURE_CLAMP_TO_BORDER_S";
						int releasedata = texture->IsDataPreserved();
						if (releasedata == vml::textures::Texture::TEXTURE_RELEASE_DATA_TRUE) ReleaseDataString = "YES";
						else ReleaseDataString = "NO";

						std::string fullpathfilename = texture->GetResourceFileName();
						std::string filename = vml::strings::SplitPath::GetName(texture->GetResourceFileName());
						//	std::string mainpath = vml::strings::SplitPath::GetDirectory(fullpathfilename);

						ImGui::TableNextColumn();
						ImGui::Text("Bitmap");
						ImGui::TableNextColumn();
						ImGui::Image((void*)(intptr_t)texture->GetID(), { 128,128 });

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

						// close table

						ImGui::EndTable();

					}

				}

			public:

				// -------------------------------------------------------------------------------
				/*
				void CreateDataBase(const std::vector<vml::objects::Object3d_2*>* objects)
				{
					Cache.clear();

					for (auto jt = objects->begin(); jt != objects->end(); ++jt)
					{
						vml::objects::Object3d_2* obj = (*jt);

						const std::vector<vml::models::Model3d_2*>* models = obj->GetModelArray();

						for (auto it = models->begin(); it != models->end(); ++it)
						{
							vml::models::Model3d_2* model = (*it);

							AddToDb(model->GetDiffuseTexture(), model->GetScreenName());
						}

					}

					// dump db

					for (auto it = Cache.begin(); it != Cache.end(); ++it)
					{

						//			std::cout << (*it).first->GetResourceFileName().c_str() << std::endl;

						unsigned int count = 0;

						for (auto jt = (*it).second.begin(); jt != (*it).second.end(); ++jt)
						{
							//			std::cout << "		( " << count << " ) " << (*jt) << std::endl;

							count++;
						}

					}
					
				}
				*/
				// -------------------------------------------------------------------------

				void ShowExampleTree()
				{
					unsigned int tableids = 0;

					for (auto it = Cache.begin(); it != Cache.end(); ++it)
					{
						vml::textures::Texture* tex = (*it).first;

						if (ImGui::TreeNode(tex->GetResourceFileName().c_str()))
						{
							std::string tableid;
							tableid = std::format("table_{0}", tableids);

							DrawTextureInfo(tex, tableid);

							tableids++;
							tableid = std::format("table_{0}", tableids);

							//

							unsigned int flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders;

							if (ImGui::BeginTable(tableid.c_str(), 2, flags))
							{

								ImGui::TableSetupColumn("Model(s) using this Texture");
								ImGui::TableSetupColumn("Model ScreenName");
								ImGui::TableHeadersRow();

								for (auto jt = (*it).second.begin(); jt != (*it).second.end(); ++jt)
								{
									ImGui::TableNextColumn();
									ImGui::Text("Model ScreenName");
									ImGui::TableNextColumn();
									ImGui::Text((*jt).c_str());
								}

								// close table

								ImGui::EndTable();

							}

							ImGui::TreePop();

							tableids++;

						}

					}

				}

				// -------------------------------------------------------------------------
				// ctor / dtor

				TextureDataBase()
				{}

				~TextureDataBase()
				{}

		};
	}
}
