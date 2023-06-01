#include "hello_imgui/hello_imgui.h"


int main(int , char *[])
{
    // struct containing all the application params (run params to run the application)
    HelloImGui::RunnerParams params;
    params.appWindowParams.windowGeometry.size = {800, 600};
    params.appWindowParams.windowTitle = "Hello ImGui!";
    params.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;

    // window states
    bool show_window_1 = false;
    
    // contains functions for widgets
    params.callbacks.ShowGui = [&]() 
    { 
        {
            // main widget states
            static float f = 0.0f;
            static int counter = 0;

            // show demo window with a Begin/End pair
            ImGui::Begin("Hello, World!");

            ImGui::Text("Some text here.");
            ImGui::Checkbox("Window 1", &show_window_1);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&params.imGuiWindowParams.backgroundColor); // Edit 3 floats representing a color


            // buttons return true when clicked
            if (ImGui::Button("Increment")) 
            {
                counter++;
            } 
            
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (show_window_1) 
        {
            // the window will have a close button to negate the bool when clicked
            ImGui::Begin("Window 1", &show_window_1);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_window_1 = false;
            ImGui::End();
        }

    };

    // run the app
    HelloImGui::Run(params);
    return 0;
}
