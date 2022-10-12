#include "gui.hpp"

GUI::GUI(std::string glsl_version, GLFWwindow *window, Scene *scene)
{
	this -> window = window;
	this -> scene = scene;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void GUI::NewFrameSetUp()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUI::Render(int width, int height)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::OneFrame(std::deque<std::string> v_names)
{
	bool active = true;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &active, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id, dock_id_right, dock_id_right_bottom, dock_id_bottom;
	if(this -> first)
	{
		dockspace_id = ImGui::GetID("Dockspace Demo");
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);


		ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
		dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, NULL, &dock_main_id);
		dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, NULL, &dock_main_id);
		dock_id_right_bottom = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.65f, NULL, &dock_id_right);

		ImGui::DockBuilderDockWindow("World", dock_id_right);
		ImGui::DockBuilderDockWindow("Model", dock_id_right_bottom);
		ImGui::DockBuilderDockWindow("Log", dock_id_bottom);

		ImGui::DockBuilderFinish(dockspace_id);
		ImGui::DockBuilderFinish(dock_id_right);
	}
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::DockSpace(dock_id_right, ImVec2(0.0f, 0.0f), dockspace_flags);
	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("Quit")) {glfwSetWindowShouldClose(this -> window, true);}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Settings"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::End();

	{
		ImGui::Begin("World", &active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		const int namesSize = v_names.size();
// 		std::cout << namesSize << std::endl;
		static std::string world_action = " ";
		static std::string currentItem;

		const char* worldItems[namesSize];
		float xwindowsize = 512, ywindowsize = 360;
		if(world_action == "open 3d model")
		{
			ImGui::OpenPopup("Add 3D Model");
		}
		else if(world_action == "open 3d shape")
		{
			ImGui::OpenPopup("Add 3D Shape");
		}
		for(int i = 0; i < namesSize; i++)
		{
			worldItems[i] = v_names[i].c_str();
		}
			ImGui::SetNextWindowSize(ImVec2(xwindowsize, ywindowsize));
			ImGui::SetNextWindowPos(ImVec2((viewport -> Size.x - xwindowsize)/2, (viewport -> Size.y - ywindowsize)/2));
			bool open = true;
			if(ImGui::BeginPopupModal("Add 3D Shape", &open))
			{
				static std::string currentShapeType;
				std::string shapeTypes[] = {"Cube", "Cylinder", "Sphere", "Tripyramid", "Pyramid", "Triangular Prism"};
				ImGui::Text("Select Component:");
				if(ImGui::BeginCombo("##Select Component", currentShapeType.c_str()))
				{
					for(int i = 0; i < IM_ARRAYSIZE(shapeTypes); i++)
					{
						bool isSelected = (currentShapeType == shapeTypes[i]);
						if(ImGui::Selectable(shapeTypes[i].c_str(), &isSelected))
						{
							currentShapeType = shapeTypes[i];
						}
						if(isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				static float size = 0.5f;
				ImGui::Text("Input Size: ");
				ImGui::InputFloat("##input size", &size, 0.0f, 100.0f);
				static float x = 0.0f, y = 0.0f, z = 0.0f, mass = 1.0f;
				ImGui::Text("Set Position:");
				ImGui::InputFloat("##x", &x, 0.0f, 10.0f);
				ImGui::InputFloat("##y", &y, 0.0f, 10.0f);
				ImGui::InputFloat("##z", &z, 0.0f, 10.0f);
				ImGui::Text("Set Mass:");
				ImGui::InputFloat("##mass", &mass, 0.0f, 10.0f);
				static float my_color[3] = {0.3, 0.3, 0.3};
				ImGui::ColorEdit3("Color", my_color);
				if(ImGui::Button("Load"))
				{
					if(currentShapeType == "Cube")
					{
						AddCube(size, glm::vec3(x, y, z), mass, my_color);
						world_action = " ";
						for(auto &model : this -> scene -> world ->getModels())
							model.second -> isSelected = false;
						currentItem = this -> scene -> world -> getNames().back();
						this -> scene -> world -> getModels().at(currentItem) -> isSelected = true;
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::SameLine();
				if(ImGui::Button("Cancel"))
				{
					world_action = " ";
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::SetNextWindowSize(ImVec2(xwindowsize, ywindowsize));
			ImGui::SetNextWindowPos(ImVec2((viewport -> Size.x - xwindowsize)/2, (viewport -> Size.y - ywindowsize)/2));
			if(ImGui::BeginPopupModal("Add 3D Model", &open))
			{
				static char *filename = "Choose A 3D Model";
				if(filename) ImGui::Text(filename);
				else ImGui::Text("Choose A 3D Model");
				if(ImGui::Button("Choose 3D Model"))
				{
					osdialog_filters *filters = osdialog_filters_parse("Source:stl");
					filename = osdialog_file(OSDIALOG_OPEN, NULL, NULL, filters);
				}
				static float modelx = 0.0f, modely = 0.0f, modelz = 0.0f, modelmass = 1.0f;
				ImGui::Text("Set Position:");
				ImGui::InputFloat("##modelx", &modelx, 0.0f, 10.0f);
				ImGui::InputFloat("##modely", &modely, 0.0f, 10.0f);
				ImGui::InputFloat("##modelz", &modelz, 0.0f, 10.0f);
				ImGui::Text("Set Mass:");
				ImGui::InputFloat("##modelmass", &modelmass, 0.0f, 10.0f);
				static float my_color[3] = {0.3, 0.3, 0.3};
				ImGui::ColorEdit3("Color Of Model", my_color);
				static std::string message;
				if(ImGui::Button("Load"))
				{
					if(filename != "Choose A 3D Model")
					{
						AddComponent(filename, glm::vec3(modelx, modely, modelz), modelmass, my_color);
						world_action = " ";
						for(auto &model : this -> scene -> world -> getModels())
							model.second -> isSelected = false;
						currentItem = this -> scene -> world -> getNames().back();
						this -> scene -> world -> getModels().at(currentItem) -> isSelected = true;
						ImGui::CloseCurrentPopup();
					}
					else if(filename == "Choose A 3D Model")
					{
						message = "You have to choose a 3D model to load!";
					}
				}
				ImGui::SameLine();
				if(ImGui::Button("Cancel"))
				{
					world_action = " ";
					ImGui::CloseCurrentPopup();
				}
				ImGui::Text(message.c_str());
				ImGui::EndPopup();
			}
		if(ImGui::BeginTabBar("World"))
		{
			if(ImGui::BeginTabItem("Components"))
			{
				
				static int worldItemsCurrent = 0;
				ImGui::ListBox("##components", &worldItemsCurrent, worldItems, IM_ARRAYSIZE(worldItems), 9);
				if(ImGui::Button("Add 3D Model"))
				{
					world_action = "open 3d model";
				}
				ImGui::SameLine();
				if(ImGui::Button("Add 3D Shape"))
				{
					world_action = "open 3d shape";
				}
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Settings"))
			{
				if(ImGui::CollapsingHeader("Camera Settings"))
				{
					static float speed = 5.0f;
					ImGui::Text("Speed:");
					ImGui::SliderFloat("##speed", &speed, 0.0f, 1000.0f);
					this -> scene -> changeCameraSpeed(speed);
					static float sensitivity = 0.1f;
					ImGui::Text("Sensitivity");
					ImGui::SliderFloat("##sensitivity", &sensitivity, 0.0f, 0.3f);
					this -> scene -> changeCameraSensitivity(sensitivity);
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		ImGui::Begin("Model", &active, 0);
		ImGui::Text("Select Component:");
		if(ImGui::BeginCombo("##Select Component", currentItem.c_str()))
		{
			for(const auto &s : v_names)
			{
				bool isSelected = (currentItem == s);
				this -> scene -> world -> getModels().at(s) -> isSelected = false;
				if(ImGui::Selectable(s.c_str(), &isSelected))
				{
					currentItem = s;
					this -> scene -> world -> getModels().at(currentItem) -> isSelected = true;
				}
				if(isSelected)
					ImGui::SetItemDefaultFocus();
			}
			this -> currentItemMem = currentItem;
			ImGui::EndCombo();
		}
		if(ImGui::BeginTabBar("Model"))
		{
			if(ImGui::BeginTabItem("Preview"))
			{
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Script"))
			{
				glm::mat4 origModel;
				char code[1024 * 16] = "";

				if(ImGui::Button("Run"))
				{
					origModel = this -> scene -> world -> getModels().at(currentItem) -> GetModel();
// 					this -> scene -> loadedModels.at(currentItem) -> loopCommands = this -> scene -> loadedModels.at(currentItem) -> interpreter .Interpret(this -> scene -> loadedModels.at(currentItem) -> sourceCode);

					this -> scene -> world -> getModels().at(currentItem) -> Run();
// 					this -> scene -> setLoopCommands(this -> m_interpreters.at(currentItem).GetCom());
					if(this -> scene -> world -> getModels().at(currentItem) -> interpreter.hasLoop)
					{
						this -> scene -> world -> getModels().at(currentItem) -> isRunning = true;
// 						this -> scene -> setRunningBool(this -> m_isRunning);
					}

				}
				ImGui::SameLine();
				if(ImGui::Button("Pause"))
				{
					this -> scene -> world -> getModels().at(currentItem) -> isRunning = false;
// 					this -> scene -> setRunningBool(this -> m_isRunning);
				}
				ImGui::SameLine();
				if(ImGui::Button("Resume"))
				{
					this -> scene -> world -> getModels().at(currentItem) -> isRunning = true;
// 					this -> scene -> setRunningBool(this -> m_isRunning);
				}
				ImGui::SameLine();
				if(ImGui::Button("Reset"))
				{
					this -> scene -> world -> getModels().at(currentItem) -> isRunning = false;
// 					this -> scene -> setRunningBool(this -> m_isRunning);
					this -> scene -> world -> getModels().at(currentItem) -> ChangeModelMat(origModel);
				}

				if(currentItem.empty())
					ImGui::InputTextMultiline("##test", code, 1024 * 16, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 38));
				else
					ImGui::InputTextMultiline("##test", this -> scene -> world -> getModels().at(currentItem) -> sourceCode, 1024 * 16, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 38));
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Controls"))
			{

				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Info"))
			{
				if(!currentItem.empty())
				{
					ImGui::Text("Mass: ");
					ImGui::SameLine();
					float mass = this -> scene -> world -> getModels().at(currentItem) -> mass;
					ImGui::Text(std::to_string(mass).c_str());
					ImGui::Text("Current Position:");
					float infox = this -> scene -> world -> getModels().at(currentItem) -> body -> getCenterOfMassPosition().getX();
					float infoy = this -> scene -> world -> getModels().at(currentItem) -> body -> getCenterOfMassPosition().getY();
					float infoz = this -> scene -> world -> getModels().at(currentItem) -> body -> getCenterOfMassPosition().getZ();
					ImGui::Text("X pos:");
					ImGui::SameLine();
					ImGui::Text(std::to_string(infox).c_str());
					ImGui::Text("Y pos:");
					ImGui::SameLine();
					ImGui::Text(std::to_string(infoy).c_str());
					ImGui::Text("Z pos:");
					ImGui::SameLine();
					ImGui::Text(std::to_string(infoz).c_str());
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
		ImGui::Begin("Log", &active, 0);
		if(ImGui::Button("Run Simulation"))
		{
			this -> scene -> isRunning = true;
			for(int j = this -> scene -> world -> getWorld() ->getNumCollisionObjects() - 1; j >= 0; j--)
			{
				if(j >= 1)
				{
					this -> scene -> world -> getModels().at(this -> scene -> world -> getNames().at(j - 1)) -> SetOriginalTransform();
// 					this -> scene -> v_loadedModels.at(j) -> resetBody();
					this -> scene -> world -> getModels().at(this -> scene -> world -> getNames().at(j - 1)) -> body -> clearForces();
				}
			}
			this -> scene -> log.str("");
		}
		ImGui::SameLine();
		if(ImGui::Button("Pause Simulation"))
		{
			this -> scene -> isRunning = false;
		}
		ImGui::SameLine();
		if(ImGui::Button("Reset Simulation"))
		{
			this -> scene -> isRunning = false;
			for(int j = this -> scene -> world -> getWorld() ->getNumCollisionObjects() - 1; j >= 0; j--)
			{
				if(j >= 1)
				{
					this -> scene -> world -> getWorld() -> getCollisionObjectArray()[j] -> setWorldTransform(this -> scene -> world -> getModels().at(this -> scene -> world -> getNames().at(j - 1)) -> GetOriginalTransform());
					this -> scene -> world -> getModels().at(this -> scene -> world -> getNames().at(j - 1)) -> resetBody();
					this -> scene -> world -> getModels().at(this -> scene -> world -> getNames().at(j - 1)) -> SetToOriginalTransform();
				}
			}
		}
		ImGui::BeginChild("Log");
// 		std::cout << this -> scene -> log.str().size() << std::endl;
		ImGui::Text(this -> scene -> log.str().c_str());
		if(this -> scene -> isRunning)
			ImGui::SetScrollHere(1.0f);
		ImGui::EndChild();
		ImGui::End();
	}
}

void GUI::AddComponent(std::string filename, glm::vec3 pos = glm::vec3(0.0f), float mass = 1.0f, float color[3] = 0)
{
	scene -> AddModel(std::string(filename), pos, mass, color);
}

void GUI::AddCube(float size = 0.5f, glm::vec3 pos = glm::vec3(0.0f), float mass = 1.0f, float color[3] = 0)
{
// 	char code[1024 * 16] = "";
// 	commandVector comVec;
// 	this -> m_sourceCode.insert(std::make_pair(this -> currentItemMem, code));
// 	this -> m_isRunning.insert(std::make_pair("Cube", false));
// 	this -> scene -> setRunningBool(this -> m_isRunning);
	scene -> AddCube(size, pos, mass, color);
}
