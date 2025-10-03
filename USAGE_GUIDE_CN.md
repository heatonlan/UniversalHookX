# UniversalHookX 使用指南

本文档详细介绍如何使用 UniversalHookX 的各项功能。

## 目录
- [快速入门](#快速入门)
- [DLL 注入方法](#dll-注入方法)
- [功能详解](#功能详解)
- [实战案例](#实战案例)
- [进阶技巧](#进阶技巧)
- [常见问题](#常见问题)

---

## 快速入门

### 5分钟上手

#### 第1步: 准备文件

1. 确保你已编译好 DLL，或从 Release 页面下载
2. 根据目标程序选择正确的版本:
   - 32位程序 → `UniversalHookX Release Win32.dll`
   - 64位程序 → `UniversalHookX Release x64.dll`

**如何判断程序是32位还是64位?**

**方法 A**: 任务管理器
1. 打开任务管理器 (Ctrl+Shift+Esc)
2. 找到目标进程
3. 32位程序会显示 `(32位)` 标记

**方法 B**: Process Hacker
1. 下载并运行 Process Hacker
2. 找到目标进程
3. 查看 "Platform" 列: `32-bit` 或 `64-bit`

#### 第2步: 配置后端

编辑 `src/dllmain.cpp` 中的第19行:

```cpp
U::SetRenderingBackend(DIRECTX11);  // 根据目标程序修改
```

**如何确定目标程序使用的图形API?**

| 图形API | 常见游戏/程序 | 判断方法 |
|---------|-------------|----------|
| DirectX 9 | 老游戏（2010年前） | 加载 `d3d9.dll` |
| DirectX 10 | 较少见 | 加载 `d3d10.dll` |
| DirectX 11 | 大多数现代游戏 | 加载 `d3d11.dll` |
| DirectX 12 | 最新游戏（2015年后） | 加载 `d3d12.dll` |
| OpenGL | Minecraft, 独立游戏 | 加载 `opengl32.dll` |
| Vulkan | DOOM, Quake | 加载 `vulkan-1.dll` |

使用 Process Hacker 查看进程加载的 DLL:
1. 右键进程 → Properties
2. 切换到 "Modules" 标签页
3. 搜索上述 DLL 文件名

#### 第3步: 注入 DLL

使用 DLL 注入器将 DLL 注入到目标进程。

**推荐注入器**:
1. **Process Hacker** (推荐新手)
2. **Extreme Injector**
3. **Xenos Injector**

#### 第4步: 使用界面

1. 注入成功后，目标程序会出现控制台窗口
2. 按 **INSERT** 键显示控制界面
3. 开始使用各项功能！

---

## DLL 注入方法

### 方法 1: Process Hacker (推荐)

**优点**: 免费、开源、功能强大、稳定

**步骤**:

1. **下载并安装 Process Hacker**
   - 官网: https://processhacker.sourceforge.io/
   - 选择与你的系统匹配的版本

2. **以管理员身份运行**
   - 右键 Process Hacker → 以管理员身份运行

3. **找到目标进程**
   - 在进程列表中找到目标程序
   - 可以使用搜索框快速定位

4. **注入 DLL**
   - 右键目标进程
   - Miscellaneous → Inject DLL...
   - 浏览并选择编译好的 UniversalHookX DLL
   - 点击 "Inject"

5. **验证注入**
   - 目标程序会弹出控制台窗口
   - 控制台显示初始化信息
   - 按 INSERT 键测试界面

**截图参考**:
```
Process Hacker
  ├─ 右键目标进程
  ├─ Miscellaneous
  └─ Inject DLL
      ├─ 选择 DLL 文件
      └─ 点击 Inject
```

### 方法 2: Extreme Injector

**优点**: 界面友好、功能多样、支持多种注入方式

**步骤**:

1. **下载 Extreme Injector**
   - 搜索 "Extreme Injector v3"
   - 注意: 可能被杀毒软件误报，需添加例外

2. **运行注入器**
   - 以管理员身份运行

3. **选择进程和 DLL**
   - Process: 选择目标进程
   - DLL: 添加 UniversalHookX DLL

4. **配置注入设置**
   - Injection Method: Standard Injection
   - 勾选 "Close Handle"
   - 其他选项保持默认

5. **执行注入**
   - 点击 "Inject"
   - 等待成功提示

### 方法 3: 手动注入 (高级)

使用自己的注入器或脚本:

```cpp
// C++ 注入器示例（简化版）
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcessIdByName(const char* processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    
    if (Process32First(snapshot, &entry)) {
        do {
            if (!strcmp(entry.szExeFile, processName)) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));
    }
    
    CloseHandle(snapshot);
    return 0;
}

bool InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) return false;
    
    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, 
                                          MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pRemoteMemory, dllPath, strlen(dllPath) + 1, NULL);
    
    HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
    LPVOID pLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
    
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, 
                                        (LPTHREAD_START_ROUTINE)pLoadLibrary, 
                                        pRemoteMemory, 0, NULL);
    
    WaitForSingleObject(hThread, INFINITE);
    
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    
    return true;
}

int main() {
    DWORD pid = GetProcessIdByName("target.exe");
    if (pid == 0) {
        printf("进程未找到!\n");
        return 1;
    }
    
    if (InjectDLL(pid, "C:\\Path\\To\\UniversalHookX.dll")) {
        printf("注入成功!\n");
    } else {
        printf("注入失败!\n");
    }
    
    return 0;
}
```

### 注入注意事项

⚠️ **重要提示**:

1. **架构匹配**: 32位DLL注入32位程序，64位DLL注入64位程序
2. **管理员权限**: 注入器需要以管理员身份运行
3. **杀毒软件**: 可能需要添加例外，避免被拦截
4. **反作弊系统**: 在线游戏可能检测并封禁账号
5. **时机选择**: 最好在程序完全启动后再注入

---

## 功能详解

### 1. 绘制调用过滤器 (Draw Call Filter)

#### 什么是绘制调用?

绘制调用 (Draw Call) 是 CPU 向 GPU 发送渲染指令的过程。每个绘制调用通常对应一个3D对象或一组对象。

**示例场景**:
```
场景: 一个角色在房间里
- Draw Call 0-10: 房间墙壁
- Draw Call 11-15: 房间家具
- Draw Call 16-20: 角色身体
- Draw Call 21-25: 角色武器
- Draw Call 26-30: UI元素
```

#### 如何使用

**基础使用**:

1. **启用过滤器**
   ```
   勾选 "Enable Draw Call Filter"
   ```

2. **设置目标绘制调用**
   ```
   Target Draw Call: [输入数字]
   例如: 16
   ```

3. **观察结果**
   - 只有第16个绘制调用会被渲染
   - 其他对象会消失
   - "Current Draw Call" 显示总数

**查找特定对象的绘制调用**:

**方法 A: 线性搜索**
```
1. 启用过滤器
2. Target Draw Call = 0
3. 观察画面
4. 增加数字 (1, 2, 3, ...)
5. 找到目标对象时记录编号
```

**方法 B: 二分搜索 (快速)**
```
假设有100个绘制调用:

1. 测试 50:
   - 如果目标可见 → 在 0-50 之间
   - 如果目标不可见 → 在 51-100 之间

2. 假设在 0-50，测试 25:
   - 如果目标可见 → 在 0-25 之间
   - 如果目标不可见 → 在 26-50 之间

3. 重复直到找到精确编号
```

**方法 C: 区间扫描**
```
1. 禁用过滤器，记录总数: 150
2. 分段测试:
   - 0-50: 未找到
   - 51-100: 找到目标
   - 细化到 51-100 中的具体编号
```

#### 高级技巧

**多对象映射**:
创建绘制调用映射表:

```
绘制调用映射 - 游戏名称
====================================
DC 0-5:    天空盒
DC 6-20:   地形
DC 21-30:  建筑物
DC 31-35:  玩家角色
  DC 31:   头部
  DC 32:   身体
  DC 33:   手臂
  DC 34:   腿部
  DC 35:   武器
DC 36-50:  敌人
DC 51-60:  特效
DC 61-70:  UI
====================================
```

**制作工具**:
使用脚本自动扫描和记录:

```python
# 伪代码
for dc in range(0, max_draw_calls):
    set_target_draw_call(dc)
    screenshot = capture_screen()
    if has_object(screenshot):
        log(f"Object found at DC {dc}")
```

### 2. 纯色材质覆盖 (Solid Color Material Override)

#### 功能说明

将所有对象的纹理和材质替换为单一纯色，使几何体结构更清晰。

#### 如何使用

**基础使用**:

1. **启用覆盖**
   ```
   勾选 "Enable Solid Color Material"
   ```

2. **选择颜色**
   - 点击颜色框打开选择器
   - 调整 RGB 和 Alpha 值
   - 实时预览效果

3. **观察结果**
   - 所有对象变为纯色
   - 保留几何形状和深度
   - 纹理被移除

**颜色选择指南**:

| 颜色 | RGB 值 | 用途 |
|------|--------|------|
| 🔴 红色 | (1, 0, 0) | 高对比度，易识别重叠 |
| 🟢 绿色 | (0, 1, 0) | 护眼，长时间分析 |
| 🔵 蓝色 | (0, 0, 1) | 冷色调，技术分析 |
| ⚪ 白色 | (1, 1, 1) | 检查轮廓和边缘 |
| ⚫ 黑色 | (0, 0, 0) | 检查光照效果 |
| 🟡 黄色 | (1, 1, 0) | 警示色，突出显示 |
| 🟣 紫色 | (1, 0, 1) | 区分不同对象 |
| 🟠 橙色 | (1, 0.5, 0) | 暖色调，容易注意 |

**半透明效果**:
调整 Alpha 通道 (A) 可以看到内部结构:
```
Alpha = 1.0: 完全不透明
Alpha = 0.5: 半透明
Alpha = 0.1: 几乎透明
```

#### 应用场景

**场景 1: 检测模型重叠**
```
问题: 怀疑多个模型重叠导致性能问题
解决:
1. 启用纯色覆盖 (红色)
2. 观察是否有颜色更深的区域
3. 深色区域 = 多个对象重叠
```

**场景 2: 分析模型结构**
```
问题: 想了解角色模型的组成
解决:
1. 启用绘制调用过滤器
2. 启用纯色覆盖 (不同颜色)
3. 逐个绘制调用分析，用不同颜色标记
   - DC 31 (头部): 红色
   - DC 32 (身体): 绿色
   - DC 33 (手臂): 蓝色
```

**场景 3: 调试 Z-Fighting**
```
问题: 两个面闪烁 (Z-Fighting)
解决:
1. 启用纯色覆盖 (白色)
2. 观察闪烁是否持续
3. 如果消失，说明是纹理问题
4. 如果持续，说明是深度缓冲问题
```

### 3. 组合使用技巧

#### 技巧 1: 彩色高亮特定对象

**目标**: 让敌人显示为红色，其他保持正常

**步骤**:
1. 找到敌人的绘制调用编号 (假设是 DC 40-50)
2. 启用过滤器，设置 Target = 40
3. 启用材质覆盖，颜色 = 红色
4. 截图保存
5. 重复 DC 41-50
6. 禁用过滤器，查看完整画面

**局限**: 每次只能高亮一个，需要手动切换

#### 技巧 2: 性能分析

**目标**: 找出消耗最多性能的绘制调用

**步骤**:
1. 记录无过滤时的 FPS
2. 启用过滤器，逐个测试绘制调用
3. 记录每个DC的FPS
4. 找出 FPS 最低的 DC
5. 该 DC 可能包含复杂模型

**示例数据**:
```
DC 0-30: 60 FPS (简单对象)
DC 31-35: 45 FPS (复杂角色模型)
DC 36-50: 58 FPS (中等复杂度)
→ 结论: DC 31-35 是性能瓶颈
```

#### 技巧 3: 逆向工程渲染管线

**目标**: 理解游戏的渲染顺序

**步骤**:
1. 启用过滤器 + 纯色覆盖
2. 从 DC 0 开始记录:
   ```
   DC 0-5:   天空盒 (先渲染背景)
   DC 6-30:  不透明对象 (由远到近)
   DC 31-40: 半透明对象 (由近到远)
   DC 41-50: UI (最后渲染)
   ```
3. 分析渲染优化策略

#### 技巧 4: 创建线框模式

**目标**: 类似传统的线框视图

**步骤**:
1. 启用纯色覆盖
2. 设置颜色为黑色 (0, 0, 0)
3. Alpha 设置为 0.1 (非常透明)
4. 结果: 类似线框效果

**注意**: 这不是真正的线框，只是视觉近似

---

## 实战案例

### 案例 1: 在 Minecraft 中高亮矿石

**目标**: 让钻石矿石显示为亮蓝色

**环境**:
- 游戏: Minecraft Java Edition
- 图形API: OpenGL
- 架构: 64位

**步骤**:

1. **准备工作**
   ```
   - 编译 UniversalHookX (OpenGL 后端)
   - 设置: U::SetRenderingBackend(OPENGL);
   - 编译 x64 版本
   ```

2. **注入 DLL**
   ```
   - 启动 Minecraft
   - 进入单人世界
   - 使用 Process Hacker 注入 DLL
   - 验证控制台出现
   ```

3. **定位钻石矿石**
   ```
   - 找到钻石矿石
   - 按 INSERT 显示菜单
   - 启用 Draw Call Filter
   - Target DC = 0
   - 逐个增加，观察矿石是否消失
   - 假设在 DC 127 时，只有钻石可见
   ```

4. **应用高亮**
   ```
   - 保持 Target DC = 127
   - 启用 Solid Color Material
   - 颜色设置为亮蓝色 (0, 1, 1)
   - 钻石现在是亮蓝色
   ```

5. **局限性**
   ```
   ⚠️ 这只在测试环境有效
   ⚠️ 真实使用会被视为作弊
   ⚠️ 仅用于学习渲染原理
   ```

### 案例 2: 分析 DirectX 11 游戏角色模型

**目标**: 了解角色模型由多少部分组成

**环境**:
- 游戏: 某款 DirectX 11 游戏
- 架构: 64位

**步骤**:

1. **初始扫描**
   ```
   - 注入 DLL
   - 禁用过滤器，观察 Current Draw Call
   - 假设总数: 250
   ```

2. **定位角色**
   ```
   - 使用二分搜索:
     - 测试 DC 125: 角色不可见
     - 测试 DC 187: 角色可见
     - 测试 DC 156: 角色部分可见
     - 细化到 DC 150-160
   ```

3. **分解部件**
   ```
   DC 150: 头部 (头发、脸部)
   DC 151: 头部 (眼睛)
   DC 152: 身体 (躯干)
   DC 153: 身体 (装备)
   DC 154: 手臂 (左)
   DC 155: 手臂 (右)
   DC 156: 腿部 (左)
   DC 157: 腿部 (右)
   DC 158: 武器 (主手)
   DC 159: 武器 (副手)
   DC 160: 阴影投射
   ```

4. **彩色标记**
   ```
   - 对每个部件使用不同颜色
   - 截图保存
   - 创建文档记录
   ```

5. **分析结果**
   ```
   发现:
   - 角色共 11 个绘制调用
   - 每个身体部位独立
   - 武器和装备动态附加
   - 阴影单独渲染
   
   优化建议:
   - 远距离可以合并部件
   - LOD系统减少细节
   ```

### 案例 3: 调试重叠面问题

**目标**: 找出导致闪烁的重叠面

**环境**:
- 自制 DirectX 11 应用
- 问题: 某些面闪烁

**步骤**:

1. **重现问题**
   ```
   - 运行应用
   - 观察闪烁位置
   ```

2. **启用纯色覆盖**
   ```
   - 注入 DLL
   - 启用 Solid Color Material (白色)
   - 观察闪烁是否持续
   ```

3. **隔离问题绘制调用**
   ```
   - 启用 Draw Call Filter
   - 测试每个 DC
   - 找到两个会导致闪烁的 DC
   - 假设: DC 42 和 DC 43
   ```

4. **分析原因**
   ```
   - 两个 DC 渲染同一位置
   - 深度值完全相同
   - 导致 Z-Fighting
   ```

5. **解决方案**
   ```
   在原始代码中:
   - 调整其中一个面的深度偏移
   - 或合并两个绘制调用
   - 或禁用其中一个
   ```

---

## 进阶技巧

### 热键绑定

虽然默认只有 INSERT 键，但可以修改代码添加更多热键:

编辑 `menu.cpp`:

```cpp
void Render() {
    // 检测热键
    if (GetAsyncKeyState(VK_F1) & 1) {
        bEnableDrawCallFilter = !bEnableDrawCallFilter;
    }
    
    if (GetAsyncKeyState(VK_F2) & 1) {
        bEnableMaterialOverride = !bEnableMaterialOverride;
    }
    
    if (GetAsyncKeyState(VK_F3) & 1) {
        iTargetDrawCall++;
    }
    
    if (GetAsyncKeyState(VK_F4) & 1) {
        iTargetDrawCall--;
        if (iTargetDrawCall < 0) iTargetDrawCall = 0;
    }
    
    // 原有的渲染代码...
}
```

**建议热键映射**:
```
F1: 切换绘制调用过滤器
F2: 切换材质覆盖
F3: 增加目标 DC
F4: 减少目标 DC
F5: 重置计数器
F6: 切换菜单
```

### 自动化扫描脚本

使用 Python 和 pywin32 自动化操作:

```python
import win32gui
import win32api
import win32con
import time
from PIL import ImageGrab

def press_key(key):
    """模拟按键"""
    win32api.keybd_event(key, 0, 0, 0)
    time.sleep(0.05)
    win32api.keybd_event(key, 0, win32con.KEYEVENT_KEYUP, 0)

def scan_draw_calls(max_dc=200):
    """扫描所有绘制调用并截图"""
    for dc in range(max_dc):
        # 模拟输入 DC 编号 (需要自己实现)
        # set_target_dc(dc)
        
        # 等待渲染
        time.sleep(0.1)
        
        # 截图
        screenshot = ImageGrab.grab()
        screenshot.save(f"dc_{dc:03d}.png")
        
        print(f"扫描 DC {dc} 完成")
        
        # 下一个 DC (按 F3)
        press_key(0x72)  # F3

# 运行扫描
scan_draw_calls(200)
```

### 日志记录

添加详细的日志记录功能:

编辑 `menu.cpp`:

```cpp
#include <fstream>

namespace Menu {
    std::ofstream logFile;
    
    void StartLogging() {
        logFile.open("draw_call_log.txt");
        logFile << "Draw Call Log - " << __DATE__ << " " << __TIME__ << "\n";
        logFile << "========================================\n";
    }
    
    void LogDrawCall() {
        if (logFile.is_open()) {
            logFile << "Frame: " << frameCount 
                    << ", DC: " << iCurrentDrawCall 
                    << ", Filtered: " << bEnableDrawCallFilter
                    << ", Override: " << bEnableMaterialOverride
                    << "\n";
        }
    }
    
    void StopLogging() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
}
```

### 性能监控

添加 FPS 计数器:

```cpp
namespace Menu {
    float fps = 0.0f;
    int frameCount = 0;
    float elapsedTime = 0.0f;
    
    void UpdateFPS(float deltaTime) {
        frameCount++;
        elapsedTime += deltaTime;
        
        if (elapsedTime >= 1.0f) {
            fps = frameCount / elapsedTime;
            frameCount = 0;
            elapsedTime = 0.0f;
        }
    }
    
    void Render() {
        // ... 现有代码 ...
        
        ig::Text("FPS: %.1f", fps);
    }
}
```

---

## 常见问题

### Q1: 注入后控制台窗口立即关闭

**原因**:
- 后端设置不正确
- 目标程序不使用该图形 API
- DLL 架构不匹配

**解决方案**:
1. 验证图形 API:
   ```
   使用 Process Hacker 查看加载的 DLL
   ```
2. 检查架构匹配:
   ```
   32位程序 → 32位 DLL
   64位程序 → 64位 DLL
   ```
3. 尝试不同后端:
   ```
   依次尝试: DX11 → DX9 → OpenGL
   ```

### Q2: 按 INSERT 键没有反应

**原因**:
- ImGui 初始化失败
- 输入事件被拦截
- 钩子未正确安装

**解决方案**:
1. 检查控制台输出:
   ```
   查找错误信息
   ```
2. 按 HOME 键重新钩取:
   ```
   这会重新初始化图形钩子
   ```
3. 尝试其他键:
   ```
   修改代码使用不同的热键
   ```

### Q3: 绘制调用过滤不生效

**原因**:
- 目标程序使用不同的绘制函数
- 钩子未覆盖所有变体
- 多线程渲染

**解决方案**:
1. 检查是否钩取了正确的函数:
   ```
   DirectX 11:
   - Draw
   - DrawIndexed
   - DrawInstanced
   - DrawIndexedInstanced
   ```
2. 添加更多钩子:
   ```cpp
   // 在 hook_directx11.cpp 中添加
   MH_CreateHook(fnDrawInstanced, &hkDrawInstanced, &oDrawInstanced);
   ```

### Q4: 材质覆盖颜色不对

**原因**:
- 着色器未正确创建
- 常量缓冲区更新失败
- 混合模式问题

**解决方案**:
1. 检查控制台日志:
   ```
   查找着色器创建错误
   ```
2. 验证颜色值:
   ```
   确保 RGBA 都在 0.0-1.0 范围
   ```
3. 禁用 Alpha 混合:
   ```
   设置 Alpha = 1.0 (完全不透明)
   ```

### Q5: 游戏崩溃或冻结

**原因**:
- 与反作弊系统冲突
- 与其他覆盖层冲突
- 内存访问错误

**解决方案**:
1. 检查是否有反作弊:
   ```
   不要在线使用!
   ```
2. 关闭其他覆盖层:
   ```
   MSI Afterburner, RivaTuner, Discord 覆盖层
   ```
3. 使用 Debug 版本:
   ```
   更多错误信息，更容易调试
   ```

### Q6: 性能显著下降

**原因**:
- 过度的钩子开销
- 无优化的 Debug 版本
- 频繁的状态切换

**解决方案**:
1. 使用 Release 版本:
   ```
   性能差异可达 10 倍
   ```
2. 禁用不用的功能:
   ```
   只在需要时启用过滤器和覆盖
   ```
3. 优化代码:
   ```
   减少每帧的操作
   缓存常用数据
   ```

---

## 最佳实践

### 工作流程建议

**阶段 1: 准备和验证**
```
1. 确认目标程序的图形 API
2. 编译正确架构的 DLL
3. 准备注入器和测试环境
4. 在单机环境测试
```

**阶段 2: 基础探索**
```
1. 注入 DLL，验证控制台
2. 按 INSERT 测试菜单
3. 观察总绘制调用数
4. 尝试纯色覆盖功能
```

**阶段 3: 深入分析**
```
1. 启用绘制调用过滤器
2. 系统性扫描所有 DC
3. 记录重要的 DC 编号
4. 创建映射文档
```

**阶段 4: 应用和优化**
```
1. 根据需求调整功能
2. 修改代码添加自定义功能
3. 优化性能
4. 记录发现和结果
```

### 安全使用指南

⚠️ **重要警告**:

1. **仅用于单机游戏或自己的程序**
2. **不要在多人在线游戏中使用**
3. **遵守软件服务条款**
4. **自行承担使用风险**

✅ **合法使用场景**:
- 学习图形编程
- 逆向工程研究（教育目的）
- 调试自己的应用程序
- 游戏模组开发（单机）

❌ **禁止使用场景**:
- 在线游戏作弊
- 绕过反作弊系统
- 未经授权修改软件
- 商业用途（未获授权）

---

## 获取帮助

### 文档资源

- 主文档: `README_CN.md`
- 编译指南: `BUILD_GUIDE_CN.md`
- 使用指南: 本文档

### 社区支持

- **GitHub Issues**: 报告问题和请求功能
- **GitHub Discussions**: 交流使用经验
- **技术论坛**: 搜索类似问题的解决方案

### 提交问题模板

```markdown
**问题描述**
简要描述遇到的问题

**环境信息**
- OS: Windows 10/11
- 目标程序: [程序名称]
- 图形 API: DirectX 11
- DLL 版本: Release x64
- 注入器: Process Hacker

**重现步骤**
1. 启动目标程序
2. 注入 DLL
3. 按 INSERT 键
4. [具体操作]

**预期结果**
应该发生什么

**实际结果**
实际发生了什么

**截图**
如果适用，添加截图

**控制台日志**
粘贴控制台输出

**额外信息**
其他相关信息
```

---

## 总结

UniversalHookX 是一个强大的图形 API 钩子工具，适合:
- 🎓 学习现代图形编程
- 🔬 研究游戏渲染技术
- 🐛 调试图形应用程序
- 🛠️ 开发游戏模组

**关键要点**:
1. 选择正确的图形后端和架构
2. 使用可靠的注入器
3. 系统性地探索绘制调用
4. 记录重要发现
5. 负责任地使用

**下一步**:
- 查看实战案例获取灵感
- 尝试修改代码添加新功能
- 在自己的项目中应用学到的知识
- 与社区分享经验和发现

祝你使用愉快！🚀

---

**文档版本**: 1.0  
**最后更新**: 2025-10-03  
**作者**: UniversalHookX 社区
