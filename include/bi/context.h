#ifndef __BI_CORE_CONTEXT_H__
#define __BI_CORE_CONTEXT_H__

#include <stdbool.h>
#include <GL/glew.h>
#include <bi/bi_sdl.h>
#include <bi/shader.h>
#include <bi/profile.h>
#include <bi/timer.h>

#define BI_CORE_MAJOR_VERSION 0
#define BI_CORE_MINOR_VERSION 4
#define BI_CORE_PATCHLEVEL 0

struct _BiContext;
typedef struct _BiContext BiContext;

struct _BiNode;
typedef struct _BiNode BiNode;

struct _BiLayer;
typedef struct _BiLayer BiLayer;

struct _BiContext {
    bool running;

    // geometry
    int w;
    int h;
    int camera_x;
    int camera_y;

    // background color
    uint8_t color[4];

    // fps
    BiProfile profile;

    // show debug info
    bool debug;

    // Layers
    BiLayer* layers[0xFF];
    int layers_size;

    // rendering queue
    BiNode* rendering_nodes_queue[0xFFFF];
    int rendering_nodes_queue_size;

    // event and update callback
    BiNode* callback_planned_nodes[0xFFFF];
    int callback_planned_nodes_size;

    // timer callback
    BiTimer **timers;
    int timers_size;

    //
    BiShader shader;

    //
    SDL_Window *window;

    //
    void* userdata;
};

typedef void (*InitializeFunction)(BiContext*);

extern void bi_init_context(BiContext* context,int w, int h, int fps, bool highdpi, const char* title );
extern void bi_set_title(BiContext* context,const char* title);

// layer
extern void bi_add_layer(BiContext* context, BiLayer* layer);
extern void bi_remove_layer(BiContext* context, BiLayer* layer);

#endif
