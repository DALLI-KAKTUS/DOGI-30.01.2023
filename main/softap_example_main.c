/* 
  ┌─────────────────────────────────────────────────────────────────────────┐
  │ DOA İÇİN HAZIRLADIIM HARİKA DOUM GÜNÜ HEDİYESİ                          │
  └─────────────────────────────────────────────────────────────────────────┘


  ┌─────────────────────────────────────────────────────────────────────────┐
  │ BAĞLANTILAR                                                             │
  │ -----------                                                             │
  │ OLED EKRAN                                                              │
  │ SDA => 3                                                                |
  │ SCL => 2                                                                │
  └─────────────────────────────────────────────────────────────────────────┘
  ┌─────────────────────────────────────────────────────────────────────────┐
  │ PARÇALAR                                                                |
  |--------------                                                           │
  │ 128X64 I2C SSD1306 OLED EKRAN                                           |
  │ ESP32S2 MINI KART (ESP32-S2FN4R2)                                       |
  │ 8GB SD KART                                                             |
  │ KARBONFİBER KUTU                                                        │
  └─────────────────────────────────────────────────────────────────────────┘
 */

//Kütüphaneler
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include <esp_http_server.h>
#include <esp_ota_ops.h>
#include "lwip/err.h"
#include "lwip/sys.h"
/*
#include "ssd1306.h" 

*/
/*
#include "ssd1306.h"
#include "font8x8_basic.h"
*/
//WIFI tanımlamaları
#define WIFI_SSID      "DOGA"
#define WIFI_PASS      "30052022"
#define WIFI_CHANNEL    1
#define MAX_STA_CONN    2

static const char *TAG = "GOTLALEM";



//oled ekran pin tanımlamaları
#define oled_sda 7
#define oled_scl 6
#define oled_rst 35

//bitmapler

uint8_t doga_pogaca_bitmap[] = {
// 'doa_pogaca_bitmap', 64x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x07, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0x83, 0xff, 
0xff, 0xff, 0x00, 0x7f, 0xfc, 0x7f, 0x81, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0xfc, 0x6e, 0xc1, 0xff, 
0xff, 0xff, 0x7f, 0x7f, 0xf8, 0x7f, 0xc1, 0xff, 0xff, 0xff, 0x1e, 0x7f, 0xf8, 0x7f, 0xc0, 0xff, 
0xff, 0xff, 0xc0, 0xff, 0xf0, 0x61, 0xc0, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xf0, 0x61, 0xc0, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xf0, 0x73, 0xc0, 0xff, 0xff, 0xff, 0x87, 0xff, 0xf0, 0x7f, 0xc0, 0xff, 
0xff, 0xff, 0xb3, 0xff, 0xf0, 0x3f, 0xc0, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xf0, 0x1f, 0xc0, 0xff, 
0xff, 0xff, 0x39, 0xff, 0xf0, 0x0f, 0xc0, 0xff, 0xff, 0xff, 0x7c, 0xff, 0xf0, 0x0f, 0xc0, 0xff, 
0xff, 0xff, 0x7e, 0xff, 0xf0, 0x0f, 0xc0, 0xff, 0xff, 0xff, 0x7e, 0xff, 0xf8, 0x1f, 0xc0, 0xff, 
0xff, 0xff, 0x3e, 0xff, 0xf8, 0x1f, 0xc0, 0x3f, 0xff, 0xff, 0xbe, 0xff, 0xc0, 0x0f, 0xc0, 0x3f, 
0xff, 0xff, 0xce, 0x7f, 0x98, 0x21, 0xc7, 0x1f, 0xff, 0xff, 0xef, 0x7f, 0x30, 0x38, 0x07, 0x8f, 
0xff, 0xff, 0xe7, 0x7f, 0x70, 0x3f, 0x8f, 0xcf, 0xff, 0xff, 0xe7, 0x7e, 0x70, 0x7f, 0x0f, 0xcf, 
0xff, 0xff, 0xe7, 0x7c, 0xf8, 0x7f, 0x0f, 0xe7, 0xff, 0xff, 0xef, 0x79, 0xfc, 0x7f, 0x0f, 0xf7, 
0xff, 0xff, 0xef, 0x71, 0xff, 0xfe, 0x0f, 0xf7, 0xff, 0xff, 0xef, 0x25, 0xff, 0xfe, 0x27, 0xf7, 
0xff, 0xff, 0xef, 0x8d, 0xf3, 0xbb, 0xf7, 0xf3, 0xff, 0xff, 0xef, 0xfd, 0xf5, 0x55, 0xf7, 0xfb, 
0xff, 0xff, 0xef, 0xfc, 0xf5, 0x51, 0xfb, 0xf9, 0xff, 0xff, 0xef, 0xfe, 0xf3, 0xb5, 0xfb, 0xfd, 
0xff, 0xff, 0xe7, 0xf8, 0x7f, 0xff, 0xfb, 0xfd, 0xff, 0xff, 0xf7, 0xf3, 0x7e, 0x4f, 0xfb, 0xf1, 
0xff, 0xff, 0xf3, 0xe7, 0x7e, 0x0f, 0xfa, 0x07, 0x01, 0xff, 0xf9, 0x9f, 0x7f, 0x1f, 0xf8, 0xf7, 
0x7c, 0xff, 0xfc, 0x3f, 0x7f, 0xbf, 0xfd, 0xf7, 0xfe, 0x7f, 0xff, 0xff, 0x7f, 0xff, 0xfd, 0xf7, 
0xff, 0x1f, 0xff, 0xff, 0x44, 0x45, 0x45, 0xf7, 0xff, 0xc7, 0xff, 0xff, 0x55, 0xd5, 0x5d, 0xf7, 
0xff, 0xf1, 0xff, 0xff, 0x44, 0x4c, 0xc5, 0xe7, 0xff, 0xfc, 0xff, 0xff, 0x55, 0xd5, 0x5d, 0xef, 
0xff, 0xff, 0x3f, 0xff, 0x44, 0x55, 0x45, 0xef, 0xff, 0xff, 0x9f, 0xf8, 0x7f, 0xff, 0xfd, 0xef, 
0xff, 0xff, 0xc7, 0xc3, 0x3f, 0xff, 0xfd, 0xef, 0xff, 0x80, 0xf3, 0x9f, 0xdf, 0xff, 0xf9, 0xcf, 
0xff, 0x00, 0x78, 0xbf, 0xcf, 0xff, 0xf1, 0xdf, 0xfe, 0x00, 0x3e, 0x3f, 0xe7, 0xff, 0xe7, 0xdf, 
0xf4, 0x00, 0x3f, 0xc1, 0xf3, 0xff, 0xcf, 0xdf, 0xf4, 0x00, 0x3f, 0xfd, 0xf8, 0xff, 0x1f, 0xdf, 
0xf6, 0x00, 0x67, 0xfc, 0xfe, 0x3e, 0x7f, 0x9f, 0xf7, 0x81, 0xef, 0x8e, 0x3f, 0x98, 0xfe, 0x3f, 
0xf3, 0xff, 0xcf, 0x07, 0x8f, 0xd3, 0xfe, 0xff, 0xf8, 0x00, 0x1f, 0x07, 0xe7, 0xc7, 0xf0, 0xff, 
0xff, 0xff, 0xff, 0x8f, 0xf2, 0x1f, 0xc5, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xfc, 0xff, 0x9d, 0xff, 
0xff, 0xff, 0xfe, 0x03, 0xfe, 0x20, 0x3d, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0x8f, 0xfd, 0xff, 
0xff, 0xff, 0xff, 0x07, 0xff, 0xe7, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xfc, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x7e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x7f
};


//html dosyalarının gömülmesi
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

//WIFI kurulumu

//wifi işleyicisi
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}
//wifi başlatıcı
void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
//wifi ayarları
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = WIFI_CHANNEL,
            .password = WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_OPEN,
            .pmf_cfg = {
                    .required = false,
            },
        },
    };
//wifi modu için err check ve logging
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap Bitti. SSID:%s password:%s channel:%d",
             WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
}

//index için GET işleyicisi
static esp_err_t index_get_isleyici(httpd_req_t *req)
{
	esp_err_t error;
	ESP_LOGI (TAG, "index cagrildi");
	error = httpd_resp_send(req, (const char *) index_html_start, index_html_end - index_html_start);;
	if (error != ESP_OK)
	{
	ESP_LOGI (TAG, "Error %d while sending Response", error);
	}
else ESP_LOGI (TAG, "Response sent Successfully");
return error;	

}



//uri yapıları

//index URI yapısı
static const httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_get_isleyici,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};




//404 hatası işleyicisi
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
        /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

//WEB için ekstra işleyiciler

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        //URI işleyicileri, iki alt satırdaki kodu her URI yapısı için yaz
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &index_uri);
        return server;
    }
    ESP_LOGI(TAG, "Error starting server!");
    return server == NULL ? ESP_FAIL : ESP_OK;;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    return httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        if (stop_webserver(*server) == ESP_OK) {
            *server = NULL;
        } else {
            ESP_LOGE(TAG, "Failed to stop http server");
        }
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

//ekran isimlendirmesi
SSD1306_t ekran;
//oledin kurulumu ve temel ayarları
void oled_kurulumu(){
    //i2c icin init
    ESP_LOGI(TAG, "Oled Ekran SDA Pini=%d",oled_sda);
    ESP_LOGI(TAG, "Oled Ekran SCL Pini=%d",oled_scl);
    ESP_LOGI(TAG, "Oled Ekran RST Pini=%d",oled_rst);
    i2c_master_init(&ekran, oled_sda, oled_scl, oled_rst);

    //oled ters döndürme
    ekran._flip = false;
    //ekran başlatıcı
    ssd1306_init(&ekran, 128, 64);
    //oled ekran kontrast ayarı
    ssd1306_contrast(&ekran, 0xff);
    //oled ekran temizleme ayarı
    ssd1306_clear_screen(&ekran, false);
}
//cihaz açıldığında oled ekranda gösterilecek için task handler tanımlaması
TaskHandle_t oled_baslangic_isleyici = NULL;
//cihaz açıldığında oled ekranda gösterilecekler için task fonksiyonu
void oled_baslangic(){
    while (1)
    {   
        ssd1306_clear_screen(&ekran, false);
	    ssd1306_bitmaps(&ekran, 0, 0, doga_pogaca_bitmap, 64, 64, false);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

}

void app_main(void)
{
    //server isinmendirmesi   
    static httpd_handle_t server = NULL;
    //NVS başlatıcı
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    //wifi çalıştırma
    wifi_init_softap();
    //wifi işleyicisi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
    
 
    //oled kurulumunun çalıştırılması
    oled_kurulumu();
    //cihaz açıldığında oled ekranda gösterilecek için task
    xTaskCreate(oled_baslangic, "oled_baslangic", 4096, NULL, 10, &oled_baslangic_isleyici);

}
