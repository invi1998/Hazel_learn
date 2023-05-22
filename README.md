# Hazel_learn
## 整体结构

Hazel 项目的整体结构非常清晰，分为多个目录，每个目录都有自己的职责。以下是 Hazel 的主要目录：

- `src`：包含了 Hazel 引擎的所有源代码。
- `vendor`：包含了 Hazel 引擎依赖的第三方库，例如 GLFW、Glad 和 ImGui 等。
- `assets`：包含了 Hazel 引擎需要用到的各种资源文件，例如模型、纹理、音频等。

## 渲染器模块

渲染器模块是 Hazel 引擎最重要的模块之一，负责将场景中的物体渲染到屏幕上。在 Hazel 中，渲染器模块采用了现代OpenGL的方式来实现，并使用了Shader类封装着色器程序、VertexArray类封装顶点数组对象和VertexBuffer类封装缓冲对象等。以下是 Hazel 渲染器模块的主要组件：

- `GraphicsContext`：图形上下文，负责窗口的创建、OpenGL的初始化等。
- `Shader`：着色器程序类，负责生成并管理OpenGL着色器对象。
- `VertexArray`：顶点数组对象类，负责管理OpenGL顶点数组对象。
- `VertexBuffer`：缓冲对象类，负责管理OpenGL缓冲对象。
- `IndexBuffer`：索引缓冲对象类，负责管理OpenGL索引缓冲对象。
- `Renderer`：渲染器类，负责将场景中的物体渲染到屏幕上。

## 场景管理器模块

场景管理器模块是 Hazel 引擎负责管理物体、光源、相机、材质等的核心模块。这个模块负责将所有物体和光源放置到一个场景中，同时负责更新和绘制场景。以下是 Hazel 场景管理器模块的主要组件：

- `Entity`：实体类，代表场景中的物体，每个实体由多个组件组成。
- `Component`：组件类，代表实体的不同部分，例如 Transform（变换）组件、Mesh（网格）组件、Material（材质）组件等。
- `Scene`：场景类，负责管理场景中的所有实体和光源，并控制更新和渲染场景。
- `Camera`：相机类，控制视角和投影矩阵，以便正确地渲染场景。

## 实体组件系统

实体组件系统是 Hazel 引擎的另一个核心模块，它负责将实体拆分成多个组件，以便更好地管理和组合。实体组件系统是基于Entity-Component-System (ECS)设计思想而实现的。以下是 Hazel 实体组件系统的主要组件：

- `IComponent`：组件接口类，所有组件都要继承这个类。
- `ComponentArray`：组件数组类，负责运行时组件存储管理。
- `ComponentManager`：组件管理器类，负责管理所有组件类型和它们的数组。
- `EntityManager`：实体管理器类，负责所有实体的创建、销毁和查询。

## 输入模块

输入模块是 Hazel 引擎的一个模块，用于捕获输入事件（例如按键和鼠标点击）。这个模块使用GLFW库来处理输入事件。以下是 Hazel 输入模块的主要组件：

- `Input`：输入类，负责捕获和处理

  ## Core

还有一些模块和组件是 Hazel 引擎的重要组成部分，下面列出来：

- `Core`：核心模块，包含了一些基础数据结构、常用函数和单例类等。
- `Utils`：工具模块，包含了一些杂项工具函数和类，例如文件系统、性能计时器等。
- `Scripting`：脚本模块，可以使用Lua脚本编写逻辑代码。
- `ImGuiLayer`：ImGui图形用户界面层，负责在窗口中显示ImGui控件。
- `EntryPoint`：程序入口，负责初始化Hazel引擎，并启动游戏循环。

这些模块和组件共同构成了 Hazel 引擎的完整功能。虽然 Hazel 引擎还有许多其他的细节和实现，但以上列出的模块和组件是 Hazel 引擎最重要的核心组成部分。



## 项目管理

在 Hazel 项目中，使用了 Premake 工具来管理项目。Premake 是一个轻量级的跨平台构建系统，它采用Lua脚本语言作为配置文件，可以生成各种不同平台和编译器的项目文件，例如Visual Studio、Xcode、Makefile等。

以下是 Hazel 项目中 Premake 配置文件的主要部分：

- `premake5.lua`：Hazel 项目的主配置文件，定义了项目名称、版本号、目录结构等基本信息，并包含其他子配置文件。
- `premake/Modules`：Premake 模块目录，存放自定义的 Premake 模块，例如 OpenGL 和 GLFW 的模块。
- `premake/options.lua`：项目选项文件，定义了一些编译时选项，例如开启或关闭调试信息等。
- `premake/Hazel.lua`：Hazel 引擎项目配置文件，定义了编译 Hazel 引擎库的相关选项和依赖。
- `premake/Sandbox.lua`：Sandbox 示例项目配置文件，定义了编译 Sandbox 示例项目的相关选项和依赖。

通过 Premake，Hazel 项目可以很方便地进行跨平台编译和构建。在 Premake 文件中，可以指定所需的库和源文件，以及编译器选项和链接器选项等。同时，Premake 还提供了很多可重用的模块，例如 OpenGL 和 GLFW 的模块，可以让开发者更加便捷地集成和使用这些库。

总之，Premake 工具为 Hazel 项目提供了灵活、可扩展和跨平台的项目管理方式。通过 Premake，Hazel 项目可以非常方便地进行编译、构建和发布。
