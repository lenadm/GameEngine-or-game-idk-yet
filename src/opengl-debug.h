#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef RELEASE_BUILD
void runtimeAssert_(bool cond, const char* condString, const char* file, int line);

#define RUNTIME_ASSERT(cond) runtimeAssert_(cond, #cond, __FILE__, __LINE__)

GLenum glCheckError_(const char *file, int line);

#define GL_CHECK_ERROR() glCheckError_(__FILE__, __LINE__) 

void APIENTRY glDebugOutput(
    GLenum source, GLenum type, unsigned int id, 
    GLenum severity, GLsizei length, const char *message, 
    const void *userParam
);

#define ENABLE_DEBUG_OUTPUT() \
    do { \
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); \
        int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags); \
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) { \
            glEnable(GL_DEBUG_OUTPUT); \
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); \
            glDebugMessageCallback(glDebugOutput, nullptr); \
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); \
        } \
    } while(0)
#else
#define RUNTIME_ASSERT(cond) ((void)0)
#define glCheckError() ((void)0)
#define ENABLE_DEBUG_OUTPUT() ((void) 0)
#endif
