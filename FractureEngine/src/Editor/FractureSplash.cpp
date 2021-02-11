#include "FractureSplash.h"
#include <sys/stat.h>
#include <imgui\imgui_internal.h>



bool Fracture::FractureSplash::p_open;

Fracture::FractureSplash::FractureSplash(Editor* editor) :m_editor(editor)
{
    FRACTURE_INFO("Initializing Splash");
	m_isShow = true;
    m_run = false;
    m_logger = m_editor->GetLogger();
	m_window = std::unique_ptr<GameWindow>(new GameWindow(800,400,"Splash"));
    m_AssetManger = std::unique_ptr<AssetManager>();  

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImFont* pFont = io.Fonts->AddFontFromFileTTF("content/fonts/Roboto-Regular.TTF", 20.0f);

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	Style();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window->Context(),true);
	ImGui_ImplOpenGL3_Init("#version 400");

    m_AssetManger->AddTexture("splash","content/textures/splashtest.png",TextureType::Diffuse);
    m_AssetManger->AddTexture("title", "content/textures/title.png", TextureType::Diffuse);
    FRACTURE_INFO("Completed Init");
}

Fracture::FractureSplash::~FractureSplash()
{
}

bool Fracture::FractureSplash::Show()
{
    FRACTURE_INFO("Show Splash");

    m_title = m_AssetManger->getTexture("title");
    m_splash = m_AssetManger->getTexture("splash");

	while (m_isShow)
	{
        onUpdate();
		onBeginFrame();
		onRender();
		onEndFrame();
	}
    Close();    
    return m_run;
}

void Fracture::FractureSplash::Close()
{   
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(); 
  m_window->close();
  m_window.release();
  m_AssetManger.release();
}

void Fracture::FractureSplash::onUpdate()
{
    m_window->pollEvents();
}

void Fracture::FractureSplash::onBeginFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Fracture::FractureSplash::onRender()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    dockspace_flags |= ImGuiDockNodeFlags_NoTabBar;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
   
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    ImGui::Begin("DockSpace Demo", &p_open, window_flags);

  
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::Begin("Splash",0,ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);

    //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); 
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::Image((void*)m_splash->id, ImVec2(300, 400));
    ImGui::PopStyleVar();
    ImGui::End();

    ImGui::Begin("Project", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);


    ImGui::Image((void*)m_title->id, ImVec2(500, 200));
    ImGui::PushID("##project");
    ImGui::Columns(3);
    ImGui::AlignTextToFramePadding();
    ImGui::SetColumnWidth(0, 160);

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
    ImGui::PushFont(boldFont);

    if (ImGui::Button("New Project...", ImVec2(160, 200)))
    {       
        std::string filepath = FileDialogue::SelectDirectory();
        if (!filepath.empty())
        {
            createNewProject(filepath);
            m_editor->isNewProject(true);
            m_isShow = false;
            m_run = true;
        }    
    }
    ImGui::SameLine();
    ImGui::NextColumn();

    if (ImGui::Button("Open Project...", ImVec2(160, 200)))
    {
       std::string filepath =  FileDialogue::OpenFile("Fracture Project (*.fracture)\0*.fracture\0)");
       if (!filepath.empty())
       {
           //ProjectSerializer serializer();
           openProject(filepath);
           m_isShow = false;
           m_run = true;
       }
        else
        {
            FRACTURE_CRITICAL("Failed to Load Project");
            m_isShow = true;
            m_run = false;      
        }
    }
    ImGui::SameLine();

    ImGui::NextColumn();
    if (ImGui::Button("Exit", ImVec2(160, 200)))
    {
        m_isShow = false;
        m_run = false;
    }
    ImGui::NextColumn();
    ImGui::Columns(1);

    ImGui::PopFont();
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::PopID();
    ImGui::End();

    ImGui::End();
}

void Fracture::FractureSplash::onEndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{		
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();		
	}
    m_window->swapBuffers();
}

void Fracture::FractureSplash::createNewProject(const std::string& filepath)
{  
    m_editor->Properties()->ProjectDirectory = filepath;
    m_editor->Properties()->ProjectName ="Untitled";
    m_editor->Properties()->ProjectFilePath = filepath +"\\"+m_editor->Properties()->ProjectName+".fracture";
    m_editor->Properties()->ContentDirectory = filepath + "\\content";
    m_editor->Properties()->GameConfigPath = filepath;      
    m_editor->Properties()->ScenesPath = filepath + "\\content\\scenes";
    m_editor->Properties()->ModelsPath = filepath + "\\content\\models";
    m_editor->Properties()->TexturesPath = filepath + "\\content\\textures";
    m_editor->Properties()->ShadersPath = filepath + "\\content\\shaders";
    m_editor->Properties()->FontsPath = filepath + "\\content\\fonts";


    int content = mkdir(m_editor->Properties()->ContentDirectory.c_str());
    if (content)
    {

    }
    int models = mkdir(m_editor->Properties()->ModelsPath.c_str());
    int scenes = mkdir(m_editor->Properties()->ScenesPath.c_str());
    int shaders = mkdir(m_editor->Properties()->ShadersPath.c_str());
    int textures = mkdir(m_editor->Properties()->TexturesPath.c_str());
    int fonts = mkdir(m_editor->Properties()->FontsPath.c_str());

}

void Fracture::FractureSplash::openProject(const std::string& filepath)
{
    m_editor->Properties()->ProjectFilePath = filepath;    
}

inline void Fracture::FractureSplash::Style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int is3D = 0;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
    colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
    colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = is3D;

    style.WindowRounding = 2;
    style.ChildRounding = 2;
    style.FrameRounding = 2;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 2;

#ifdef IMGUI_HAS_DOCK 
    style.TabBorderSize = (float)is3D;
    style.TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.22f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif
}
