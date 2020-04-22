// RendererTest.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <Core/Components/Camera.h>

#include <Editor.h>
#include <Core/Profiler/Profiler.h>
#include <Window.h>
#include <Vec3.h>
#include <Core/DataStructure/GameObject3D.h>
#include <Core/UserEntry/InputManager.h>

#define SPEED (0.1f)

QXuint indexPackD;
QXuint indexPackE;
QXuint indexPackF;
QXuint indexPackB;
QXuint indexPackL;
QXuint indexPackR;
QXbool state = false;

// Update platform IO from glfw
void PlatformUpdate(Editor* editor, Quantix::Core::Components::Camera* camera)
{
	// Mouse state
	{
		Math::QXvec2 mousePos = GetMousePos();

		editor->_mouseInput->DeltaMouseX = mousePos.x - editor->_mouseInput->MouseX;
		editor->_mouseInput->DeltaMouseY = mousePos.y - editor->_mouseInput->MouseY;

		editor->_mouseInput->MouseX = mousePos.x;
		editor->_mouseInput->MouseY = mousePos.y;

		camera->ChangeView(editor->_mouseInput->DeltaMouseX, editor->_mouseInput->DeltaMouseY, editor->GetWin().GetWidth(), editor->GetWin().GetHeight(), editor->GetApp()->info.deltaTime);
	}

	// Screen size
	glfwGetWindowSize(editor->GetWin().GetWindow(), (QXint*)&editor->GetApp()->info.width, (QXint*)&editor->GetApp()->info.height);
}

void	CameraUpdate(Editor* editor, Quantix::Core::Components::Camera* camera)
{
	if (editor->_mouseInput->MouseCaptured)
	{
		PlatformUpdate(editor, camera);
		if (GetKey(QX_KEY_W) == Quantix::Core::UserEntry::EKeyState::DOWN)
			camera->SetPos(camera->GetPos() + (camera->GetDir() * SPEED));
		if (GetKey(QX_KEY_S) == Quantix::Core::UserEntry::EKeyState::DOWN)
			camera->SetPos(camera->GetPos() - (camera->GetDir() * SPEED));
		if (GetKey(QX_KEY_A) == Quantix::Core::UserEntry::EKeyState::DOWN)
			camera->SetPos(camera->GetPos() - (camera->GetDir().Cross(camera->GetUp()) * SPEED));
		if (GetKey(QX_KEY_D) == Quantix::Core::UserEntry::EKeyState::DOWN)
			camera->SetPos(camera->GetPos() + (camera->GetDir().Cross(camera->GetUp()) * SPEED));
		camera->UpdateLookAt(camera->GetPos());
	}
}

void InitScene(Editor* editor, std::vector<Quantix::Core::Components::Light>& lights)
{
	Quantix::Core::Components::Light light;
	light.ambient = { 0.5f, 0.5f, 0.5f };
	light.diffuse = { 0.7f, 0.7f, 0.7f };
	light.specular = { 1.0f, 1.0f, 1.0f };
	light.position = { 0.0f, 0.0f, 10.f };
	light.direction = { 0.0f, 0.0f, -1.f };
	light.constant = 0.5f;
	light.linear = 0.09f;
	light.quadratic = 0.032f;
	light.cutOff = cos(0.70f);
	light.outerCutOff = cos(0.76f);
	light.type = Quantix::Core::Components::ELightType::SPOT;

	Quantix::Core::Components::Light light2;
	light2.ambient = { 0.5f, 0.5f, 0.5f };
	light2.diffuse = { 0.7f, 0.7f, 0.7f };
	light2.specular = { 1.0f, 1.0f, 1.0f };
	light2.position = { 0.0f, 12.f, 5.f };
	light2.direction = { 0.0f, 0.0f, -1.f };
	light2.constant = 0.5f;
	light2.linear = 0.09f;
	light2.quadratic = 0.032f;
	light2.cutOff = cos(0.70f);
	light2.outerCutOff = cos(0.76f);
	light2.type = Quantix::Core::Components::ELightType::POINT;

	lights.push_back(light);
	lights.push_back(light2);
}

void InitPack()
{
	/*indexPackD = AddButton(Quantix::Core::UserEntry::EKey::QX_KEY_F1, Quantix::Core::UserEntry::ETriggerType::PRESSED);
	indexPackE = AddButton(Quantix::Core::UserEntry::EKey::QX_KEY_ESCAPE, Quantix::Core::UserEntry::ETriggerType::PRESSED);
	indexPackF = AddButton(Quantix::Core::UserEntry::EKey::QX_KEY_W, Quantix::Core::UserEntry::ETriggerType::DOWN);
	indexPackB = AddButton(Quantix::Core::UserEntry::EKey::QX_KEY_S, Quantix::Core::UserEntry::ETriggerType::DOWN);
	indexPackL = AddButton(Quantix::Core::UserEntry::EKey::QX_KEY_A, Quantix::Core::UserEntry::ETriggerType::DOWN);
	indexPackR = AddButton(Quantix::Core::UserEntry::EKey::QX_KEY_D, Quantix::Core::UserEntry::ETriggerType::DOWN);*/
}

void	DebugMode()
{
	if (GetKey(QX_KEY_F1) == Quantix::Core::UserEntry::EKeyState::PRESSED)
	{
		if (!state)
			ACTIVATE_PROFILING(!GETSTATE_PROFILING());
		state = true;
	}
	if (GetKey(QX_KEY_F1) != Quantix::Core::UserEntry::EKeyState::RELEASED)
		state = false;
}

void Init(Editor* editor, std::vector<Quantix::Core::Components::Light>& lights)
{
	glfwSetWindowUserPointer(editor->GetWin().GetWindow(), editor->_mouseInput);

	//Init Pack Input Manager
	InitPack();

	//Init Scene
	InitScene(editor, lights);

	//Init Editor
	editor->Init();
}

void Update(Editor* editor, std::vector<Quantix::Core::Components::Light>& lights, Quantix::Core::Components::Camera* cam)
{
	if (GetKey(QX_KEY_ESCAPE) == Quantix::Core::UserEntry::EKeyState::PRESSED)
	{
		if (editor->_mouseInput->MouseCaptured)
		{
			glfwSetInputMode(editor->GetWin().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			editor->_mouseInput->MouseCaptured = false;
		}
	}

	Quantix::Core::UserEntry::InputManager::GetInstance()->Update(editor->GetWin().GetWindow());

	Quantix::Core::UserEntry::EKeyState state = GetKey(QX_MOUSE_BUTTON_LEFT);

	DebugMode();

	std::vector<Quantix::Core::Components::Mesh*>	meshes;

	START_PROFILING("Draw");
	if (!editor->GetPause() || editor->GetPlay())
	editor->GetApp()->Update(meshes);
	//Editor Update
	if (editor->GetPlay())
		editor->Update(editor->GetApp()->renderer.Draw(meshes, lights, editor->GetApp()->info, editor->GetMainCamera()));
	else
		editor->Update(editor->GetApp()->renderer.Draw(meshes, lights, editor->GetApp()->info, editor->GetEditorCamera()));
	STOP_PROFILING("Draw");
	START_PROFILING("Refresh");
	editor->GetWin().Refresh(editor->GetApp()->info);
	STOP_PROFILING("Refresh");
	if (editor->GetPlay())
		CameraUpdate(editor, editor->GetMainCamera());
	else
		CameraUpdate(editor, editor->GetEditorCamera());
}

int main()
{
	try
	{
		Editor*											editor = new Editor(1920, 900);
		//Init Camera
		Quantix::Core::Components::Camera*				cam = new Quantix::Core::Components::Camera({ 0, 7, -10 }, { 0, -1, 1 }, Math::QXvec3::up);
		
		editor->SetMainCamera(cam);

		std::vector<Quantix::Core::Components::Light>	lights;

		Init(editor, lights);

		while (!editor->GetWin().ShouldClose())
			Update(editor, lights, cam);

		delete cam;
	}
	catch (const std::exception& e)
	{
		std::cout << "Crash : " << std::endl;
		std::cout << e.what() << std::endl;
	}

	Quantix::Core::Debugger::Logger::GetInstance()->CloseLogger();
	return 0;
}
