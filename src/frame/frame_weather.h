#ifndef _FRAME_WEATHER_H_
#define _FRAME_WEATHER_H_

#include "frame_base.h"
#include "../epdgui/epdgui.h"
#include "resources/WeatherImageResource.h"


class Frame_Weather : public Frame_Base
{
public:
    Frame_Weather();
    ~Frame_Weather();
    int run();
    int init(epdgui_args_vector_t &args);
    void MainScreen(m5epd_update_mode_t mode);
private:
    uint32_t _next_update_time;
    uint32_t _time;
    M5EPD_Canvas *_weatherScreen;
    char buf[128];
    openWeatherQuerry weatherLocalReport;
};

#endif //_FRAME_WEATHER_H_
