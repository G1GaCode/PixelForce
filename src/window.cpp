#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../headers/window.hpp"

Window::Window(int width, int height)
{
    this->width = width;
    this->height = height;

    // GLFW base settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    this->window = glfwCreateWindow(width, height, "PixelForce", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GlFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, close_window_esc);

    // GLEW base settings
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        this->~Window();
    }

    glfwGetFramebufferSize(window, &this->height, &this->width);
    glViewport(0, 0, width, height);
}

Window::~Window()
{
    glfwDestroyWindow(this->window);
}

void Window::Clear(GLclampf red, GLclampf green, GLclampf blue)
{
    // fills the window with one color
    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void close_window_esc(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}