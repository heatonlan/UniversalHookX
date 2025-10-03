#pragma once

#include <Windows.h>

namespace Menu {
    void InitializeContext(HWND hwnd);
    void Render( );

    inline bool bShowMenu = true;
    
    // Draw call filtering
    inline bool bEnableDrawCallFilter = false;
    inline int iTargetDrawCall = 0;
    inline int iCurrentDrawCall = 0;
    
    // Material override
    inline bool bEnableMaterialOverride = false;
    inline float vSolidColor[4] = {1.0f, 0.0f, 0.0f, 1.0f}; // RGBA - Default Red
} // namespace Menu
