#include "menu.hpp"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"

namespace ig = ImGui;

namespace Menu {
    void InitializeContext(HWND hwnd) {
        if (ig::GetCurrentContext( ))
            return;

        ImGui::CreateContext( );
        ImGui_ImplWin32_Init(hwnd);

        ImGuiIO& io = ImGui::GetIO( );
        io.IniFilename = io.LogFilename = nullptr;
    }

    void Render( ) {
        if (!bShowMenu)
            return;

        // Main control window
        ig::Begin("UniversalHookX - Graphics Control", &bShowMenu, ImGuiWindowFlags_AlwaysAutoResize);
        
        ig::Text("Graphics API Hook Controller");
        ig::Separator();
        
        // Draw Call Filter Section
        ig::Spacing();
        ig::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Draw Call Filter");
        ig::Checkbox("Enable Draw Call Filter", &bEnableDrawCallFilter);
        
        if (bEnableDrawCallFilter) {
            ig::InputInt("Target Draw Call", &iTargetDrawCall);
            if (iTargetDrawCall < 0) iTargetDrawCall = 0;
            ig::Text("Current Draw Call: %d", iCurrentDrawCall);
            ig::TextWrapped("Only the selected draw call will be rendered. Use this to isolate specific objects.");
        }
        
        // Material Override Section
        ig::Spacing();
        ig::Separator();
        ig::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Material Override");
        ig::Checkbox("Enable Solid Color Material", &bEnableMaterialOverride);
        
        if (bEnableMaterialOverride) {
            ig::ColorEdit4("Solid Color", vSolidColor, ImGuiColorEditFlags_AlphaBar);
            ig::TextWrapped("Replace all materials with a solid color. Useful for identifying geometry.");
        }
        
        // Reset counters button
        ig::Spacing();
        ig::Separator();
        if (ig::Button("Reset Draw Call Counter")) {
            iCurrentDrawCall = 0;
        }
        
        // Info section
        ig::Spacing();
        ig::Separator();
        ig::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Press INSERT to toggle menu");
        
        ig::End();
        
        // Optional: Show ImGui Demo Window for reference
        // ig::ShowDemoWindow( );
    }
} // namespace Menu
