#include <bi/main_loop.h>
#include <bi/context.h>
#include <bi/node.h>
#include <bi/render.h>
#include <bi/util.h>
#include <bi/logger.h>

static bool node_event_handle(BiNode* n,BiContext* context,SDL_Event *e)
{
  bool swallow = false;
  switch(e->type){
    case SDL_MOUSEMOTION:
      if(n->_on_move_cursor!=NULL){
        int y = context->h - e->motion.y;
        swallow = n->_on_move_cursor(context,n,e->motion.x,y);
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      if(n->_on_click!=NULL){
        int y = context->h - e->button.y;
        swallow = n->_on_click(context,n,e->button.x,y,e->button.button,true);
      }
      break;
    case SDL_MOUSEBUTTONUP:
      if(n->_on_click!=NULL){
        int y = context->h - e->button.y;
        swallow = n->_on_click(context,n,e->button.x,y,e->button.button,false);
      }
      break;
    case SDL_FINGERMOTION:
      if(n->_on_move_finger!=NULL){
        float y = 1.0 - e->tfinger.y;
        swallow = n->_on_move_finger(context,n, e->tfinger.x, y, e->tfinger.fingerId);
      }
      break;
    case SDL_FINGERDOWN:
      if(n->_on_touch!=NULL){
        float y = 1.0 - e->tfinger.y;
        swallow = n->_on_touch(context,n,e->tfinger.x,y,e->tfinger.fingerId,true);
      }
      break;
    case SDL_FINGERUP:
      if(n->_on_touch!=NULL){
        float y = 1.0 - e->tfinger.y;
        swallow = n->_on_touch(context,n,e->tfinger.x,y,e->tfinger.fingerId,false);
      }
      break;
    case SDL_KEYDOWN:
      if(n->_on_keyinput!=NULL){
        swallow = n->_on_keyinput(context,n,e->key.keysym.scancode,e->key.keysym.sym,e->key.keysym.mod,true);
      }
      break;
    case SDL_KEYUP:
      if(n->_on_keyinput!=NULL){
        swallow = n->_on_keyinput(context,n,e->key.keysym.scancode,e->key.keysym.sym,e->key.keysym.mod,false);
      }
      break;
    case SDL_TEXTINPUT:
      if(n->_on_textinput!=NULL){
        swallow = n->_on_textinput(context,n,e->text.text);
      }
      break;
  }
  return swallow;
}

static void main_loop( void* arg )
{
    int64_t now = bi_get_now();
    BiContext *context = (BiContext*)arg;
    context->frame_start_at = now;

    bi_profile_record(&context->profile,now);

    context->profile.callback_planned_nodes_size = context->callback_planned_nodes_size;

    //
    // callback and event handling
    //

    // Global Timers
    bi_run_timers(context,&context->timers);

    const int PUMP_EVENT_MAX = 32;
    SDL_Event e[PUMP_EVENT_MAX];
    SDL_PumpEvents();
    int event_size = SDL_PeepEvents(e,PUMP_EVENT_MAX,SDL_GETEVENT,SDL_FIRSTEVENT,SDL_LASTEVENT);
    // callback
    for(int i=context->callback_planned_nodes_size-1;i>=0;i--){
      BiNode* n = context->callback_planned_nodes[i];
      if( n == NULL ){
        continue;
      }
      // on update
      if(n->_on_update) n->_on_update(context,n);
      // Timer
      bi_run_timers(context,&n->timers);
      // Event Handler
      if( n->_final_visibility ) {
        for(int i=0;i<event_size;i++) {
          if(e[i].type == 0) continue;
          bool swallow = node_event_handle(n,context,&e[i]);
          if(swallow) {
            e[i].type = 0; // XXX: swallow
          }
        }
      }
    }

    // failsafe
    for(int i=0;i<event_size;i++) {
      if(e[i].type==SDL_QUIT){
          LOG("Program quit after %i ticks", e[i].quit.timestamp);
          context->running = false;
      }
    }

    // reset queue
    context->callback_planned_nodes_size = 0;

    int64_t phase2 = bi_get_now();

    //
    // rendering
    //
    bi_render(context);

    //
    int64_t phase3 = bi_get_now();

    //
    context->profile.time_spent_on_callback = phase2 - now;
    context->profile.time_spent_on_rendering = phase3 - phase2;
}

void bi_start_run_loop(BiContext* context)
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, context, context->profile.target_fps, false);
#else
    // fallback
    if( context->profile.target_fps == 0 ) {
      context->profile.target_fps = 60;
    }
    SDL_GL_SetSwapInterval(0);
    while (context->running) {
        double start_at = bi_get_now();
        main_loop(context);
        double end_at = bi_get_now();
        int sleep = 1;
        if(context->profile.target_fps>0) { sleep = (1.0 / context->profile.target_fps) * 1000 - (end_at - start_at); }
        if(sleep<=0) { sleep = 1; }
        SDL_Delay(sleep);
    }
#endif
}
