#ifndef __BI_CORE_CONTEXT_H__
#define __BI_CORE_CONTEXT_H__

#include <stdbool.h>
#include <bi/bi_gl.h>
#include <bi/bi_sdl.h>
#include <bi/shader.h>
#include <bi/profile.h>
#include <bi/node.h>
#include <bi/layer.h>

#define BI_CORE_MAJOR_VERSION 0
#define BI_CORE_MINOR_VERSION 13
#define BI_CORE_PATCHLEVEL 0

typedef struct _BiContext BiContext;

struct _BiContext {
    bool running;

    // geometry
    int w;
    int h;
    int camera_x;
    int camera_y;

    // background color
    uint8_t color[4];

    //
    int64_t program_start_at;

    // time of frame start(millisecond)
    int64_t frame_start_at;

    // fps, etc...
    BiProfile profile;

    // show debug info
    bool debug;

    // Layers
    BiLayerGroup layers;

    // rendering queue
    BiNode* rendering_nodes_queue[0xFFFF];
    int rendering_nodes_queue_size;

    // event and update callback
    BiNode* callback_planned_nodes[0xFFFF];
    int callback_planned_nodes_size;

    // global timers
    BiTimers timers;

    // default shader
    BiShader default_shader;

    //
    SDL_Window *window;

    // default texture
    GLuint default_texture;

    //
    void* userdata;
};

typedef void (*InitializeFunction)(BiContext*);

extern void bi_init_context(BiContext* context,int w, int h, int fps, bool highdpi, const char* title );
extern void bi_set_title(BiContext* context,const char* title);

// Convenient Layer functions
extern void bi_add_layer(BiContext* context, BiLayer* layer);
extern void bi_update_layer_order(BiContext* context);
extern void bi_remove_layer(BiContext* context, BiLayer* layer);

#endif
