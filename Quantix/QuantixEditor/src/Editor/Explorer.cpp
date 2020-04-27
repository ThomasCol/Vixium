#include <imgui.h>
#include <imgui_internal.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Explorer.h"
#include "Profiler.h"

static void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

Explorer::Explorer() :
	_folder{},
	_path{"./"}
{
}

int Explorer::InitFormatFolder()
{
	static QXint folderSize = _folder.GetSizeFolder();
	ImGui::SliderInt("Size", &folderSize, 1, 100);

	_folder.SetSizeFolder(folderSize);

	ImVec2 size = _folder.GetSizeFile();

	size.x = size.y = 70.f + folderSize * 1.3f;

	_folder.SetSizeFile(size);

	QXint lines = 8;
	QXint columns{ (QXint)(ImGui::GetContentRegionAvail().x / size.x) };

	if (columns < 1)
		columns = 1;

	ImGui::Columns(columns, NULL, false);
	return lines * columns;
}


void Explorer::CheckRootFolder(QXint& index)
{
	if (_path != "./" && index == 0)
	{
		QXstring name = "..";
		PushId(_folder.GetIDFolder(), name, index);
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			ModifyFolder(name);
		index++;
		ImGui::NextColumn();
	}
}

void	Explorer::ModifyFolder(QXstring name)
{
	if (name != "")
	{
		if (name == "..")
		{
			_path = fs::path(_path).parent_path().string();
			if (_path.size() == 1 || _path == "./.")
				_path = "./";
		}
		else
		{
			if (_path == "./")
				_path = _path + name;
			else
				_path = _path + "/" + name;
		}
	}
}

void	Explorer::OpenSoftware(QXstring name)
{
	QXstring filePath;
	if (_path == "./")
		filePath = _path + name;
	else
		filePath = _path + "/" + name;
	std::size_t found = filePath.find("./");
	if (found != QXstring::npos)
		filePath.erase(found, 2);
	findAndReplaceAll(filePath, "/", "\\");
	filePath = (fs::current_path() / filePath).string();

	QXstring cmd = "openwith " + filePath;
	std::cout << cmd << std::endl;
	system(cmd.c_str());
}

void Explorer::PushId(QXuint img, QXstring name, QXint& index)
{
	ImGui::PushID(index);
	ImGui::BeginGroup();
	ImGui::ImageButton((ImTextureID)(size_t)img, _folder.GetSizeFile(), ImVec2(0, 1), ImVec2(1, 0), -1, ImVec4(0, 0, 0, 0));
	ImGui::TextWrapped(name.c_str());
	ImGui::EndGroup(); 
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		QXstring filePath = _path + "/" + name;
		ImGui::Text(name.c_str());
		ImGui::SetDragDropPayload("path", filePath.c_str(), filePath.size() + 1, ImGuiCond_Once);
		ImGui::EndDragDropSource();
	}
	PopUpMenuItem(name);
	PopUpMenu();
	ImGui::PopID();
}

void Explorer::DrawFolder(QXint& index)
{
	for (int i = 0; i < _folder.GetFolder().size(); i++)
	{
		QXstring name = _folder.GetFolder()[i];
		PushId(_folder.GetIDFolder(), name, index);
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			ModifyFolder(name);
		index++;
		ImGui::NextColumn();
	}
}

void Explorer::DrawFile(QXint& index)
{
	for (QXint i = 0; i < _folder.GetFile().size(); i++)
	{
		QXstring name = _folder.GetFile()[i];

		if (name.find(".cpp") != std::string::npos)
			PushId(_folder.GetIDCPP(), name, index);
		else if (name.find(".h") != std::string::npos)
			PushId(_folder.GetIDH(), name, index);
		else if (name.find(".sln") != std::string::npos)
			PushId(_folder.GetIDSLN(), name, index);
		else if (name.find(".vcxproj") != std::string::npos)
			PushId(_folder.GetIDVCXPROJ(), name, index);
		else
			PushId(_folder.GetIDFile(), name, index);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			OpenSoftware(name);
		index++;
		ImGui::NextColumn();
	}
}

void Explorer::DrawImg(QXint& index)
{
	for (int i = 0; i < _folder.GetImg().size(); i++)
	{
		QXstring name = _folder.GetImg()[i];
		PushId(_folder.GetIDImg(), name, index);
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			OpenSoftware(name);
		index++;
		ImGui::NextColumn();
	}
}

void Explorer::DrawRepository(QXint& index)
{
	DrawFolder(index);
	DrawFile(index);
	DrawImg(index);
}


void Explorer::MenuRename(QXstring file)
{
	QXstring name;
	QXchar currName[64];

	memcpy(currName, file.c_str(), file.size() + 1);
	if (ImGui::BeginMenu("Rename"))
	{
		if (ImGui::InputText("##Input", currName, IM_ARRAYSIZE(currName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::filesystem::rename(std::filesystem::current_path().string() + "\\" + file, std::filesystem::current_path().string() + "\\" + currName);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndMenu();
	}
}

void Explorer::MenuItem(QXbool* selection, std::vector<QXstring> itemMenu, QXstring file)
{
	for (QXuint i{ 0 }; i < itemMenu.size(); i++)
	{
		if (selection[i])
		{
			if (itemMenu[i] == "Open")
			{
				if (!fs::is_directory(std::filesystem::current_path().string() + "\\" + file))
					OpenSoftware(file);
				else
					ModifyFolder(file);
				selection[i] = false;
			}
			else if (itemMenu[i] == "Delete")
			{
				std::filesystem::remove_all(std::filesystem::current_path().string() + "\\" + file);
				selection[i] = false;
			}
		}
	}
}

void Explorer::PopUpMenuItem(QXstring& itemFile)
{
	if (ImGui::BeginPopupContextItem("Context Item"))
	{
		static QXbool selection[2] = { false, false };
		ImGui::Selectable("Open", &selection[0]);
		MenuRename(itemFile);
		ImGui::Selectable("Delete", &selection[1]);
		MenuItem(selection, { "Open", "Delete" }, itemFile);

		ImGui::EndPopup();
	}
}

void Explorer::CreateItemFolder(QXstring item)
{
	QXchar currName[64];

	memset(currName, 0, 64);
	if (ImGui::BeginMenu(item.c_str()))
	{
		if (ImGui::InputText("##Input", currName, IM_ARRAYSIZE(currName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::filesystem::create_directory(std::filesystem::current_path().string() + "\\" + currName);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndMenu();
	}
}

void Explorer::CreateItemFile(QXstring item)
{
	QXchar currName[64];

	memset(currName, 0, 64);
	if (ImGui::BeginMenu(item.c_str()))
	{
		if (ImGui::InputText("##Input", currName, IM_ARRAYSIZE(currName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string cmd = "echo off > " + fs::current_path().string() + "\\" + currName;
			std::cout << "Create File: " << cmd << std::endl;
			system(cmd.c_str());
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndMenu();
	}
}

void Explorer::CreateItem(std::vector<QXstring> itemMenu)
{
	CreateItemFolder(itemMenu[0]);
	CreateItemFile(itemMenu[1]);
}

void Explorer::PopUpMenu()
{
	if (ImGui::BeginPopupContextWindow("Context Menu", 1, false))
	{
		if (ImGui::BeginMenu("Create"))
		{
			CreateItem({ "Create Folder", "Create File" });

			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}

void Explorer::Update(Quantix::Core::DataStructure::ResourcesManager& cache, QXstring name, ImGuiWindowFlags flags)
{
	ImGui::Begin(name.c_str(), NULL, flags);
	{
		_folder = Folder(cache, _path);

		QXint rowCol = InitFormatFolder();
		QXint i = 0;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		CheckRootFolder(i);
		DrawRepository(i);
		ImGui::PopStyleColor();
	}
	ImGui::End();
}
