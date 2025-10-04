# UniversalHookX - 通用图形API钩子 ![C++](https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg) ![Windows](https://img.shields.io/badge/platform-Windows-0078d7.svg)

## 项目简介

UniversalHookX 是一个通用的 Windows 图形 API 钩子库，可以拦截和修改应用程序的图形渲染调用。该项目支持多种图形 API，并使用 ImGui 提供可视化的控制界面。

### 主要特性

- ✅ 支持多种图形 API（DirectX 9/10/11/12, OpenGL, Vulkan）
- ✅ 实时拦截绘制调用（Draw Calls）
- ✅ 选择性渲染特定的绘制调用
- ✅ 纯色材质覆盖功能
- ✅ 基于 ImGui 的友好用户界面
- ✅ 使用 MinHook 进行函数钩子

## 支持的图形后端

| 后端 | 状态 | 说明 |
|------|------|------|
| DirectX 9 | ✅ 支持 | 包括 DirectX9Ex |
| DirectX 10 | ✅ 支持 | |
| DirectX 11 | ✅ 支持 | 增强功能完整 |
| DirectX 12 | ✅ 支持 | 不支持 Windows 7 |
| OpenGL | ✅ 支持 | |
| Vulkan | ⚠️ 部分支持 | 某些游戏可能有问题 |

## 编译说明

### 系统要求

- **操作系统**: Windows 10/11
- **IDE**: Visual Studio 2019 或更高版本
- **平台工具集**: v143 或更高
- **Windows SDK**: 10.0
- **Vulkan SDK**: 用于 Vulkan 后端支持（可选）

### 编译步骤

1. **安装必要的工具**
   ```
   - Visual Studio 2019/2022（含 C++ 桌面开发工作负载）
   - Windows 10 SDK
   - Vulkan SDK（如果需要 Vulkan 支持）
   ```

2. **配置 Vulkan 环境变量（可选）**
   - 安装 Vulkan SDK 后，确保设置了 `VULKAN_SDK` 环境变量
   - 通常安装程序会自动设置此变量

3. **打开解决方案**
   ```
   打开 UniversalHookX.sln
   ```

4. **选择配置**
   - **Debug|Win32**: 32位调试版本
   - **Release|Win32**: 32位发布版本（优化）
   - **Debug|x64**: 64位调试版本
   - **Release|x64**: 64位发布版本（优化）

5. **编译项目**
   ```
   生成 -> 生成解决方案 (Ctrl+Shift+B)
   ```

6. **输出位置**
   ```
   编译后的 DLL 文件位于: UniversalHookX/bin/
   文件名格式: UniversalHookX [配置] [平台].dll
   例如: UniversalHookX Release x64.dll
   ```

### 编译选项

在 `src/backend.hpp` 中可以启用/禁用特定的后端：

```cpp
#define ENABLE_BACKEND_DX9      // DirectX 9
#define ENABLE_BACKEND_DX10     // DirectX 10
#define ENABLE_BACKEND_DX11     // DirectX 11
#define ENABLE_BACKEND_DX12     // DirectX 12
#define ENABLE_BACKEND_OPENGL   // OpenGL
// #define ENABLE_BACKEND_VULKAN   // Vulkan（注释掉=禁用）
```

注释掉不需要的后端可以减小 DLL 体积。

## 使用说明

### 基本使用

1. **选择渲染后端**
   
   在 `src/dllmain.cpp` 中设置目标应用程序使用的图形 API：
   ```cpp
   U::SetRenderingBackend(DIRECTX11);  // 可选: DIRECTX9, DIRECTX10, DIRECTX11, DIRECTX12, OPENGL, VULKAN
   ```

2. **注入 DLL**
   
   使用 DLL 注入器将编译好的 DLL 注入到目标进程：
   - 确保 DLL 架构（32位/64位）与目标进程匹配
   - 推荐工具: Process Hacker, Extreme Injector, 或自定义注入器

3. **使用界面**
   
   注入成功后，按 **INSERT** 键显示/隐藏控制界面

### 新增功能使用指南

#### 1. 绘制调用过滤器（Draw Call Filter）

该功能允许你只渲染特定的绘制调用，用于隔离和分析特定的3D对象。

**使用步骤：**

1. 启用"Enable Draw Call Filter"复选框
2. 在"Target Draw Call"输入框中输入要显示的绘制调用编号
3. "Current Draw Call"显示当前帧的总绘制调用数
4. 只有匹配的绘制调用会被渲染，其他的会被跳过

**应用场景：**
- 🎯 定位特定的游戏对象（角色、武器、敌人等）
- 🔍 分析渲染性能问题
- 🛠️ 逆向工程游戏渲染管线
- 🎨 识别特定模型的绘制顺序

**示例：**
```
假设一个场景有100个绘制调用：
- 设置 Target Draw Call = 42
- 结果：只有第42个绘制调用会被渲染，其他99个会被隐藏
```

#### 2. 纯色材质覆盖（Material Override）

该功能将所有对象的材质替换为单一纯色，方便识别几何体结构。

**使用步骤：**

1. 启用"Enable Solid Color Material"复选框
2. 使用颜色选择器选择想要的颜色
3. 颜色选择器支持 RGB 和 Alpha 通道调整
4. 所有渲染的对象将使用选定的颜色

**应用场景：**
- 🎨 识别重叠的几何体
- 📐 分析模型结构
- 🔧 调试 Z-fighting 问题
- 👁️ 突出显示特定对象（配合绘制调用过滤器）

**颜色推荐：**
- 红色 (1.0, 0.0, 0.0, 1.0) - 高对比度，易于识别
- 绿色 (0.0, 1.0, 0.0, 1.0) - 护眼，适合长时间观察
- 蓝色 (0.0, 0.0, 1.0, 1.0) - 冷色调，适合技术分析
- 白色 (1.0, 1.0, 1.0, 1.0) - 全亮度，检查轮廓

#### 3. 组合使用

最强大的功能是将两个特性组合使用：

**工作流程示例：**

1. **第一步：扫描绘制调用**
   - 禁用过滤器
   - 启用纯色材质（使用红色）
   - 观察总绘制调用数

2. **第二步：定位目标对象**
   - 启用绘制调用过滤器
   - 从 0 开始，逐个测试绘制调用
   - 找到目标对象对应的绘制调用编号

3. **第三步：分析和修改**
   - 锁定目标绘制调用
   - 改变纯色以区分不同部分
   - 记录重要的绘制调用编号

### 快捷键

| 快捷键 | 功能 |
|--------|------|
| **INSERT** | 显示/隐藏菜单 |
| **HOME** | 重新钩取图形 API（如果出现问题） |

## 技术原理

### 钩子机制

UniversalHookX 使用 MinHook 库实现函数钩子：

1. **虚表钩取（VTable Hooking）**
   - 创建临时设备获取虚函数表指针
   - 提取关键函数地址
   - 使用 MinHook 安装钩子

2. **钩取的关键函数**

#### DirectX 11 示例：
```cpp
// 交换链函数
- IDXGISwapChain::Present
- IDXGISwapChain::Present1
- IDXGISwapChain::ResizeBuffers
- IDXGISwapChain::ResizeBuffers1

// 工厂函数
- IDXGIFactory::CreateSwapChain
- IDXGIFactory::CreateSwapChainForHwnd
- IDXGIFactory::CreateSwapChainForCoreWindow
- IDXGIFactory::CreateSwapChainForComposition

// 绘制函数（新增）
- ID3D11DeviceContext::Draw
- ID3D11DeviceContext::DrawIndexed
```

### 绘制调用拦截原理

```
应用程序绘制流程：
┌─────────────────────────────────────┐
│  应用调用 DrawIndexed()              │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  我们的钩子函数 hkDrawIndexed()      │
│  ├─ 增加计数器                       │
│  ├─ 检查是否启用过滤                 │
│  ├─ 检查是否匹配目标                 │
│  └─ 应用材质覆盖（如果启用）         │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  原始 DrawIndexed() - 如果允许执行   │
└─────────────────────────────────────┘
```

### 材质覆盖实现

材质覆盖通过替换着色器实现：

```
正常渲染流程：
应用的顶点着色器 → 应用的像素着色器 → 输出

覆盖后的流程：
应用的顶点着色器 → 我们的纯色像素着色器 → 纯色输出
```

着色器代码结构：
```hlsl
// 顶点着色器 - 传递顶点位置
struct VS_INPUT { float4 pos : POSITION; };
struct PS_INPUT { float4 pos : SV_POSITION; };
PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    output.pos = input.pos;
    return output;
}

// 像素着色器 - 输出纯色
cbuffer ColorBuffer : register(b0) { 
    float4 solidColor; 
};
float4 main() : SV_TARGET { 
    return solidColor; 
}
```

## 代码结构

```
UniversalHookX/
├── src/
│   ├── dllmain.cpp              # DLL 入口点
│   ├── backend.hpp              # 后端配置
│   │
│   ├── console/                 # 控制台输出
│   │   ├── console.cpp
│   │   └── console.hpp
│   │
│   ├── menu/                    # UI 菜单（新增功能）
│   │   ├── menu.cpp            # 渲染菜单和控件
│   │   └── menu.hpp            # 菜单状态变量
│   │
│   ├── hooks/                   # 钩子实现
│   │   ├── hooks.cpp           # 钩子管理
│   │   ├── hooks.hpp
│   │   └── backend/            # 各后端实现
│   │       ├── dx9/
│   │       ├── dx10/
│   │       ├── dx11/           # DirectX 11（增强功能完整）
│   │       │   ├── hook_directx11.cpp
│   │       │   └── hook_directx11.hpp
│   │       ├── dx12/
│   │       ├── opengl/
│   │       └── vulkan/
│   │
│   ├── utils/                   # 工具函数
│   │   ├── utils.cpp
│   │   └── utils.hpp
│   │
│   └── dependencies/            # 第三方库
│       ├── imgui/              # ImGui UI 库
│       └── minhook/            # MinHook 钩子库
│
├── UniversalHookX.sln          # Visual Studio 解决方案
├── UniversalHookX.vcxproj      # 项目文件
└── README.md                   # 英文文档
```

## 开发指南

### 添加自定义功能

1. **修改菜单界面**
   
   编辑 `src/menu/menu.cpp` 中的 `Render()` 函数：
   ```cpp
   void Render() {
       if (!bShowMenu)
           return;
       
       ig::Begin("My Custom Window");
       // 添加你的 ImGui 控件
       ig::Text("Hello World");
       ig::Button("Click Me");
       ig::End();
   }
   ```

2. **添加新的钩子**
   
   在对应的后端文件中（例如 `hook_directx11.cpp`）：
   ```cpp
   // 定义原始函数指针
   static std::add_pointer_t<返回类型 WINAPI(参数...)> oOriginalFunc;
   
   // 定义钩子函数
   static 返回类型 WINAPI hkMyFunction(参数...) {
       // 你的自定义逻辑
       return oOriginalFunc(参数...);
   }
   
   // 在 Hook() 函数中安装钩子
   MH_CreateHook(函数地址, &hkMyFunction, &oOriginalFunc);
   MH_EnableHook(函数地址);
   ```

3. **添加全局状态**
   
   在 `src/menu/menu.hpp` 中：
   ```cpp
   namespace Menu {
       // 现有变量...
       
       // 你的新变量
       inline bool bMyFeature = false;
       inline float fMyValue = 1.0f;
   }
   ```

### 支持新的图形后端

1. 在 `src/hooks/backend/` 下创建新目录
2. 实现 `Hook()` 和 `Unhook()` 函数
3. 在 `backend.hpp` 中添加 `#define ENABLE_BACKEND_XXX`
4. 在 `hooks.cpp` 中添加对新后端的调用

## 已知问题和解决方案

### 常见问题

1. **与其他覆盖层冲突**
   - ⚠️ 问题：与 MSI Afterburner 等程序冲突可能导致崩溃
   - ✅ 解决：禁用其他覆盖层程序

2. **Vulkan 后端问题**
   - ⚠️ 问题：某些游戏（如 DOOM Eternal）可能出现画面抖动
   - ✅ 解决：按 HOME 键重新钩取，或切换到其他后端

3. **OpenGL 纹理故障**
   - ⚠️ 问题：Minecraft 等游戏 UI 纹理可能出现故障
   - ✅ 解决：这是已知问题，通常不影响功能

4. **DLL 注入失败**
   - ⚠️ 问题：注入器报错或 DLL 无法加载
   - ✅ 解决：
     - 检查 DLL 和目标进程架构是否匹配（32位/64位）
     - 以管理员身份运行注入器
     - 检查杀毒软件是否拦截

5. **菜单不显示**
   - ⚠️ 问题：注入成功但按 INSERT 键无反应
   - ✅ 解决：
     - 确认后端设置正确
     - 按 HOME 键重新钩取
     - 检查控制台输出（如果可见）

### 调试技巧

1. **启用控制台输出**
   ```cpp
   // 在 dllmain.cpp 中，控制台会自动分配
   // 查看日志输出诊断问题
   ```

2. **检查钩子状态**
   ```cpp
   // 钩子创建时会输出日志
   LOG("[+] DirectX11: Draw call hooks enabled.\n");
   ```

3. **验证函数地址**
   ```cpp
   LOG("[+] Function address: 0x%p\n", functionAddress);
   ```

## 性能考虑

### 性能影响

- **正常模式**（无过滤，无覆盖）：< 1% 性能影响
- **绘制调用过滤**：2-5% 性能影响（取决于绘制调用数量）
- **材质覆盖**：5-10% 性能影响（需要着色器切换）
- **组合模式**：5-10% 性能影响

### 优化建议

1. 只在需要时启用功能
2. 不使用时完全禁用过滤和覆盖
3. 避免每帧频繁切换设置
4. 在高性能场景下关闭调试输出

## 应用场景

### 游戏逆向工程
- 🎮 分析游戏渲染管线
- 🔍 定位特定游戏对象
- 📊 测量渲染性能
- 🛠️ 开发游戏模组

### 图形开发调试
- 🐛 调试渲染问题
- 🎨 验证材质和着色器
- 📐 分析几何体结构
- ⚡ 优化绘制调用

### 研究和学习
- 📚 学习现代图形 API
- 🔬 研究游戏引擎架构
- 💡 理解渲染优化技术
- 🎓 教学演示工具

## 安全和法律说明

⚠️ **重要提示**：

1. **仅用于学习和研究目的**
2. **不要在在线多人游戏中使用**（可能被视为作弊）
3. **遵守游戏和软件的服务条款**
4. **不要用于商业用途而不获得许可**
5. **自行承担使用风险**

## 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

### 代码风格

- 使用一致的缩进（4个空格）
- 函数和变量使用有意义的命名
- 添加必要的注释
- 遵循现有代码风格

## 更新日志

### v2.0 (当前版本)
- ✨ 新增：绘制调用过滤器
- ✨ 新增：纯色材质覆盖功能
- ✨ 新增：改进的 ImGui 控制界面
- 🔧 改进：DirectX 11 后端增强
- 📝 新增：完整的中文文档

### v1.0 (原始版本)
- ✅ 支持 DirectX 9/10/11/12
- ✅ 支持 OpenGL 和 Vulkan
- ✅ 基础 ImGui 集成
- ✅ MinHook 钩子系统

## 依赖项

| 库 | 版本 | 用途 | 许可证 |
|----|------|------|--------|
| [MinHook](https://github.com/TsudaKageyu/minhook) | - | 函数钩取 | BSD 2-Clause |
| [ImGui](https://github.com/ocornut/imgui) | - | 用户界面 | MIT |
| [Vulkan SDK](https://vulkan.lunarg.com/) | - | Vulkan API 支持 | Apache 2.0 |

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

## 联系方式

- **问题反馈**: 通过 GitHub Issues 提交
- **功能建议**: 通过 GitHub Discussions 讨论
- **安全漏洞**: 请私下联系维护者

## 致谢

- TsudaKageyu - MinHook 作者
- ocornut - ImGui 作者
- Khronos Group - Vulkan API
- Microsoft - DirectX SDK
- 所有贡献者和测试者

## FAQ（常见问题解答）

### Q: 如何确定目标程序使用的图形 API？
A: 可以使用以下方法：
- 使用 Process Hacker 查看加载的 DLL（d3d11.dll, opengl32.dll 等）
- 查看游戏设置中的图形 API 选项
- 尝试不同的后端设置，查看哪个有效

### Q: 为什么有些绘制调用无法过滤？
A: 可能的原因：
- 某些绘制调用使用了不同的函数（如 DrawInstanced）
- 后处理效果在不同的渲染阶段
- 需要钩取更多的绘制函数

### Q: 材质覆盖不生效怎么办？
A: 检查以下几点：
- 确认"Enable Solid Color Material"已启用
- 某些渲染技术（如延迟渲染）可能需要特殊处理
- 查看控制台是否有错误信息

### Q: 可以同时使用多个后端吗？
A: 不可以。每次只能选择一个后端，需要在编译时启用，运行时选择。

### Q: 如何获取更多帮助？
A: 
1. 查看控制台输出日志
2. 在 GitHub Issues 中搜索类似问题
3. 提交新的 Issue 并附上详细信息（目标程序、配置、错误信息等）

---

**最后更新**: 2025-10-03  
**文档版本**: 2.0  
**项目状态**: 活跃开发中

如果本项目对你有帮助，请给个 ⭐ Star！
