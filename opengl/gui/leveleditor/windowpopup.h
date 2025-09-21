#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WindowPopUp
{

public:

	bool		ShowWPopUp;
	bool		ShowQPopUp;
	std::string PopUpString;
	std::string Title;
	int			WResult;
	int			QResult;

	//---------------------------------------------------------------------------------

	void ShowWarningPopUp()
	{
		if (ShowWPopUp)
		{
			ImGui::OpenPopup(Title.c_str());

			// Always center this window when appearing

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(350.0f, 80.0f));

			if (ImGui::BeginPopupModal(Title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				// center text

				auto windowWidth = ImGui::GetWindowSize().x;
				auto textWidth = ImGui::CalcTextSize(PopUpString.c_str()).x;
				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
				ImGui::Text(PopUpString.c_str());

				// cetner ok button

				ImGuiStyle& style = ImGui::GetStyle();
				float size = ImGui::CalcTextSize("\tOK\t").x + style.FramePadding.x * 2.0f;
				float avail = ImGui::GetContentRegionAvail().x;
				float off = (avail - size) * 0.5f;
				if (off > 0.0f)
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

				if (ImGui::Button("\tOK\t")) { ImGui::CloseCurrentPopup(); ShowWPopUp = 0; PopUpString = "No Text"; WResult = 1; }

				ImGui::SetItemDefaultFocus();
				ImGui::EndPopup();
			}
		}
		else
		{
			WResult = 0;
		}

	}

	//---------------------------------------------------------------------------------

	void ShowQuestionPopUp()
	{
		if (ShowQPopUp)
		{

			ImGui::OpenPopup(Title.c_str());

			// Always center this window when appearing

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(350.0f, 80.0f));

			if (ImGui::BeginPopupModal(Title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				// center text

				auto windowWidth = ImGui::GetWindowSize().x;
				auto textWidth = ImGui::CalcTextSize(PopUpString.c_str()).x;
				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
				ImGui::Text(PopUpString.c_str());

				// cetner ok button

				if (ImGui::Button("OK", ImVec2(113, 0))) { ImGui::CloseCurrentPopup(); ShowQPopUp = 0; PopUpString = "No Text";	QResult = 1; }
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(113, 0))) { ImGui::CloseCurrentPopup(); ShowQPopUp = 0; PopUpString = "No Text";	QResult = 2; }

				ImGui::SetItemDefaultFocus();
				ImGui::EndPopup();

			}
		}
		else
		{
			QResult = 0;
		}
	}

	// -------------------------------------------------------------------
	// ctor / dtor

	WindowPopUp()
	{
		ShowWPopUp = false;
		ShowQPopUp = false;
		PopUpString = "No Text";
		WResult = 0;
		QResult = 0;
		Title = ICON_FA_TRIANGLE_EXCLAMATION;
		Title += " Warning";
	}

	~WindowPopUp()
	{
	}

};
