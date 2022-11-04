#include "frame_weather.h"

Frame_Weather::Frame_Weather(void)
{
    _frame_name = "Frame_Weather";
    _frame_id = 8;

    _weatherScreen = new M5EPD_Canvas(&M5.EPD);
    _weatherScreen->createCanvas(540, 875);
    _weatherScreen->setTextSize(26);

    //M5EPD_Canvas canvas_temp(&M5.EPD);
    //canvas_temp.createRender(36);
    uint8_t language = GetLanguage();

    //setGlobalLastFrame(8);
    //Serial.print("Last frame value: ");
    //Serial.print(GetGlobalLastFrame);


    if(language == LANGUAGE_JA)
    {
        exitbtn("ホーム");
        _canvas_title->drawString("コントロールパネル - WS", 270, 34);
    }
    else if(language == LANGUAGE_ZH)
    {
        exitbtn("主页");
        _canvas_title->drawString("控制面板 - WS", 270, 34);
    }
    else
    {
        exitbtn("Home");
        _canvas_title->drawString("Weather station", 270, 34);
    }




    //_weatherScreen->drawFastHLine(0, 43, 540, 15);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

Frame_Weather::~Frame_Weather(void)
{
    delete _key_exit;
}

void Frame_Weather::MainScreen(m5epd_update_mode_t mode)
{
    //StatusBar(UPDATE_MODE_GL16);
    if((millis() - _time) < _next_update_time)
    {
        return;
    }

    uint64_t lastTime = millis();
    uint64_t currTime = millis();
    

    Serial.println("Main screen - printing new data");

    // push all data to screen
    //_weatherScreen->pushCanvas(0, 100, mode);

    if(GetCurrentWeatherInformation())
    {
        weatherLocalReport = GetLatestWeatherInformation();

        _weatherScreen->fillCanvas(0);

        _weatherScreen->pushImage(20, 150 - 96 - 96/4, 96, 96, WeatherImageResource_compass_96x96);
        sprintf(buf, "Latitude: %.5f", weatherLocalReport.coord_lat);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 - 96 - 96/4 + 8);
        sprintf(buf, "Longtitude: %.5f", weatherLocalReport.coord_lon);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 - 96 - 96/4 + 8 + 28);
        sprintf(buf, "City, state: %s, %s", weatherLocalReport.name, weatherLocalReport.sys_country);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 - 96 - 96/4 + 8 + 28 + 28);

        //_weatherScreen->drawString("Test print", 0, 0);
        uint8_t weatherUnits = GetWeatherUnits();
        Serial.println(weatherUnits);

        _weatherScreen->pushImage(20, 150, 96, 96, WeatherImageResource_temperature_96x96);

        /*
        if (weatherUnits == 0)
        {
            _weatherScreen->pushImage(20, 150, 96, 96, WeatherImageResource_celsius_96x96);
        }
        else if (weatherUnits == 1)
        {
            _weatherScreen->pushImage(20, 150, 96, 96, WeatherImageResource_fahrenheit_96x96);
        }
        else
        {
             _weatherScreen->pushImage(20, 150, 96, 96, WeatherImageResource_weather_app_96x96);
        }
        */
        
        sprintf(buf, "Temperature: %.2f ℃", weatherLocalReport.main_temp);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 8);
        sprintf(buf, "Feels like: %.2f ℃", weatherLocalReport.main_feels_like);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 8 + 28);
        sprintf(buf, "Min: %.2f ℃, Max: %.2f ℃", weatherLocalReport.main_temp_min, weatherLocalReport.main_temp_max);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 8 + 28 + 28);

        _weatherScreen->pushImage(20, 150 + 96 + 96/4, 96, 96, WeatherImageResource_barometer_96x96);
        sprintf(buf, "Pressure: %d hPa", weatherLocalReport.main_pressure);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 96 + 96/4 + 96/2-14);

        _weatherScreen->pushImage(20, 150 + 96 + 96/4 + 96 + 96/4, 96, 96, WeatherImageResource_humidity_96x96);
        sprintf(buf, "Humidity: %d %%", weatherLocalReport.main_humidity);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 96 + 96/4 + 96 + 96/4 + 96/2-14);

        _weatherScreen->pushImage(20, 150 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4, 96, 96, WeatherImageResource_cloud_96x96);
        sprintf(buf, "Wind speed: %.2f m/s", weatherLocalReport.wind_speed);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96/4);
        sprintf(buf, "Wind direction: %d deg", weatherLocalReport.wind_deg);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96/4 + 28);

        // push all data to screen
        //_weatherScreen->pushCanvas(0, 100, mode);

        // SHT30
        M5.SHT30.UpdateData();
        float ctemp = M5.SHT30.GetTemperature();
        float chumi = M5.SHT30.GetRelHumidity();
        
        _weatherScreen->pushImage(20, 150 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4, 96, 96, WeatherImageResource_weather_app_96x96);
        sprintf(buf, "Int. temperature: %.2f ℃", ctemp);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96/4);
        sprintf(buf, "Int. humidity: %.2f %%", chumi);
        _weatherScreen->drawString(buf, 20 + 96 + 10, 150 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96 + 96/4 + 96/4 + 28);
        // push all data to screen
        //_weatherScreen->pushCanvas(0, 100, mode);

    }
    else
    {
        _weatherScreen->fillCanvas(0);
        _weatherScreen->drawString("ERROR no data received", 0, 0);
    }

    // Time
    rtc_time_t time_struct;
    rtc_date_t date_struct;
    M5.RTC.getTime(&time_struct);
    M5.RTC.getDate(&date_struct);

    sprintf(buf, "Last update");
    _weatherScreen->drawString(buf, 20, 800);
    sprintf(buf, "%04d - %02d - %02d --- %2d:%02d", date_struct.year, date_struct.mon, date_struct.day, time_struct.hour, time_struct.min);
    _weatherScreen->drawString(buf, 20, 800 + 28);

    // push all data to screen
    _weatherScreen->pushCanvas(0, 100, mode);

    currTime = millis();

    Serial.printf("Time to refresh page %d\n", currTime-lastTime);

    lastTime = currTime;
    _time = millis();

    M5.shutdown(60);
    
    _next_update_time = 60000;

}

int Frame_Weather::run()
{
    MainScreen(UPDATE_MODE_GLR16);
    return 1;
}


int Frame_Weather::init(epdgui_args_vector_t &args)
{
    _is_run = 1;
    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);
    EPDGUI_AddObject(_key_exit);

    
    //_weatherScreen->drawString("Printing new data", 0, 0);
    

    _time = 0;
    // update data every minute
    _next_update_time = 0;
    // get current weather information
    //GetCurrentWeatherInformation();

    MainScreen(UPDATE_MODE_GC16);
    //MainScreen(UPDATE_MODE_NONE);

    return 3;
}
