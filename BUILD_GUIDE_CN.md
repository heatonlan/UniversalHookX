# UniversalHookX 编译指南

本文档详细说明如何从源代码编译 UniversalHookX 项目。

## 目录
- [环境准备](#环境准备)
- [Windows 编译](#windows-编译)
- [跨平台编译（Linux）](#跨平台编译linux)
- [常见编译问题](#常见编译问题)
- [验证构建](#验证构建)

---

## 环境准备

### 必需组件

#### 1. Visual Studio 2019 或更高版本

**下载地址**: https://visualstudio.microsoft.com/downloads/

**必需工作负载**:
- ✅ 使用 C++ 的桌面开发
- ✅ Windows 10 SDK (10.0.19041.0 或更高)

**可选组件**:
- Visual Studio Installer 中的 CMake 工具（如果计划使用 CMake）
- Git for Windows（用于版本控制）

**安装步骤**:
1. 运行 Visual Studio Installer
2. 选择"使用 C++ 的桌面开发"工作负载
3. 在"单个组件"选项卡中，确保选中：
   - MSVC v143 - VS 2022 C++ x64/x86 生成工具
   - Windows 10 SDK (10.0.19041.0)
   - C++ ATL（可选，用于高级功能）
4. 点击"安装"或"修改"

#### 2. Vulkan SDK（可选）

**下载地址**: https://vulkan.lunarg.com/

**注意**: 仅在需要 Vulkan 后端支持时安装

**安装步骤**:
1. 下载最新版本的 Vulkan SDK
2. 运行安装程序，使用默认选项
3. 安装完成后，验证环境变量 `VULKAN_SDK` 已设置
4. 打开命令提示符，运行: `echo %VULKAN_SDK%`
5. 应该显示类似: `C:\VulkanSDK\1.x.xxx.x`

#### 3. Git（推荐）

**下载地址**: https://git-scm.com/download/win

用于克隆仓库和管理源代码。

---

## Windows 编译

### 方法 1: 使用 Visual Studio IDE（推荐）

#### 步骤 1: 获取源代码

```bash
# 使用 Git 克隆
git clone https://github.com/bruhmoment21/UniversalHookX.git
cd UniversalHookX

# 或者下载 ZIP 文件并解压
```

#### 步骤 2: 配置后端

编辑 `UniversalHookX/src/backend.hpp`:

```cpp
// 根据需要注释或取消注释以下行
#define ENABLE_BACKEND_DX9
#define ENABLE_BACKEND_DX10
#define ENABLE_BACKEND_DX11
#define ENABLE_BACKEND_DX12
#define ENABLE_BACKEND_OPENGL
// #define ENABLE_BACKEND_VULKAN  // 如果没有 Vulkan SDK，请注释此行
```

**注意**: 
- 注释掉不需要的后端可以减小 DLL 大小
- DirectX 12 在 32 位构建时会自动排除

#### 步骤 3: 打开解决方案

1. 双击 `UniversalHookX.sln`
2. Visual Studio 将自动打开项目

#### 步骤 4: 选择配置

在 Visual Studio 顶部工具栏:

**配置选项**:
- **Debug**: 包含调试信息，未优化（用于开发）
- **Release**: 优化版本，体积小（用于发布）

**平台选项**:
- **Win32 (x86)**: 32 位版本（用于 32 位目标程序）
- **x64**: 64 位版本（用于 64 位目标程序）

**推荐配置**:
- 开发测试: `Debug | x64`
- 正式使用: `Release | x64` 和 `Release | Win32`（都编译）

#### 步骤 5: 编译

**方法 A: 使用菜单**
1. 点击菜单栏 `生成` → `生成解决方案`
2. 或按快捷键 `Ctrl + Shift + B`

**方法 B: 使用右键菜单**
1. 在解决方案资源管理器中右键点击 `UniversalHookX` 项目
2. 选择 `生成`

#### 步骤 6: 查找输出文件

编译成功后，DLL 文件位于:
```
UniversalHookX/bin/
├── UniversalHookX Debug Win32.dll
├── UniversalHookX Debug x64.dll
├── UniversalHookX Release Win32.dll
└── UniversalHookX Release x64.dll
```

### 方法 2: 使用命令行（MSBuild）

适合自动化构建和 CI/CD。

#### 步骤 1: 打开开发者命令提示符

**方法 A**: 
- 开始菜单 → Visual Studio 2022 → Developer Command Prompt for VS 2022

**方法 B**:
- 打开普通命令提示符
- 运行 Visual Studio 环境设置脚本:
  ```cmd
  "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
  ```

#### 步骤 2: 导航到项目目录

```cmd
cd /d "C:\Path\To\UniversalHookX"
```

#### 步骤 3: 编译项目

**编译所有配置**:
```cmd
msbuild UniversalHookX.sln /p:Configuration=Release /p:Platform=x64
msbuild UniversalHookX.sln /p:Configuration=Release /p:Platform=Win32
msbuild UniversalHookX.sln /p:Configuration=Debug /p:Platform=x64
msbuild UniversalHookX.sln /p:Configuration=Debug /p:Platform=Win32
```

**仅编译特定配置**:
```cmd
# Release 64位
msbuild UniversalHookX.sln /p:Configuration=Release /p:Platform=x64

# Debug 32位
msbuild UniversalHookX.sln /p:Configuration=Debug /p:Platform=Win32
```

**并行构建（更快）**:
```cmd
msbuild UniversalHookX.sln /p:Configuration=Release /p:Platform=x64 /m
```

**清理构建**:
```cmd
msbuild UniversalHookX.sln /t:Clean /p:Configuration=Release /p:Platform=x64
```

#### 批处理脚本示例

创建 `build.bat` 文件:

```batch
@echo off
echo ========================================
echo UniversalHookX 自动构建脚本
echo ========================================

REM 设置 Visual Studio 环境
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

REM 清理之前的构建
echo 清理之前的构建...
msbuild UniversalHookX.sln /t:Clean /p:Configuration=Release /p:Platform=x64
msbuild UniversalHookX.sln /t:Clean /p:Configuration=Release /p:Platform=Win32

REM 编译 Release 版本
echo.
echo 编译 Release x64...
msbuild UniversalHookX.sln /p:Configuration=Release /p:Platform=x64 /m
if %errorlevel% neq 0 (
    echo 编译 x64 失败！
    pause
    exit /b 1
)

echo.
echo 编译 Release Win32...
msbuild UniversalHookX.sln /p:Configuration=Release /p:Platform=Win32 /m
if %errorlevel% neq 0 (
    echo 编译 Win32 失败！
    pause
    exit /b 1
)

echo.
echo ========================================
echo 构建成功！
echo 输出文件位于: UniversalHookX\bin\
echo ========================================
pause
```

使用方法:
```cmd
build.bat
```

---

## 跨平台编译（Linux）

虽然 UniversalHookX 主要用于 Windows，但可以在 Linux 上使用 MinGW 交叉编译。

### 安装交叉编译工具链

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install mingw-w64 wine64
```

#### Arch Linux:
```bash
sudo pacman -S mingw-w64-gcc wine
```

### 创建 Makefile

由于项目使用 Visual Studio 项目文件，需要创建自定义 Makefile 或使用 CMake。

**注意**: 这需要大量的配置工作，不推荐用于生产环境。推荐在 Windows 上编译，或使用 Wine 运行 MSBuild。

---

## 常见编译问题

### 问题 1: 无法找到 d3d12.lib

**错误信息**:
```
LINK : fatal error LNK1104: 无法打开文件"d3d12.lib"
```

**原因**: Windows SDK 未正确安装或版本过旧

**解决方案**:
1. 打开 Visual Studio Installer
2. 修改安装，确保选中"Windows 10 SDK (10.0.19041.0 或更高)"
3. 重新安装或更新 SDK

### 问题 2: Vulkan SDK 未找到

**错误信息**:
```
Cannot open include file: 'vulkan/vulkan.h'
```

**解决方案**:

**方法 A**: 安装 Vulkan SDK
1. 从 https://vulkan.lunarg.com/ 下载并安装
2. 重启 Visual Studio

**方法 B**: 禁用 Vulkan 后端
1. 编辑 `src/backend.hpp`
2. 注释掉: `// #define ENABLE_BACKEND_VULKAN`

### 问题 3: 平台工具集不匹配

**错误信息**:
```
error MSB8020: 无法找到 v143 的生成工具
```

**解决方案**:
1. 打开 `UniversalHookX.vcxproj` 文件
2. 找到 `<PlatformToolset>` 标签
3. 修改为你安装的版本:
   - Visual Studio 2019: `v142`
   - Visual Studio 2022: `v143`

或者在 Visual Studio 中:
1. 右键项目 → 属性
2. 常规 → 平台工具集
3. 选择已安装的版本

### 问题 4: DLL 体积过大

**原因**: Debug 版本包含调试信息

**解决方案**:
1. 使用 Release 配置编译
2. 在项目属性中:
   - C/C++ → 优化 → 优化 = 最小化大小 (/O1)
   - 链接器 → 调试 → 生成调试信息 = 否

### 问题 5: 缺少 DLL 依赖

**错误信息**: 运行时提示缺少 `VCRUNTIME140.dll` 等

**解决方案**:
1. 安装 Visual C++ Redistributable
   - 下载: https://aka.ms/vs/17/release/vc_redist.x64.exe (64位)
   - 下载: https://aka.ms/vs/17/release/vc_redist.x86.exe (32位)
2. 或者静态链接运行时:
   - 项目属性 → C/C++ → 代码生成
   - 运行库 = 多线程 (/MT) 或 多线程调试 (/MTd)

### 问题 6: ImGui 编译错误

**错误信息**:
```
error C2065: 'ImTextureID' : 未声明的标识符
```

**解决方案**:
确保项目属性中定义了预处理器:
1. 项目属性 → C/C++ → 预处理器
2. 预处理器定义应包含: `ImTextureID=ImU64`

这已在项目文件中配置，如果仍有问题，检查是否被覆盖。

---

## 验证构建

### 检查 DLL 有效性

#### 方法 1: 使用 Dependency Walker

1. 下载 Dependency Walker: http://www.dependencywalker.com/
2. 打开编译的 DLL
3. 检查是否有缺失的依赖项（红色标记）

#### 方法 2: 使用 dumpbin（命令行）

```cmd
dumpbin /dependents "UniversalHookX\bin\UniversalHookX Release x64.dll"
```

输出应该包含:
```
KERNEL32.dll
d3d11.lib
dxgi.lib
USER32.dll
...
```

### 测试 DLL

#### 简单测试程序

创建测试注入器或使用现有工具:

1. **使用 Process Hacker**:
   - 下载: https://processhacker.sourceforge.io/
   - 找到测试进程（如 notepad.exe）
   - 右键 → Miscellaneous → Inject DLL
   - 选择编译的 DLL

2. **检查控制台输出**:
   - DLL 注入后会创建控制台窗口
   - 应该看到类似输出:
     ```
     [+] Rendering backend: DIRECTX11
     [+] DirectX11: g_pd3dDevice: 0x...
     [+] DirectX11: Draw call hooks enabled.
     ```

3. **测试 UI**:
   - 按 INSERT 键
   - 应该显示控制界面
   - 测试各个功能

### 性能测试

编译优化版本后，可以进行性能对比:

```
无 Hook: 60 FPS
有 Hook（禁用功能）: 59-60 FPS (< 1% 影响)
启用绘制调用过滤: 57-59 FPS (2-5% 影响)
启用材质覆盖: 54-57 FPS (5-10% 影响)
```

---

## 高级构建选项

### 自定义编译宏

在项目属性 → C/C++ → 预处理器中添加:

```cpp
// 禁用控制台输出（减小体积）
DISABLE_CONSOLE

// 启用额外的调试日志
VERBOSE_LOGGING

// 禁用特定功能
DISABLE_DRAW_CALL_FILTER
DISABLE_MATERIAL_OVERRIDE
```

### 优化 DLL 体积

1. **启用链接时代码生成 (LTCG)**:
   - C/C++ → 优化 → 全程序优化 = 是
   - 链接器 → 优化 → 链接时代码生成 = 是

2. **移除未使用的代码**:
   - 链接器 → 优化 → 引用 = 是 (/OPT:REF)
   - 链接器 → 优化 → 启用 COMDAT 折叠 = 是 (/OPT:ICF)

3. **减小体积优化**:
   - C/C++ → 优化 → 优化 = 最小化大小 (/O1)
   - C/C++ → 优化 → 优选大小或速度 = 优选代码大小 (/Os)

### 创建调试符号文件（PDB）

对于 Release 版本调试:
1. 链接器 → 调试 → 生成调试信息 = 是
2. 链接器 → 调试 → 生成程序数据库文件 = `$(OutDir)$(TargetName).pdb`

---

## CI/CD 集成

### GitHub Actions 示例

创建 `.github/workflows/build.yml`:

```yaml
name: Build UniversalHookX

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: windows-latest
    
    strategy:
      matrix:
        platform: [x64, Win32]
        configuration: [Release, Debug]
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Setup MSBuild
      uses: microsoft/setup-msbuild@v1
    
    - name: Build
      run: |
        msbuild UniversalHookX.sln /p:Configuration=${{ matrix.configuration }} /p:Platform=${{ matrix.platform }} /m
    
    - name: Upload artifacts
      uses: actions/upload-artifact@v3
      with:
        name: UniversalHookX-${{ matrix.configuration }}-${{ matrix.platform }}
        path: UniversalHookX/bin/*.dll
```

---

## 故障排查清单

编译前检查:

- [ ] Visual Studio 2019/2022 已正确安装
- [ ] Windows 10 SDK 已安装
- [ ] C++ 桌面开发工作负载已安装
- [ ] 如需 Vulkan，SDK 已安装且环境变量已设置
- [ ] 源代码完整，无缺失文件
- [ ] `backend.hpp` 配置正确

编译时检查:

- [ ] 选择了正确的配置（Debug/Release）
- [ ] 选择了正确的平台（x86/x64）
- [ ] 没有语法错误
- [ ] 所有依赖库可访问

编译后检查:

- [ ] DLL 文件已生成在 `bin/` 目录
- [ ] DLL 大小合理（Release 通常 < 1MB）
- [ ] 使用 dumpbin 检查依赖项
- [ ] 在测试程序中验证功能

---

## 获取帮助

如果遇到编译问题:

1. **查看输出窗口**: Visual Studio → 视图 → 输出
2. **查看错误列表**: Visual Studio → 视图 → 错误列表
3. **搜索错误代码**: 复制错误代码在网上搜索
4. **查看文档**: 参考 Microsoft 官方文档
5. **提交 Issue**: 在 GitHub 上提交详细的问题报告

**提交 Issue 时请包含**:
- Visual Studio 版本
- Windows 版本
- 完整的错误消息
- 使用的配置和平台
- `vcxproj` 文件的相关部分

---

## 总结

本指南涵盖了从环境准备到成功编译 UniversalHookX 的完整流程。遵循这些步骤，你应该能够成功编译出可用的 DLL 文件。

**快速开始**:
1. 安装 Visual Studio 2022 + C++ 工作负载
2. 克隆项目
3. 打开 `UniversalHookX.sln`
4. 选择 `Release | x64`
5. 按 `Ctrl + Shift + B`
6. 在 `bin/` 目录找到 DLL

**推荐构建**:
- 日常使用: `Release | x64` 和 `Release | Win32`
- 开发调试: `Debug | x64`

祝编译顺利！🎉

---

**文档版本**: 1.0  
**最后更新**: 2025-10-03
