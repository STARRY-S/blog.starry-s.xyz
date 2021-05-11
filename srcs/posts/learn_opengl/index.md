---
title: Learn OpenGL
created: 2021-05-10T21:33:10+08:00
updated: 2021-05-11T13:59:16+08:00
layout: post
tags:
- 学习资料
- OpenGL
categories:
- 学习资料
- OpenGL
---

其实咱很久很久以前就开始看[LearnOpenGL CN](https://learnopengl-cn.github.io/)这个网站了......

<!--more-->

-----

# 前言

首先需要明白什么是核心模式，什么是立即渲染模式，立即渲染模式的代码中都包含`glBegin()`和`glEnd()`，绘图的部分都是在这两个函数之间，比如画一个点就是`glVertex2f(x, y)`，这种方式画图确实很简单，用户只要提供坐标和颜色就好了，不用知道GPU干了什么，而缺点是性能低，有很多限制，所以新版本的OpenGL为了给开发者提供更多的可操作空间而逐渐废弃了立即渲染改用核心模式。

核心模式提供了很多强大的功能，可以管线编程，代码量增加了很多，所以对初学者不太友好。

LearnOpenGL CN上面的代码基于C++，而OpenGL是用C编写的，教程上的代码除了矩阵运算部分可以使用C++的运算符重载而使代码编写起来变得更简单一些外，其他部分基本没啥区别，而且教程为了便于教学使用的也是面向过程式编程，没有将各个功能封装到一个类里面，所以咱这篇教程用C编写。

要记住C就是C，C++就是C++，C不是C++的子集，只能说C++兼容部分C的代码，不要把C和C++混用，不要因为C++支持面向对象使得一些功能看起来简单很易于使用就轻易迈入C++的坑。

# 准备工作

有关OpenGL的介绍以及安装GLFW、GLAD以及编译所需的CMake的部分可以直接看[教程](https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/)，咱尽可能把教程上没有提到的或者是刚开始学OpenGL很难理解的部分记录下来。

大体上就是安装`glfw`，Arch Linux使用包管理器`sudo pacman -S glfw-x11` (如果你使用的是wayland，那么安装`glfw-wayland`)，然后把下载的`glad/glad.h`复制到`/usr/include`下，把`glad.c`复制到工程文件夹的代码目录下。

为使用CMake生成Makefile，编写`CMakeLists.txt`:

```
cmake_minimum_required(VERSION 3.0)

project(
    "Learn OpenGL"
    LANGUAGES C
    VERSION 0.1.0
)

set(C_FLAGS "-Wall -lm -ldl -std=c11")
set(CMAKE_C_FLAGS ${C_FLAGS})
set(OpenGL_GL_PREFERENCE LEGACY)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} src)

aux_source_directory("src" GLSRC)
add_executable(main ${GLSRC})
target_link_libraries(main ${OPENGL_gl_LIBRARY} glfw)
```

完成上述步骤后，确保工程文件夹的结构如下：

``` text
├── build/
├── CMakeLists.txt
└── src/
    ├── glad.c
    └── main.c
```

# 创建窗口

编辑`main.c`，加入所需的头文件

``` C
// glad.h要加在glfw3.h之前

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
```

在main函数中初始化OpenGL并创建窗口

``` C
int main(int argc, char **argv)
{
        glfwInit();
        /* 使用OpenGL版本为3.3 */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        /* 使用核心模式 */
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        /* 创建窗口 */
        GLFWwindow *window =
                    glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
        if (window == NULL) {
                fprintf(stderr, "Failed to create window.\n");
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);

        /* 初始化glad */
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwTerminate();
                return -1;
        }

        /* prepare render */

        /* main loop */

        glfwTerminate();
        return 0;
}
```

创建窗口的代码比较容易理解，到这里可以尝试编译一下代码检查有没有遇到什么问题，如果编译失败了可以尝试检查GLFW是否安装正确，代码哪里出现了什么问题。

编译代码并运行生成的程序：

```
cd build
cmake .. && make -j8
./main
```

如果一切正常的话，可以看到窗口一闪而过就消失了，因为到目前为止我们只创建了一个窗口，创建完成后就结束了程序，所以窗口会瞬间消失。

-----

# 准备绘图

在绘制形状之前，我们需要一些准备工作比如设定视口、回调函数，创建顶点缓冲区等操作，然后在大循环中进行渲染绘制。

## 视口

首先要告诉OpenGL咱的窗口尺寸是多少，以便OpenGL根据窗口大小显示数据和坐标。

在`/* prepare render */`处添加代码：

``` C
glViewport(0, 0, 800, 600);
```

首先需要知道你传给OpenGL的坐标都是`-1.0`到`1.0`之间的数，OpenGL再将其转换成屏幕上的像素点坐标，这个过程由GPU运算所得，所以你得告诉OpenGL你的屏幕尺寸，`glViewport`的前两个参数为`0, 0`指的是窗口左下角的位置，这个值咱目前不需要修改。

## 回调函数

我们需要一个窗口被更改的回调函数，这样当窗口尺寸发生变化时，程序可以调用这个函数调整视口。

我们还需要一个[按键回调函数](https://www.glfw.org/docs/3.3/input_guide.html#input_key)，当用户按下键盘上的某个按键后会调用这个回调函数处理按键操作。

编写函数`framebuffer_size_callback`用来处理窗口尺寸更改:

``` C
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}
```

这样当你拖拽窗口进行缩放时，窗口里的图像也会跟着窗口尺寸改变而进行缩放。

编写`key_callback`函数处理用户按键操作：

``` C
void key_callback(GLFWwindow *window, int key, int s, int action, int m)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }
}
```

当用户按下`ESC`或`Q`键时，可以结束OpenGL窗口的运行。

然后我们需要注册这两个回调函数，在`/* prepare render */`处添加下面的代码:

``` C
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
glfwSetKeyCallback(window, key_callback);
```

## 大循环

在大循环中，每循环一次代表绘制一帧画面。这里利用了双缓冲将绘制的图形放到缓存中，然后将缓存中的图形刷新到显示器上。(因为OpenGL绘图操作是逐行绘制的，如果不先将图形存到缓存中而直接显示在显示器上的话，在画面快速变动时你很可能会遇到画面撕裂，垂直不同步)

大循环部分的代码长这个样子:

``` C
while(!glfwWindowShouldClose(window))
{
        /* 清空背景色为深灰色 */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* draw something */

        /* swap buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
}
```

每次循环的开始需要使用`glClear`清空屏幕，这样就不会看到上一次渲染的结果。

这里我们使用`glClearColor`设定清空屏幕的颜色为接近纯黑色的灰色。

大循环每循环一次，OpenGL就渲染了一帧画面并显示在显示器上，所以你可以计算一秒钟循环的次数而估算游戏的帧率。

> 因为我们使用了双缓冲，所以这时游戏的帧数被限制为等于显示的刷新帧数，
> 比如显示器刷新率为144帧，游戏的帧率就被锁为144，如果想解除这个限制需要改为单缓冲。

------

到此为止，我们已经创建了一个游戏引擎，但是这个引擎还什么都没有做。

尝试编译代码，可以看到一个黑色的窗口，按`ESC`或`Q`即可结束运行。

<!-- ![Window](images/create_window.png "创建窗口") -->
<div style="text-align:center">
    <img src="images/create_window.png" alt="Window" title="创建窗口">
</div>

如果你遇到什么问题，可以[对照一下代码](learn-code-1/main.c)是否有问题。

-----

# 画一个三角形

画三角形的过程很简单，首先要确定三角形的三个顶点坐标，然后告诉GPU这三个点坐标就可以了 :)

> 如果你看不懂顶点着色器、几何着色器、片段着色器、光栅化这些难懂的知识点，那么你不必急于弄懂这个过程，等把图形绘制出来后再回来看这部分的内容。

## 标准化设备坐标

因为OpoenGL的坐标为都是-1.0f到1.0f之间的数（我们先不考虑视口变换这些复杂的情形）

x, y, z的坐标为-1.0f到1.0f的坐标称作标准化设备坐标，坐标原点在窗口的正中央且在窗口的表面上，往右为x正方向，往上为y正方向，往屏幕里面为z正方向（这里是左手系）。

有关标准化设备坐标的介绍请看[这里](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/#_2)。

所以我们先声明三角形的顶点到一个一维数组里面（用一维数组是因为顶点坐标值在内存的分布都是连续的，这样方便给GPU传坐标）。

``` C
float vertices[] = {
    -0.5f, -0.5f, 0.0f,     // 左下角
     0.5f, -0.5f, 0.0f,     // 右下角
     0.0f,  0.5f, 0.0f      // y轴正上方
};

```

## 顶点缓冲

首先我们需要创建一个顶点缓冲对象，用来存我们的顶点信息，这个对象叫“Vertex Buffer Object”（VBO），之后创建一个顶点数组对象“Vertex Array Object”（VAO），用来存我们创建的VBO。

为便于理解，你可以把VBO当作为开辟了一块GPU上的显存(Buffer)，用来存顶点信息，然后VAO是一个包含多个VBO的数组(Array)。

绘图时可以把VBO中存的大量顶点信息发送给GPU，因为用CPU给显卡发顶点坐标的速度慢而且没办法一次发送大量的顶点坐标，所以我们把顶点坐标存到显存中，绘图时直接访问显存即可。

生成一个VBO和一个VAO的代码为：

``` C
// VAO和VBO的ID都是非负整型
GLuint VBO, VAO;

// 生成一个VAO和一个VBO
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
```

我们刚刚创建好了一个缓存对象，我们现在需要告诉这个缓存的数据空间大小以及数据（顶点坐标）。

首先我们绑定刚创建的VBO

``` C
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

绑定后才能给它传递信息，不然GL不知道你想给哪个Buffer上传递信息。

``` C
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

 - 第一个参数是目标缓冲的类型，我们刚刚把VBO绑定到`GL_ARRAY_BUFFER`上了

 - 第二个参数是顶点信息所占的空间大小，单位是字节。咱们这个三角形一共3个顶点，每个顶点有3个float值，

   所以大小是`3 * 3 * sizeof(float)`，不过我们可以直接用`sizeof(vertices)`知道整个数组的大小。

 - 第三个参数是数组的地址

 - 第四个参数告诉GL我们的顶点数据几乎不会改变，所以是`GL_STATIC_DRAW`。

   如果数据会被改变很多次，则为`GL_DYNAMIC_DRAW`

   如果数据每次绘制都会更改，则改为`GL_STREAM_DRAW`

使用完VBO后，一个良好的习惯是解绑

``` C
glBindBuffer(GL_ARRAY_BUFFER, 0);
```

## 着色器

我们需要写两个着色器程序，分别为顶点着色器和片段着色器。

### 顶点着色器

因为OpenGL使用的是标准化设备坐标而不是屏幕上的像素点为坐标，所以GPU需要把标准化设备坐标转换为屏幕上的像素点，这个过程由顶点着色器实现。

而顶点着色器不知道咱们三角形的每个顶点的坐标是多少，所以咱们得往着色器中传我们刚才创建的VBO里面的坐标。

着色器程序使用GLSL编写，其代码和C很像

在代码文件夹中新建一个`vertex.glsl`，编写以下代码：

``` glsl
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

 - 第一行代表我们使用OpenGL 3.3 核心模式

 - 第二行声明了一个类型为`vec3`（三维坐标）的变量，该变量的位置为`0`，需要人为的通过这个位置给他传值

 - main函数中`gl_Position`代表这个顶点的位置坐标，我们把CPU传给顶点着色器的`vec3`转换为`vec4`。
