#include "hello_imgui/hello_imgui.h"

// Struct that holds the application's state
struct AppState
{
    float speed = 0.0f;
    int counter = 0;

    enum class MineState
    {
        Init,
        Mining,
        Done
    };
    float mine_progress = 0.f;
    MineState mineState = MineState::Init;
};

// contains the ImGui widgets on the left panel
void CommandGui(AppState & state)
{
    
    ImGui::Text("This is the mining control panel. Remember to set your speed before starting. \n Move your mouse around to mine faster!");

    ImGui::Separator();

    // edit the float using a slider
    if (ImGui::SliderFloat("Miner speed", &state.speed, 0.0f, 1.0f))
        HelloImGui::Log(HelloImGui::LogLevel::Warning, "state.speed was changed to %f", state.speed);

    // buttons return true when clicked
    if (ImGui::Button("Reset!")) {
        state.counter = 0;
        HelloImGui::Log(HelloImGui::LogLevel::Info, "Reset was pressed");
    }

    ImGui::SameLine();
    ImGui::Text("Coins = %d", state.counter);

    // checks the state enums, does stuff, logs it
    switch(state.mineState)
    {
        case AppState::MineState::Init:
            if (ImGui::Button(ICON_FA_MONEY_BILL " Start"))
            {
                state.mineState = AppState::MineState::Mining;
                HelloImGui::Log(HelloImGui::LogLevel::Warning, "Miner has started!");
            }
            break;

        case AppState::MineState::Mining:
            ImGui::Text(ICON_FA_HOURGLASS_HALF " Mining");
            state.mine_progress += (0.003f * state.speed); // multiplied by speed
            if (state.mine_progress >= 1.f)
            {
                state.mineState = AppState::MineState::Done;
                HelloImGui::Log(HelloImGui::LogLevel::Warning, "Miner is done!");
            }
            break;

        case AppState::MineState::Done:
            ImGui::Text(ICON_FA_THUMBS_UP " Completed");    
            if (ImGui::Button("Collect"))
            {
                state.counter++;
                state.mineState = AppState::MineState::Init;
                state.mine_progress = 0.f;
            }
            break;
    }
}

// gui in the bottom status bar
void StatusBarGui(const AppState &appState)
{
    if (appState.mineState == AppState::MineState::Mining) {
        ImGui::Text("Mining completion: ");
        ImGui::SameLine();
        ImGui::ProgressBar(appState.mine_progress, HelloImGui::EmToVec2(12.f, 1.f));
    }
}

int main(int, char **)
{

    // Our application state
    AppState appState;

    // RunnerParams hold the settings as well as the Gui callbacks
    HelloImGui::RunnerParams runnerParams;

    runnerParams.appWindowParams.windowTitle = "Docking demo";
    runnerParams.appWindowParams.windowGeometry.size = {800, 600};
    runnerParams.appWindowParams.restorePreviousGeometry = true;


    // pass default bottom status bar the StatusBarGui
    runnerParams.imGuiWindowParams.showStatusBar = true;
    runnerParams.callbacks.ShowStatus = [&appState] { StatusBarGui(appState); };

    // ShowMenus contain the menu items in dropdown elements
    runnerParams.imGuiWindowParams.showMenuBar = true; 
    runnerParams.callbacks.ShowMenus = []() {
        if (ImGui::BeginMenu("My Menu"))
        {
            // menu items are like buttons
            if (ImGui::MenuItem("Test me"))
                HelloImGui::Log(HelloImGui::LogLevel::Debug, "It works");
            ImGui::EndMenu();
        }
    };

    //    ___________________________________________
    //    |        |                                |
    //    | Left   |                                |
    //    | Space  |    MainDockSpace               |
    //    |        |                                |
    //    |        |                                |
    //    |        |                                |
    //    -------------------------------------------
    //    |     BottomSpace                         |
    //    -------------------------------------------

    // create MainDockSpace by asking for ProvideFullScreenDockSpace
    runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

    // you can drag windows outside out the main window in order to put their content into new native windows
    runnerParams.imGuiWindowParams.enableViewports = true;

    // split MainDockSpace from the bottom to get BottomSpace (it is still above the status bar)
    HelloImGui::DockingSplit splitMainBottom;
    // BottomSpace occupies 0.25 of MainDockSpace
    splitMainBottom.initialDock = "MainDockSpace";
    splitMainBottom.newDock = "BottomSpace";
    splitMainBottom.direction = ImGuiDir_Down;
    splitMainBottom.ratio = 0.25f; 

    // split MainDockSpace from the left to get LeftSpace
    HelloImGui::DockingSplit splitMainLeft;
    // LeftSpace occupies 0.25 of the remaining MainDockSpace
    splitMainLeft.initialDock = "MainDockSpace";
    splitMainLeft.newDock = "LeftSpace";
    splitMainLeft.direction = ImGuiDir_Left;
    splitMainLeft.ratio = 0.25f;

    // make the docking splits known to runnerParams
    runnerParams.dockingParams.dockingSplits = { splitMainBottom, splitMainLeft };

    // make the dockable windows, name and assign it dockspace
    // then pass it the gui function containing the widgets

    // place the commands window in the LeftSpace, which uses CommandGui
    HelloImGui::DockableWindow commandsWindow;
    commandsWindow.label = "Commands";
    commandsWindow.dockSpaceName = "LeftSpace";
    commandsWindow.GuiFunction = [&appState]() { CommandGui(appState); };

    // place a log window in the BottomSpace, uses the inbuilt HelloImGui logger
    HelloImGui::DockableWindow logsWindow;
    logsWindow.label = "Logs";
    logsWindow.dockSpaceName = "BottomSpace";
    logsWindow.GuiFunction = [] { HelloImGui::LogGui(); };

    // the default ImGui demo is placed in MainDockSpace
    HelloImGui::DockableWindow demoWindow;
    demoWindow.label = "Dear ImGui Demo";
    demoWindow.dockSpaceName = "MainDockSpace";
    demoWindow.GuiFunction = [] { ImGui::ShowDemoWindow(); };

    // Finally, transmit these windows to HelloImGui
    runnerParams.dockingParams.dockableWindows = { commandsWindow, logsWindow, demoWindow };

    // run app
    HelloImGui::Run(runnerParams);
    return 0;
}