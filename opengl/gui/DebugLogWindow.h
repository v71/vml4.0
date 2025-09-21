#pragma once


namespace vml
{
	namespace gui
	{

		class DebugLogWindow
		{
			private:

				// combo data

				std::string	ComboItemString_2[1024];
				bool		ComboButton_2[1024];
				int			ComboStringCount_2;
				size_t		CurrentItemId_2;
				std::string ComboId_2;
				std::string Name_2;

				// ---------------------------------------------------

				bool							   AutoScroll;
				std::deque<vml::utils::LogMessage> AllMessages;
				std::deque<vml::utils::LogMessage> FilteredMessages;
				std::deque<vml::utils::LogMessage> InfoMessages;
				std::deque<vml::utils::LogMessage> ErrorMessages;
				std::deque<vml::utils::LogMessage> WarningMessages;
				ImVec4							   InfoColor;
				ImVec4							   DebugColor;
				ImVec4							   ErrorColor;
				ImVec4							   WarningColor;

				// ---------------------------------------------------
				// filter logs according to input string

				void Filter(const std::string &typefilter)
				{
					
					std::deque<vml::utils::LogMessage> rg;
					std::deque<vml::utils::LogMessage>::const_iterator it;

					// copy all messages from source queue

					rg = vml::utils::Logger::GetInstance()->GetHistory();

					it = rg.begin();

					AllMessages.clear();

					while (it != rg.end())
					{
						AllMessages.emplace_back(*it);
						++it;
					}

					// copy info, warning and errors messages
					// in the corresponding queues

					InfoMessages.clear();
					ErrorMessages.clear();
					WarningMessages.clear();

					it = rg.begin();

					while (it != rg.end())
					{
						vml::utils::LogMessage lvl = (*it);

						if (lvl.GetLevel() == vml::utils::LogMessage::LogLevel::LEVEL_ERROR)
							ErrorMessages.emplace_back(*it);
						
						if (lvl.GetLevel() == vml::utils::LogMessage::LogLevel::LEVEL_INFO)
							InfoMessages.emplace_back(*it);

						if (lvl.GetLevel() == vml::utils::LogMessage::LogLevel::LEVEL_WARNING)
							WarningMessages.emplace_back(*it);

						++it;

					}

					if (typefilter == "ALL")
						FilteredMessages = AllMessages;
					else if (typefilter == "INFO")
						FilteredMessages = InfoMessages;
					else if (typefilter == "ERROR")
						FilteredMessages = ErrorMessages;
					else if (typefilter == "WARNING")
						FilteredMessages = WarningMessages;

				}

				// ------------------------------------------------------------------
				// check if an array contains just one element of another array

				bool Contains(std::vector<std::string>& tokens, std::vector<std::string>& words)
				{
					for (size_t i = 0; i < words.size(); ++i)
					{
						std::string word = words[i];

						for (size_t j = 0; j < tokens.size(); ++j)
						{
							if (word == tokens[j])
								return true;
						}
					}
					return false;
				}

				// ---------------------------------------------------------------------

				void DrawWithTimeStamp()
				{
					
					// Using those as a base value to create width/height that are factor of the size of our font

					ImGuiTableFlags mflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
											 ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
											 ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

					float inner_width_with_scroll = 0.0f;
					float inner_width_to_use = (mflags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
					float row_min_height = 0;

					if (ImGui::BeginTable("table_0", 4, mflags, ImVec2(0, 0), inner_width_to_use))
					{
						// Declare columns

						ImGui::TableSetupColumn("TimeStamp", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 0);
						ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 1);
						ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 2);
						ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthStretch, 0, 3);
						ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible

						//	ImGui::TableHeadersRow();

						// Instead of calling TableHeadersRow() we'll submit custom headers ourselves

						ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

						for (int column = 0; column < 4; column++)
						{
							ImGui::TableSetColumnIndex(column);
							const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
							float column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(column_name).x) * 0.5f;
							if (column1_x > ImGui::GetCursorPosX())
								ImGui::SetCursorPosX(column1_x);
							ImGui::TableHeader(column_name);
						}

						// Demonstrate using clipper for large vertical lists

						ImGuiListClipper clipper;
						static int item_current_idx = -1;
						float column1_x = 0.0f;
						ImVec4 color = ImColor{ IM_COL32(0,255,0,255) };

						// begin list clipping

						clipper.Begin((int)FilteredMessages.size());

						while (clipper.Step())
						{

							for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
							{

								vml::utils::LogMessage lvl = FilteredMessages[i];
								std::string messagetype;
								if (lvl.GetLevel() == vml::utils::LogMessage::LogLevel::LEVEL_ERROR) { color = ErrorColor; messagetype="ERROR"; }
								if (lvl.GetLevel() == vml::utils::LogMessage::LogLevel::LEVEL_INFO) { color = InfoColor; messagetype = "INFO";	}
								if (lvl.GetLevel() == vml::utils::LogMessage::LogLevel::LEVEL_WARNING) { color = WarningColor; messagetype = "WARNING"; }

								// push id

								ImGui::PushID(i);

								ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

								// timestamp 

								ImGui::TableSetColumnIndex(0);
								column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(FilteredMessages[i].GetDateString().c_str()).x) * 0.5f;
								if (column1_x > ImGui::GetCursorPosX())
									ImGui::SetCursorPosX(column1_x);
								ImGui::PushStyleColor(ImGuiCol_Text, color);
								if (ImGui::Selectable(FilteredMessages[i].GetDateString().c_str(), (item_current_idx == i), ImGuiSelectableFlags_SpanAllColumns))
									item_current_idx = (int)i;
								ImGui::PopStyleColor();

								// class

								ImGui::TableSetColumnIndex(1);
								column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(messagetype.c_str()).x) * 0.5f;
								if (column1_x > ImGui::GetCursorPosX())
									ImGui::SetCursorPosX(column1_x);
								ImGui::PushStyleColor(ImGuiCol_Text, color);
								ImGui::TextUnformatted(messagetype.c_str());
								ImGui::PopStyleColor();

								// message

								ImGui::TableSetColumnIndex(2);
								column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(0) - ImGui::CalcTextSize(FilteredMessages[i].GetText().c_str()).x) * 0.5f;
								if (column1_x > ImGui::GetCursorPosX())
									ImGui::SetCursorPosX(column1_x);
								ImGui::PushStyleColor(ImGuiCol_Text, color);
								ImGui::TextUnformatted(FilteredMessages[i].GetText().c_str());
								ImGui::PopStyleColor();

								// text line

								ImGui::TableSetColumnIndex(3);
								column1_x = ImGui::GetCursorPosX() + (ImGui::GetColumnWidth(1) - ImGui::CalcTextSize(FilteredMessages[i].GetFileNamePos().c_str()).x) * 0.5f;
								if (column1_x > ImGui::GetCursorPosX())
									ImGui::SetCursorPosX(column1_x);
								ImGui::PushStyleColor(ImGuiCol_Text, color);
								ImGui::TextUnformatted(FilteredMessages[i].GetFileNamePos().c_str());
								ImGui::PopStyleColor();

								// pop id

								ImGui::PopID();

							}
						}

						// if autoscroll is set then 
						// scroll to the last element

						if (AutoScroll)
							ImGui::SetScrollHereY(1.0f);

						// close clipper 

						clipper.End();

						// close table

						ImGui::EndTable();

					}
					
				}

				// ---------------------------------------------------------------------------------
				// draw second combo

				void DrawCombo2()
				{
					ImGuiStyle* style = &ImGui::GetStyle();

					ImGui::Text(Name_2.c_str());
					ImGui::SameLine();

					static const char* current_item_2 = ComboItemString_2[0].c_str();
					const float width = ImGui::GetWindowWidth();
					const float combo_width = width / 16.0f;
					ImGui::SetNextItemWidth(combo_width);

					if (ImGui::BeginCombo(ComboId_2.c_str(), current_item_2))
					{
						for (size_t i = 0; i < ComboStringCount_2; ++i)
						{
							if (ImGui::MenuItem(ComboItemString_2[i].c_str(), "", &ComboButton_2[i]))
							{
								current_item_2 = ComboItemString_2[i].c_str();
								CurrentItemId_2 = i;
							}
						}

						ImGui::EndCombo();
					}

					// enable flags

					for (size_t i = 0; i < ComboStringCount_2; ++i)
						ComboButton_2[i] = 0;

					// set current item to true

					ComboButton_2[CurrentItemId_2] = 1;

				}

			public:

				// ---------------------------------------------------
				// ctor / dtor

				void Draw()
				{

					// timestamp button

//					ToggleButton();
				
//					ImGui::SameLine();

					// clear all button

					{
					//	if (ImGui::Button(ICON_FA_CIRCLE_XMARK))
					//		vml::utils::Logger::GetInstance()->ClearRingBuffer();
					//	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
					//	{	std::string extext = "Clear all";
					//		ImGui::SetTooltip(extext.c_str());	}
					//	ImGui::SameLine();
					}
					
					// total lines

					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255,255,255,255));
						ImGui::Text(ICON_FA_BARS);
						const size_t lines = vml::utils::Logger::GetInstance()->GetLinesCount();
						std::string text = std::to_string((size_t)lines);
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
						{	std::string extext = text + " " + "Total Messages";
							ImGui::SetTooltip(extext.c_str());	}
						ImGui::SameLine();
						ImGui::Text(text.c_str());
						ImGui::PopStyleColor();
						ImGui::SameLine();
					}
					
					// info lines

					{
						ImGui::PushStyleColor(ImGuiCol_Text, InfoColor);
						ImGui::Text(ICON_FA_CIRCLE_INFO);
						const size_t lines = InfoMessages.size();
						std::string text = std::to_string((int)lines);
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
						{	std::string extext = text + " " + "Info Messages";
							ImGui::SetTooltip(extext.c_str());	}
						ImGui::SameLine();
						ImGui::Text(text.c_str());
						ImGui::PopStyleColor();
						ImGui::SameLine();
					}
					
					// wartning lines
					
					{
						ImGui::PushStyleColor(ImGuiCol_Text, WarningColor);
						ImGui::Text(ICON_FA_TRIANGLE_EXCLAMATION);
						const size_t lines = WarningMessages.size();
						std::string text = std::to_string((int)lines);
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
						{	std::string extext = text + " " + "Warning Messages";
							ImGui::SetTooltip(extext.c_str());	}
						ImGui::SameLine();
						ImGui::Text(text.c_str());
						ImGui::PopStyleColor();
						ImGui::SameLine();
					}
					
					// error lines

					{
						ImGui::PushStyleColor(ImGuiCol_Text, ErrorColor);
						ImGui::Text(ICON_FA_EXCLAMATION);
						const size_t lines = ErrorMessages.size();
						std::string text = std::to_string((int)lines);
						if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
							std::string extext = text + " " + "Error Messages";
						ImGui::SetTooltip(extext.c_str());	}
						ImGui::SameLine();
						ImGui::Text(text.c_str());
						ImGui::PopStyleColor();
						ImGui::SameLine();
					}
	
					// separator

					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(155.0f / 255.0f, 155.0f / 255.0f, 155.0f / 255.0f, 1.0f));
						ImGui::Text("|");
						ImGui::PopStyleColor();
						ImGui::SameLine();
					}

					// draw combos
					
					DrawCombo2();

					// filter messages

					Filter(ComboItemString_2[CurrentItemId_2]);
					
					// draw messages according to timespatmp flag

					DrawWithTimeStamp();
					
				}

				// ---------------------------------------------------
				// ctor / dtor

				DebugLogWindow()
				{
					AutoScroll   = false;
					InfoColor    = ImColor{ IM_COL32(83,189,235,255) };
					WarningColor = ImColor{ IM_COL32(255,255,0,255) };
					ErrorColor   = ImColor{ IM_COL32(255,0,0,255) };

					// fill combo data

					ComboStringCount_2						= 0;
					CurrentItemId_2							= 0;
					Name_2									= "Show output from:";
					ComboId_2								= "##combo2";
					ComboItemString_2[ComboStringCount_2++] = "ALL";
					ComboItemString_2[ComboStringCount_2++] = "INFO";
					ComboItemString_2[ComboStringCount_2++] = "ERROR";
					ComboItemString_2[ComboStringCount_2++] = "WARNING";
					for (size_t i = 0; i < ComboStringCount_2; ++i)
						ComboButton_2[i] = 0;
					ComboButton_2[0] = 1;

				}

				~DebugLogWindow()
				{}

		};
	}
}
