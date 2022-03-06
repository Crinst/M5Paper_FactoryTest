#ifndef _GLOBAL_SETTING_H_
#define _GLOBAL_SETTING_H_

#include <M5EPD.h>
#include <nvs.h>


#define WALLPAPER_NUM 3

enum
{
    LANGUAGE_EN = 0,    // default, English
    LANGUAGE_JA, // Japanese
    LANGUAGE_ZH // Simplified Chinese
};

struct myIpQuerry{
    const char* ip;
    const char* country;
    const char* cc;
};

struct ipApiQuerry{
    const char* status;
    const char* country;
    const char* countryCode;
    const char* region;
    const char* regionName;
    const char* city;
    const char* zip;
    float lat;
    float lon;
    const char* timezone;
    const char* isp;
    const char* org;
    const char* as;
    const char* query;
};

struct openWeatherQuerry{
    float coord_lon;
    float coord_lat;

    int id;
    const char* main;
    const char* description;
    const char* icon;
    const char* base;

    float main_temp;
    float main_feels_like;
    float main_temp_min;
    float main_temp_max;

    int main_pressure;
    int main_humidity;
    int visibility;

    float wind_speed;
    int wind_deg;

    int clouds_all;
    long dt;
    
    int sys_type;
    int sys_id;
    const char* sys_country;

    long sys_sunrise;
    long sys_sunset;

    int timezone;
    long doc_id;
    const char* name;
    int cod;

};

uint8_t GetCurrentIpAddress();
uint8_t SetWifiConnection();
uint8_t TerminateWifiConnection();
uint8_t GetCurrentWeatherInformation();
String GetLatestWeatherInformationJson(void);
openWeatherQuerry GetLatestWeatherInformation(void);
uint8_t GetWeatherUnits(void);
uint8_t GetCurrentPositionBasedOnIpAddress();

void SetLanguage(uint8_t language);
uint8_t GetLanguage(void);
void SetWallpaper(uint16_t wallpaper_id);
uint16_t GetWallpaperID(void);
const uint8_t* GetWallpaper(void);
const char* GetWallpaperName(uint16_t wallpaper_id);

esp_err_t LoadSetting(void);
esp_err_t SaveSetting(void);
void SetWifi(String ssid, String password);
String GetWifiSSID(void);
String GetWifiPassword(void);
uint8_t isWiFiConfiged(void);
bool SyncNTPTime(void);
int8_t GetTimeZone(void);
void SetTimeZone(int8_t time_zone);

uint16_t GetTextSize();
void SetTextSize(uint16_t size);

const uint8_t* GetLoadingIMG_32x32(uint8_t id);
void LoadingAnime_32x32_Start(uint16_t x, uint16_t y);
void LoadingAnime_32x32_Stop();

uint8_t isTimeSynced(void);
void SetTimeSynced(uint8_t val);

void SetTTFLoaded(uint8_t val);
uint8_t isTTFLoaded(void);
void SetInitStatus(uint8_t idx, uint8_t val);
uint8_t GetInitStatus(uint8_t idx);

uint8_t GetGlobalLastFrame(void);
void setGlobalLastFrame(uint8_t lastFrame);

#endif //_GLOBAL_SETTING_H_
