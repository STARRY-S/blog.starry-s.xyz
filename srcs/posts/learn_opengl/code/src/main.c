#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char **argv)
{
        glfwInit();
        // OpenGL version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create Window
        GLFWwindow *window =
                glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
        if (window == NULL) {
                fprintf(stderr, "Failed to create window.\n");
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);

        // load glad before writing any OpenGL functions
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
                fprintf(stderr, "Failed to initialize GLAD\n");
                glfwTerminate();
                return -1;
        }

        /* prepare render */
        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        /* main loop */
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

        glfwTerminate();
        return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
}
