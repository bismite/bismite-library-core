#ifndef __BI_CORE_GL_EXT_H__
#define __BI_CORE_GL_EXT_H__

#include "bi/logger.h"

//
// wrapper
//

#ifndef __EMSCRIPTEN__
static inline void glGenVertexArrays_APPLE_wrapper(GLsizei s, GLuint *p) { glGenVertexArraysAPPLE(s,p); }
static inline void glBindVertexArray_APPLE_wrapper(GLuint vao) {glBindVertexArrayAPPLE(vao); }
#endif

static inline void checkSupports()
{
    LOG("OpenGL Version: %s\n", glGetString(GL_VERSION));
    LOG("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    LOG("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // iphone7: 4096
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    LOG("GL_MAX_TEXTURE_SIZE: %d\n",max_texture_size);

    // not supported in ES2.0 and WebGL1.0
    // // iphone7: 0 (not supported!)
    // GLint max_array_texture_layers;
    // glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_array_texture_layers);
    // LOG("GL_MAX_ARRAY_TEXTURE_LAYERS: %d\n",max_array_texture_layers);

    // iphone7: 8
    GLint max_texture_image_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
    LOG("GL_MAX_TEXTURE_IMAGE_UNITS: %d\n",max_texture_image_units);

    // if(GLEW_ARB_vertex_program) LOG("ARB_vertex_program ok\n");

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_instanced_arrays.txt
    if(GLEW_ARB_instanced_arrays) LOG("ARB_instanced_arrays ok\n");

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_draw_instanced.txt
    if(GLEW_ARB_draw_instanced) LOG("ARB_draw_instanced ok\n");

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_vertex_array_object.txt
    if(GLEW_ARB_vertex_array_object) LOG("ARB_vertex_array_object ok\n");

#ifndef __EMSCRIPTEN__
    // https://www.khronos.org/registry/OpenGL/extensions/APPLE/APPLE_vertex_array_object.txt
    if(GLEW_APPLE_vertex_array_object){
        LOG("APPLE_vertex_array_object ok\n");
        glGenVertexArrays = glGenVertexArrays_APPLE_wrapper;
        glBindVertexArray = glBindVertexArray_APPLE_wrapper;
    }
#endif

    // https://www.khronos.org/registry/webgl/extensions/ANGLE_instanced_arrays/
    if(GLEW_ANGLE_instanced_arrays) LOG("ANGLE_instanced_arrays ok\n");

#ifdef __EMSCRIPTEN__
    // https://www.khronos.org/registry/OpenGL/extensions/OES/OES_vertex_array_object.txt
    if(GLEW_OES_vertex_array_object) LOG("OES_vertex_array_object ok\n");
#endif

    // https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_instanced_arrays.txt
    // XXX: for Apple OpenGL ES?
    // if(GLEW_EXT_instanced_arrays) LOG("EXT_instanced_arrays ok\n");

    // https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_draw_instanced.txt
    if(GLEW_EXT_draw_instanced) LOG("EXT_draw_instanced ok\n");

    LOG("checkSupports done.\n");
}

#endif
