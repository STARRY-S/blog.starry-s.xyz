---
title: Learn OpenGL
created: 2021-05-10T21:33:10+08:00
# updated: 2021-05-01T19:27:32+08:00
layout: post
tags:
- 学习资料
- OpenGL
categories:
- 学习资料
- OpenGL
---

其实咱很久很久以前就开始看[LearnOpenGL CN](https://learnopengl-cn.github.io/)
这个网站了......

<!--more-->

<div class="alert-blue">
本篇仍在编写中...
</div>

-----

# 前言

咱是从高中就开始看LearnOpenGL CN这网站了，但是那个时候咱只学了一点C，所以当时咱遇到了C++的
命名空间`std::`时就一头雾水，更别说啥渲染图形了。

然后大一时也尝试过学OpenGL这玩意，当时不懂啥叫状态机啥叫顶点/片段着色器，被glsl绕得蒙头转向，
心想着OpenGL怎么这么复杂，照别人写好的代码抄一遍确实能画个五颜六色的三角形出来，但是当时不懂
为什么这样子向顶点着色器传顶点坐标，到底啥叫着色器，它究竟干了什么， GLSL的变量到底是怎么用的。

<!-- 所以终于上了图形学的课后这些东西才一点点弄明白，然而图形学老师用的是旧版GLUT的立即渲染模式，
还说写OpenGL很简单一点也不难，实际上老师的课件也只教了我们怎么用OpenGL画点/线/三角形。
用GLUT在教学上确实够用了而且我上的不是OpenGL课程，但是....

“只要告诉两点坐标就能画个很丑的线出来了，这一点也不难”  :(

所以咱啃了好多遍这个教程才算是一点点理解了OpenGL是如何绘图这个过程的吧

> ~~老师：“不要因为上过我的这门课就和别和别人说你们学过计算机图形学”~~ -->

-----

首先需要明白什么是核心渲染模式，什么是立即渲染模式，立即渲染模式的代码中都包含`glBegin()`
和`glEnd()`，绘图的部分都是在这两个函数之间，比如画一个点就是`glVertex2f(x, y)`，这种方式
画图确实很简单，和canvas，java的swing一样，用户只要提供坐标和颜色就好了，
不用知道GPU干了什么，而缺点是性能低，有很多限制，所以新版本的OpenGL为了给开发者提供更多的
可操作空间而废弃立即渲染改用核心模式。

核心模式提供了很多强大的功能，可以管线编程，代码量增加了很多，所以对初学者不太友好。

LearnOpenGL CN上面的代码基于C++，而OpenGL是用C编写的，教程上的代码除了矩阵运算部分可以
使用C++的运算符重载而使代码编写起来变得更简单一些外，其他部分基本没啥区别，而且教程为了便于教学
使用的也是面向过程式编程，没有将各个功能封装到一个类里面，所以咱这篇教程用C编写。

要记住C就是C，C++就是C++，C不是C++的子集，只能说C++兼容部分C的代码，不要把C和C++混用，
不要因为C++支持面向对象使得一些功能看起来简单很易于使用就轻易迈入C++的坑，毕竟还有那么多支持
面向对象的语言可以选。

# 准备工作

有关OpenGL的介绍以及安装GLFW、GLAD以及编译所需的CMake的部分可以直接看
[教程](https://learnopengl-cn.github.io/01%20Getting%20started/02%20Creating%20a%20window/)，
咱尽可能把教程上没有提到的或者是刚开始学OpenGL很难理解的部分记录下来。

大体上就是安装`glfw`，Arch Linux使用包管理器`sudo pacman -S glfw-x11`
(如果你使用的是wayland，那么安装`glfw-wayland`)，然后把下载的`glad/glad.h`复制到
`/usr/include`下，把`glad.c`复制到工程文件夹的代码目录下。

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

完成上述步骤后，确保工程文件夹的结构大体如下：

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

创建窗口的代码比较容易理解，到这里可以尝试编译一下代码检查有没有遇到什么问题，如果编译失败了
可以尝试检查OpenGL是否安装正确，代码哪里出现了什么问题。

编译代码并运行生成的程序：

```
cd build
cmake .. && make -j8
./main
```

如果一切正常的话，可以看到窗口一闪而过就消失了，因为到目前为止我们只创建了一个窗口，创建完成后
就结束了程序，所以窗口会瞬间消失。

-----

# 准备绘图

在绘制形状之前，我们需要一些准备工作比如设定视口、回调函数，创建顶点缓冲区等操作，
然后在大循环中进行渲染绘制。

## 视口

首先要告诉OpenGL咱的窗口尺寸是多少，以便OpenGL根据窗口大小显示数据和坐标。

在`/* prepare render */`处添加代码：
``` C
glViewport(0, 0, 800, 600);
```

首先需要知道你传给OpenGL的坐标都是`-1.0`到`1.0`之间的数，OpenGL再将其转换成屏幕上的像素点
坐标，这个过程由GPU运算所得，所以你得告诉OpenGL你的屏幕尺寸，`glViewport`的前两个参数为
`0, 0`指的是窗口左下角的位置，这个值咱目前不需要修改。

## 回调函数

我们需要一个窗口被更改的回调函数，这样当窗口尺寸发生变化时，程序可以调用这个函数调整视口。

编写函数`framebuffer_size_callback`用来处理窗口尺寸更改:

``` C
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}
```

这样当你拖拽窗口进行缩放时，窗口里的图像也会跟着窗口尺寸改变而进行缩放。

然后我们需要注册这个回调函数，在`/* prepare render */`处添加下面的代码:

``` C
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
```

## 处理按键操作

编写`processInput`函数处理用户按键操作：

``` C
void processInput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
}
```

当用户按下`ESC`或`Q`键时，可以结束OpenGL窗口的运行。

## 大循环

在大循环中我们进行游戏的每一帧的绘制，利用双缓冲将绘制的图形放到缓存中，然后将缓存中的图形刷新到
显示器上，(因为OpenGL绘图操作是逐行绘制的，如果不先将图形存到显存中而直接显示在显示器上的话，
在画面变动时你很可能会看到撕裂，闪烁，垂直不同步这类的问题)。

在大循环中我们要处理按键操作(例如游戏中的`W S A D`代表前后左右移动)，然后在大循环中绘制图形，
大循环部分的代码长这个样子:

``` C
while(!glfwWindowShouldClose(window))
{
        /* process input */
        processInput(window);

        /* draw something */

        /* swap buffer */
        glfwSwapBuffers(window);
        /* poll events */
        glfwPollEvents();
}
```

大循环每循环一次，OpenGL就渲染了一帧画面并显示在显示器上，所以你可以计算一秒钟循环的次数而求得
游戏的帧率。

> 因为我们使用了双缓冲，所以这时游戏的帧数被限制为等于显示的刷新帧数，
> 比如显示器刷新率为144帧，游戏的帧率就被锁为144，如果想解除这个限制需要改为单缓冲。

------

到此为止，我们已经创建了一个游戏引擎，但是这个引擎还什么都没有做。

尝试编译代码，可以看到一个黑色的窗口，按`ESC`或`Q`即可结束运行。

<!-- ![Window](images/create_window.png "创建窗口") -->
<div style="text-align:center">
    <img src="images/create_window.png" alt="Window" title="创建窗口">
</div>

-----

# 画三角形

首先我们需要创建一个顶点缓冲对象，用来存我们的顶点信息，这个对象叫“Vertex Buffer Object”
（VBO），之后创建一个顶点数组对象“Vertex Array Object”（VAO），用来存我们创建的VBO。

> 尚未完工，不定期更新
> 2021.05.10 21:33:02
