#ifndef _FRAME_MAIN_H_
#define _FRAME_MAIN_H_

#include "frame_base.h"

class Frame_Main : public Frame_Base {
   public:
    Frame_Main();
    ~Frame_Main();
    int run();
    int init(epdgui_args_vector_t &args);
    void StatusBar(m5epd_update_mode_t mode);
    void AppName(m5epd_update_mode_t mode);

<<<<<<< HEAD
private:
    EPDGUI_Button *_key[9];
=======
   private:
    EPDGUI_Button *_key[8];
>>>>>>> 277b398b781ddcc3bfc2707e79240c6ab3be00df
    M5EPD_Canvas *_bar;
    M5EPD_Canvas *_names;
    uint32_t _next_update_time;
    uint32_t _time;
};

<<<<<<< HEAD
#endif //_FRAME_MAIN_H_
=======
#endif  //_FRAME_MAIN_H_
>>>>>>> 277b398b781ddcc3bfc2707e79240c6ab3be00df
