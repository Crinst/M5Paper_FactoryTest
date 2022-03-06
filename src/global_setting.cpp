#include "global_setting.h"
#include "./resources/ImageResource.h"
#include "esp32-hal-log.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include "HTTPClient.h"

#define DEFAULT_WALLPAPER 2
SemaphoreHandle_t _xSemaphore_LoadingAnime = NULL;
static uint8_t _loading_anime_eixt_flag = false;
esp_err_t __espret__;
#define NVS_CHECK(x)              \
    __espret__ = x;           \
    if (__espret__ != ESP_OK) \
    {                         \
        nvs_close(nvs_arg);   \
        log_e("Check Err");   \
        return __espret__;    \
    }

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFOzCCBCOgAwIBAgISBIdAiJqrqU+wo19ncgXI3QAXMA0GCSqGSIb3DQEBCwUA\n" \
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
"EwJSMzAeFw0yMjAyMjYwNDAxMjFaFw0yMjA1MjcwNDAxMjBaMBcxFTATBgNVBAMT\n" \
"DGlwLnNlZWlwLm9yZzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMaP\n" \
"4X0luSiTX0+gd+t6QymBilqaUFN/S7FqDoziTpi3eNJhMFG9BrggYN3rvDMeN9IA\n" \
"q7ATZlaoeZjGwjm5FXO6U5Dfczsxs4PKyRZg9rWLNnIcBMmdSGHFgQjuVL0jlRTF\n" \
"NSYYi53IqXlK9lHQTsNcgBTLkte4YwzxhA6KRl4NKD7ITFfpvDEorfQcmZRhjunL\n" \
"8iK1kMilbTgfiblTgD9oWwUm/f5t+EnQt0Zt7JwCu15BFw4wYjrDLD14I5ly67zv\n" \
"jAu1PzuuycaOTIY+xs+SHii2WAYXw8IC/YoVBcr4HQiaF39Zw9u7TssWHvSUNDC9\n" \
"+K5vDqS4PRciE2b3yB8CAwEAAaOCAmQwggJgMA4GA1UdDwEB/wQEAwIFoDAdBgNV\n" \
"HSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4E\n" \
"FgQUFP4wU/0AJ4Ewm1g9UIHJaio6I4EwHwYDVR0jBBgwFoAUFC6zF7dYVsuuUAlA\n" \
"5h+vnYsUwsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8vcjMu\n" \
"by5sZW5jci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9yZy8w\n" \
"NQYDVR0RBC4wLIIMaXAuc2VlaXAub3Jngg1pcDQuc2VlaXAub3Jngg1pcDYuc2Vl\n" \
"aXAub3JnMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcGCysGAQQBgt8TAQEBMCgwJgYI\n" \
"KwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5cHQub3JnMIIBAwYKKwYBBAHW\n" \
"eQIEAgSB9ASB8QDvAHYARqVV63X6kSAwtaKJafTzfREsQXS+/Um4havy/HD+bUcA\n" \
"AAF/NGekPAAABAMARzBFAiEAvfoTUhiwcIgEMXgAZIi3zxnZn8YgPVhLVQfmpfCL\n" \
"YB0CIBtZPLcNErCpQHMjCqsewmQT9PTflTKuaVo9Z3AvRvFoAHUAQcjKsd8iRkoQ\n" \
"xqE6CUKHXk4xixsD6+tLx2jwkGKWBvYAAAF/NGemKgAABAMARjBEAiBzV5pxt/tx\n" \
"Kwf2g+U//MmvPbg0CScIIwyImhkx42LOVgIgbO8rpsdRPAfmWLWHoRgWIb1QO8bZ\n" \
"saOLsnXQiBf+c7AwDQYJKoZIhvcNAQELBQADggEBADH+4L5oLEx7gAlZ/DQLazDi\n" \
"f3jSs1R3ZogRlMTcdn6gsaXoC3pmE9L/dLX9NdoSXHWXZfek7OhhT09FsldmDF0R\n" \
"nYFCMWr1bvMUomVrSnBCo4N7Ql1RTEkU3Xh6QH2pSRWTXfUnbCGR9Y4N1dCwTFGW\n" \
"87etn3V3bbmZGa1divutirxy/1aaTHP+M06A/Ax8u2HuJtYdfwd6f1hmwOamAPPZ\n" \
"wiy7sLmHwVDeVDybukBXkK8SFbMbIniI/JNVN3SaxxgrKg5NX4PgcQXRYdJMGHtW\n" \
"iJnXvzQZyovBcWe73rwoUJmzb4mElT6dzXvW2no8l4JzQ6W2ftrmyr3Wh2T0RNg=\n" \
"-----END CERTIFICATE-----\n";

const uint8_t *wallpapers[] = {
    ImageResource_wallpaper_m5stack_540x960,
    ImageResource_wallpaper_engine_540x960,
    ImageResource_wallpaper_penrose_triangle_540x960
};

const uint8_t *kIMGLoading[16] = {
    ImageResource_item_loading_01_32x32,
    ImageResource_item_loading_02_32x32,
    ImageResource_item_loading_03_32x32,
    ImageResource_item_loading_04_32x32,
    ImageResource_item_loading_05_32x32,
    ImageResource_item_loading_06_32x32,
    ImageResource_item_loading_07_32x32,
    ImageResource_item_loading_08_32x32,
    ImageResource_item_loading_09_32x32,
    ImageResource_item_loading_10_32x32,
    ImageResource_item_loading_11_32x32,
    ImageResource_item_loading_12_32x32,
    ImageResource_item_loading_13_32x32,
    ImageResource_item_loading_14_32x32,
    ImageResource_item_loading_15_32x32,
    ImageResource_item_loading_16_32x32
};
const char *wallpapers_name_en[] = {
    "M5Paper",
    "Engine",
    "Penrose Triangle"
};
const char *wallpapers_name_zh[] = {
    "M5Paper",
    "引擎",
    "彭罗斯三角"
};
const char *wallpapers_name_ja[] = {
    "M5Paper",
    "エンジン",
    "ペンローズの三角形"
};
uint16_t global_wallpaper = DEFAULT_WALLPAPER;
uint8_t global_language = LANGUAGE_EN;
String global_wifi_ssid;
String global_wifi_password;
uint8_t global_wifi_configed = false;
uint16_t global_reader_textsize = 32;
uint8_t global_time_synced = false;
uint8_t global_ttf_file_loaded = false;
uint8_t global_init_status = 0xFF;
int8_t global_timezone = 8;
uint8_t global_lastFrame = 8;
String openWeatherApiKey = "25ccff7cff9dd2f07633eee9caa09008";
String cityName = "Budisov";
String stateCode = "cz";
String weatherUnits = "metric";    // standard/metric/imperial
String latestWeatherInformationJson = "";

// report of weather from openweather
struct openWeatherQuerry weatherReport;
// report of my current global ip address
struct myIpQuerry myIpReport;
// report my current city name and country code based on found global ip address
struct ipApiQuerry myIpApiReport;
String currentipAddress = "";
String backupIpAddress = "80.95.103.246";


// check if wifi connection is established, if not the it tries to connect with known wifi connection settings, 
// otherwise it will return 1 = OK, 0 = FAIL
uint8_t SetWifiConnection()
{
    Serial.printf("SSID: %s\n",GetWifiSSID().c_str());
    Serial.printf("PSWD: %s\n",GetWifiPassword().c_str());
    uint32_t lasttime;

    

    // if wifi is already connected, skip connection stage
    if(WiFi.isConnected())
        return 1;

    if(isWiFiConfiged())
        {

            lasttime = millis();
            Serial.println("\nConnecting to " + GetWifiSSID() + "...");
            WiFi.begin(GetWifiSSID().c_str(), GetWifiPassword().c_str());
            uint32_t t = millis();
            while (1)
            {
                if(millis() - t > 8000)
                {
                    Serial.println("\nWIFI not configured. SSID: " + GetWifiSSID());
                    WiFi.disconnect();
                    Serial.printf("\nTime to connect NOK %d milliseconds\n\n", (millis() - lasttime));
                    return 0;
                    break;
                }

                if(WiFi.status() == WL_CONNECTED)
                {
                    //frame_wifiscan->SetConnected(GetWifiSSID(), WiFi.RSSI());
                    Serial.println("\nWIFI CONNECTION: OK");
                    break;
                }
                else if (WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_CONNECTION_LOST)
                {
                    Serial.println("\nWIFI CONNECTION FAILED or LOST");
                    Serial.println("\nWIFI not configured. SSID: " + GetWifiSSID());
                    WiFi.disconnect();
                    Serial.printf("\nTime to connect NOK %d milliseconds\n\n", (millis() - lasttime));
                    return 0;
                    break;
                }
                else{
                  //Serial.println("WIFI CONNECTION: NOK");
                }

            }
        }
        else{
          Serial.println("\nWIFI not configured. SSID: " + GetWifiSSID());
          WiFi.disconnect();
          Serial.printf("\nTime to connect NOK %d milliseconds\n\n", (millis() - lasttime));
          return 0;

        }

    Serial.printf("\nTime to connect Ok %d milliseconds\n\n", (millis() - lasttime));    
    return 1;

}

// disconnect from current AP
uint8_t TerminateWifiConnection()
{
    // check if wifi is currently connected to any AP, if so disconnect
    if (WiFi.isConnected())
        WiFi.disconnect();

}

uint8_t GetCurrentIpAddress()
{
    /*if (!SetWifiConnection()){
        Serial.println("ERROR NO WIFI");
        return 0;
    }*/

    int retValue = 0;

    HTTPClient http;

    //http.setTimeout(10000);
    http.setRedirectLimit(5);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    //http.useHTTP10(true);
    
    // Send request
    //retValue = http.begin("http://api.ipify.org?format=json");
    //retValue = http.begin("https://api.myip.com");
    //retValue = http.begin("https://ip.seeip.org/jsonip?", root_ca);
    retValue = http.begin("http://ip.seeip.org/jsonip?");

    //retValue = http.begin("https://ip.seeip.org", root_ca);
    //retValue = http.GET();

    //retValue = http.begin("https://ip.seeip.org", 433, "/jsonip?", root_ca);

    if (!retValue)
    {
        Serial.println("HTTP BEGIN - ERROR");
        return 0;
    }

 
    retValue = http.GET();

    if (retValue < 0)
    {
        Serial.println("HTTP GET - ERROR");
        return 0;
    }
    else if (retValue >= 400){
        Serial.printf("HTTP ERROR CODE %d", retValue);
        return 0;
    }
    else if(retValue >= 100 && retValue < 400)
    {
        Serial.printf("HTTP OK CODE %d", retValue);
    }
    // Print the response
    Serial.printf("Received message %d\n", http.getSize());
    Serial.println(http.getString());

    /*
    StaticJsonDocument<128> doc;

    DeserializationError error = deserializeJson(doc, http.getString());

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
    }

    myIpReport.ip = doc["ip"]; // "66.249.75.9"
    myIpReport.country = doc["country"]; // "United States"
    myIpReport.cc = doc["cc"]; // "US"
    */

    StaticJsonDocument<48> doc;

    DeserializationError error = deserializeJson(doc, http.getString());

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
    }

    myIpReport.ip = doc["ip"]; // "80.95.103.246"

    currentipAddress = myIpReport.ip;

    Serial.println("My global IP adress is: " + currentipAddress);

    // Disconnect
    http.end();

    return 1;

}


uint8_t GetCurrentPositionBasedOnIpAddress()
{
    /*if (!SetWifiConnection()){
        Serial.println("ERROR NO WIFI");
        return 0;
    }

    if(!GetCurrentIpAddress()){
        Serial.println("ERROR NO IP");
    }
    // apply backup IP address
    else
    {
        currentipAddress = backupIpAddress;
    }*/

    int retValue = 0;

    HTTPClient http;

    http.setTimeout(10000);

    // Send request
    //http.useHTTP10(true);
    //retValue = http.begin("https://api.ipify.org");
    String request = "http://ip-api.com/json/"+currentipAddress;
    Serial.println("Request string: "+ request);
    retValue = http.begin("http://ip-api.com/json/"+currentipAddress);
    if (!retValue)
    {
        Serial.println("HTTP BEGIN - ERROR");
        return 0;
    }
 
    retValue = http.GET();

    if (retValue < 0)
    {
        Serial.println("HTTP GET - ERROR");
        return 0;
    }
    else if (retValue >= 400){
        Serial.printf("HTTP ERROR CODE %d", retValue);
        return 0;
    }
    else if(retValue >= 100 && retValue < 400)
    {
        Serial.printf("HTTP OK CODE %d", retValue);
    }
    // Print the response
    Serial.printf("Received message %d\n", http.getSize());

    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, http.getString());

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
    }

    myIpApiReport.status = doc["status"]; // "success"
    myIpApiReport.country = doc["country"]; // "Czechia"
    myIpApiReport.countryCode = doc["countryCode"]; // "CZ"
    myIpApiReport.region = doc["region"]; // "63"
    myIpApiReport.regionName = doc["regionName"]; // "Kraj Vysocina"
    myIpApiReport.city = doc["city"]; // "Ocmanice"
    myIpApiReport.zip = doc["zip"]; // "675 71"
    myIpApiReport.lat = doc["lat"]; // 49.2333
    myIpApiReport.lon = doc["lon"]; // 16.1167
    myIpApiReport.timezone = doc["timezone"]; // "Europe/Prague"
    myIpApiReport.isp = doc["isp"]; // "Vodafone Czech Republic a.s."
    myIpApiReport.org = doc["org"]; // "Vfcz Rudolfsedlak"
    myIpApiReport.as = doc["as"]; // "AS16019 Vodafone Czech Republic a.s."
    myIpApiReport.query = doc["query"]; // "80.95.103.246"

    cityName = myIpApiReport.city;
    stateCode = myIpApiReport.countryCode;

    Serial.println("My current city name is: " + cityName);
    Serial.println("My current country code is: " + stateCode);

    // Disconnect
    http.end();

    return 1;

}

uint8_t GetCurrentWeatherInformation()
{
    if (!SetWifiConnection()){
        Serial.println("ERROR NO WIFI");
        return 0;
    }

    if(!GetCurrentIpAddress()){
        Serial.println("ERROR NO IP");
        // apply backup IP address
        currentipAddress = backupIpAddress;
    }   

    if(!GetCurrentPositionBasedOnIpAddress()){
        Serial.println("ERROR NO POSITION"); 
        // apply backup
        cityName = "Prague";
        stateCode = "CZ";
    }


    int retValue = 0;

    HTTPClient http;

    http.setTimeout(10000);

    // Send request
    //http.useHTTP10(true);
    //retValue = http.begin("http://api.ipify.org");
    //retValue = http.begin("http://api.openweathermap.org/data/2.5/weather?q=Budisov,cz&units=metric&appid=25ccff7cff9dd2f07633eee9caa09008");
    retValue = http.begin("http://api.openweathermap.org/data/2.5/weather?q=" + cityName +"," + stateCode + "&units=" + weatherUnits +"&appid=" + openWeatherApiKey);
    //retValue = http.begin("http://www.arduino.cc/asciilogo.txt");
    if (!retValue)
    {
        Serial.println("HTTP BEGIN - ERROR");
        return 0;
    }
 
    retValue = http.GET();

    if (retValue < 0)
    {
        Serial.println("HTTP GET - ERROR");
        return 0;
    }
    else if (retValue >= 400){
        Serial.printf("HTTP ERROR CODE %d", retValue);
        return 0;
    }
    else if(retValue >= 100 && retValue < 400)
    {
        Serial.printf("HTTP OK CODE %d", retValue);
    }
    // Print the response
    Serial.printf("Received message %d\n", http.getSize());

    latestWeatherInformationJson = http.getString();

    Serial.print(latestWeatherInformationJson);


    // String input;

    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, latestWeatherInformationJson);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
    }

    weatherReport.coord_lon = doc["coord"]["lon"]; // 16.0038
    weatherReport.coord_lat = doc["coord"]["lat"]; // 49.2714

    for (JsonObject elem : doc["weather"].as<JsonArray>()) {

        weatherReport.id = elem["id"]; // 300, 741
        weatherReport.main = elem["main"]; // "Drizzle", "Fog"
        weatherReport.description = elem["description"]; // "light intensity drizzle", "fog"
        weatherReport.icon = elem["icon"]; // "09d", "50d"

    }

    weatherReport.base = doc["base"]; // "stations"

    JsonObject main = doc["main"];
    weatherReport.main_temp = main["temp"]; // 0.44
    weatherReport.main_feels_like = main["feels_like"]; // -4.15
    weatherReport.main_temp_min = main["temp_min"]; // 0
    weatherReport.main_temp_max = main["temp_max"]; // 1.11
    weatherReport.main_pressure = main["pressure"]; // 1025
    weatherReport.main_humidity = main["humidity"]; // 93

    weatherReport.visibility = doc["visibility"]; // 100

    weatherReport.wind_speed = doc["wind"]["speed"]; // 3.6
    weatherReport.wind_deg = doc["wind"]["deg"]; // 100

    weatherReport.clouds_all = doc["clouds"]["all"]; // 90

    weatherReport.dt = doc["dt"]; // 1613903659

    JsonObject sys = doc["sys"];
    weatherReport.sys_type = sys["type"]; // 1
    weatherReport.sys_id = sys["id"]; // 6843
    weatherReport.sys_country = sys["country"]; // "CZ"
    weatherReport.sys_sunrise = sys["sunrise"]; // 1613886852
    weatherReport.sys_sunset = sys["sunset"]; // 1613924720

    weatherReport.timezone = doc["timezone"]; // 3600
    weatherReport.doc_id = doc["id"]; // 3078472
    weatherReport.name = doc["name"]; // "BudiĹˇov"
    weatherReport.cod = doc["cod"]; // 200

    Serial.printf("Temp [°C]: %f\n", weatherReport.main_temp);
    Serial.printf("Wind speed [m/s]: %f\n", weatherReport.wind_speed);
    Serial.printf("Pressure [hPa]: %d\n", weatherReport.main_pressure);
    Serial.printf("Main humidity [%]: %d\n", weatherReport.main_humidity);

    // pass data to further processing
    //latestWeatherInformationJson = http.getString();

    // Disconnect
    http.end();

    return 1;

}

uint8_t GetWeatherUnits(void)
{   
    
    if (!weatherUnits.compareTo("metric"))
    {
        Serial.println("Weather unit 0-metric");
        return 0;
    }
    else if (!weatherUnits.compareTo("imperial"))
    {
        Serial.println("Weather unit 1-imperial");
        return 1;
    }
    else
    {
        Serial.println("Weather unit 2-standard");
        return 2;
    }
}

String GetLatestWeatherInformationJson(void)
{
    return latestWeatherInformationJson;
}

openWeatherQuerry GetLatestWeatherInformation(void)
{
    return weatherReport;
}

uint8_t GetGlobalLastFrame(void)
{
    return global_lastFrame;
}

void setGlobalLastFrame(uint8_t lastFrame)
{
    global_lastFrame = lastFrame;
}

int8_t GetTimeZone(void)
{
    return global_timezone;
}

void SetTimeZone(int8_t time_zone)
{
    global_timezone = time_zone;
}

void SetInitStatus(uint8_t idx, uint8_t val)
{
    global_init_status &= ~(1 << idx);
    global_init_status |= (val << idx);
}

uint8_t GetInitStatus(uint8_t idx)
{
    return (global_init_status & (1 << idx)) ? true : false;
}

void SetTTFLoaded(uint8_t val)
{
    global_ttf_file_loaded = val;
}

uint8_t isTTFLoaded()
{
    return global_ttf_file_loaded;
}

uint8_t isTimeSynced(void)
{
    return global_time_synced;
}

void SetTimeSynced(uint8_t val)
{
    global_time_synced = val;
    SaveSetting();
}

void SetLanguage(uint8_t language)
{
    if (language >= LANGUAGE_EN && language <= LANGUAGE_ZH)
    {
        global_language = language;
    }
    SaveSetting();
}

uint8_t GetLanguage(void)
{
    return global_language;
}

void SetWallpaper(uint16_t wallpaper_id)
{
    global_wallpaper = wallpaper_id;
    SaveSetting();
}

uint16_t GetWallpaperID(void)
{
    return global_wallpaper;
}

const uint8_t *GetWallpaper(void)
{
    return wallpapers[global_wallpaper];
}

const char *GetWallpaperName(uint16_t wallpaper_id)
{
    switch (global_language)
    {
    case LANGUAGE_ZH:
        return wallpapers_name_zh[wallpaper_id];
    case LANGUAGE_JA:
        return wallpapers_name_ja[wallpaper_id];
    default:
        return wallpapers_name_en[wallpaper_id];
    }
}

esp_err_t LoadSetting(void)
{
    nvs_handle nvs_arg;
    NVS_CHECK(nvs_open("Setting", NVS_READONLY, &nvs_arg));
    NVS_CHECK(nvs_get_u16(nvs_arg, "Wallpaper", &global_wallpaper));
    NVS_CHECK(nvs_get_u8(nvs_arg, "Language", &global_language));
    NVS_CHECK(nvs_get_u8(nvs_arg, "Timesync", &global_time_synced));
    nvs_get_i8(nvs_arg, "timezone", &global_timezone);

    if(global_wallpaper >= WALLPAPER_NUM)
    {
        global_wallpaper = DEFAULT_WALLPAPER;
    }

    size_t length = 128;
    char buf[128];
    NVS_CHECK(nvs_get_str(nvs_arg, "ssid", buf, &length));
    global_wifi_ssid = String(buf);
    length = 128;
    NVS_CHECK(nvs_get_str(nvs_arg, "pswd", buf, &length));
    global_wifi_password = String(buf);
    global_wifi_configed = true;


    NVS_CHECK(nvs_get_u8(nvs_arg, "lastFrame", &global_lastFrame));

    nvs_close(nvs_arg);

    Serial.println("LOAD SETTiNGS: OK");

    return ESP_OK;
}

esp_err_t SaveSetting(void)
{
    nvs_handle nvs_arg;
    NVS_CHECK(nvs_open("Setting", NVS_READWRITE, &nvs_arg));
    NVS_CHECK(nvs_set_u16(nvs_arg, "Wallpaper", global_wallpaper));
    NVS_CHECK(nvs_set_u8(nvs_arg, "Language", global_language));
    NVS_CHECK(nvs_set_u8(nvs_arg, "Timesync", global_time_synced));
    NVS_CHECK(nvs_set_i8(nvs_arg, "timezone", global_timezone));
    NVS_CHECK(nvs_set_str(nvs_arg, "ssid", global_wifi_ssid.c_str()));
    NVS_CHECK(nvs_set_str(nvs_arg, "pswd", global_wifi_password.c_str()));
    NVS_CHECK(nvs_set_u8(nvs_arg, "lastFrame", global_lastFrame));
    NVS_CHECK(nvs_commit(nvs_arg));
    nvs_close(nvs_arg);

    Serial.println("SAVE SETTINGS: OK");

    return ESP_OK;
}

void SetWifi(String ssid, String password)
{
    global_wifi_ssid = ssid;
    global_wifi_password = password;
    SaveSetting();
}

uint8_t isWiFiConfiged(void)
{
    return global_wifi_configed;
}

String GetWifiSSID(void)
{
    return global_wifi_ssid;
}

String GetWifiPassword(void)
{
    return global_wifi_password;
}

bool SyncNTPTime(void)
{
    const char *ntpServer = "time.cloudflare.com";
    configTime(global_timezone * 3600, 0, ntpServer);

    struct tm timeInfo;
    if (getLocalTime(&timeInfo))
    {
        rtc_time_t time_struct;
        time_struct.hour = timeInfo.tm_hour;
        time_struct.min = timeInfo.tm_min;
        time_struct.sec = timeInfo.tm_sec;
        M5.RTC.setTime(&time_struct);
        rtc_date_t date_struct;
        date_struct.week = timeInfo.tm_wday;
        date_struct.mon = timeInfo.tm_mon + 1;
        date_struct.day = timeInfo.tm_mday;
        date_struct.year = timeInfo.tm_year + 1900;
        M5.RTC.setDate(&date_struct);
        SetTimeSynced(1);
        return 1;
    }
    log_d("Time Sync failed");
    return 0;
}

uint16_t GetTextSize()
{
    return global_reader_textsize;
}

void SetTextSize(uint16_t size)
{
    global_reader_textsize = size;
}

const uint8_t* GetLoadingIMG_32x32(uint8_t id)
{
    return kIMGLoading[id];
}

void __LoadingAnime_32x32(void *pargs)
{
    uint16_t *args = (uint16_t *)pargs;
    uint16_t x = args[0];
    uint16_t y = args[1];
    free(pargs);
    M5EPD_Canvas loading(&M5.EPD);
    loading.createCanvas(32, 32);
    loading.fillCanvas(0);
    loading.pushCanvas(x, y, UPDATE_MODE_GL16);
    int anime_cnt = 0;
    uint32_t time = 0;
    while (1)
    {
        if(millis() - time > 200)
        {
            time = millis();
            loading.pushImage(0, 0, 32, 32, GetLoadingIMG_32x32(anime_cnt));
            loading.pushCanvas(x, y, UPDATE_MODE_DU4);
            anime_cnt++;
            if(anime_cnt == 16)
            {
                anime_cnt = 0;
            }
        }

        xSemaphoreTake(_xSemaphore_LoadingAnime, portMAX_DELAY);
        if(_loading_anime_eixt_flag == true)
        {
            xSemaphoreGive(_xSemaphore_LoadingAnime);
            break;
        }
        xSemaphoreGive(_xSemaphore_LoadingAnime);
    }
    vTaskDelete(NULL);
}

void LoadingAnime_32x32_Start(uint16_t x, uint16_t y)
{
    if(_xSemaphore_LoadingAnime == NULL)
    {
        _xSemaphore_LoadingAnime = xSemaphoreCreateMutex();
    }
    _loading_anime_eixt_flag = false;
    uint16_t *pos = (uint16_t*)calloc(2, sizeof(uint16_t));
    pos[0] = x;
    pos[1] = y;
    xTaskCreatePinnedToCore(__LoadingAnime_32x32, "__LoadingAnime_32x32", 16 * 1024, pos, 1, NULL, 0);
}

void LoadingAnime_32x32_Stop()
{
    xSemaphoreTake(_xSemaphore_LoadingAnime, portMAX_DELAY);
    _loading_anime_eixt_flag = true;
    xSemaphoreGive(_xSemaphore_LoadingAnime);
    delay(200);
}
