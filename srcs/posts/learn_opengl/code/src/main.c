#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define HEIGHT 600
#define WIDTH  800

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int s, int action, int m);

int main(int argc, char **argv)
{
        glfwInit();
        // version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        /* Create Window */
        GLFWwindow *window =
                glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
        if (window == NULL) {
                fprintf(stderr, "Failed to create window.\n");
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);

        /* load glad before writing OpenGL functions */
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwTerminate();
                return -1;
        }

        /* prepare render */
        glViewport(0, 0, WIDTH, HEIGHT);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);

        float vertices[] = {
                -0.5f, -0.5f, 0.0f,     // 左下角
                0.5f, -0.5f, 0.0f,     // 右下角
                0.0f,  0.5f, 0.0f      // y轴正上方
        };

        // VAO和VBO的ID都是非负整型
        GLuint VBO, VAO;
        // 生成一个VAO和一个VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        /* main loop */
        while(!glfwWindowShouldClose(window))
        {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                /* draw something */

                /* swap buffer */
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int s, int action, int m)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GL_TRUE);
        }
}
