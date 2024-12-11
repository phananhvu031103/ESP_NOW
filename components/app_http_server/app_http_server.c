#include "app_http_server.h"
#include <esp_http_server.h>
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "app_http_server";
static httpd_handle_t server = NULL;
static http_server_handle http_get_handle = NULL;
static http_server_handle http_post_handle = NULL;
static http_server_handle rgb_post_handle = NULL;
// extern const uint8_t image_start[] asm("_binary_tiennghich_jpg_start");
// extern const uint8_t image_end[] asm("_binary_tiennghich_jpg_end");

extern const uint8_t web_start[] asm("_binary_web_html_start");
extern const uint8_t web_end[] asm("_binary_web_html_end");


// An HTTP GET handler for the /hello URI
static esp_err_t hello_get_handler(httpd_req_t *req)
{
    // char*  buf = "Hello World!";
    // size_t buf_len = strlen(buf);

    // Set the response content type to text/plain
    // httpd_resp_send(req, buf, buf_len); //in ra chuoi "Hello World!"
    
    //xuat ra anh tiennghich
    //httpd_resp_set_type(req, "image/jpg"); //set type of response
    //httpd_resp_send(req, (const char *)image_start, image_end - image_start); 

    httpd_resp_send(req, (const char *)web_start, web_end - web_start); 
   

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
       // ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

// Handler for the /hello URI resource hello_get_handler
const httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "Hello World!"
};

static httpd_req_t *get_req;

void http_send_response(char *data, int len)
{
    httpd_resp_send(get_req, data, len);
}

static esp_err_t dht11_get_data_handler(httpd_req_t *req)
{
    get_req = req;
    // static float dht11_temp = 25.3;
    // static float dht11_hum = 80.5; 

    // dht11_temp += 1;
    // dht11_hum += 1;

    // if (dht11_temp > 50.0) {
    //     dht11_temp = 25.3;
    // }
    
    // if (dht11_hum > 100.0) {
    //     dht11_hum = 80.5;
    // }
    // char buf[100];
    // sprintf(buf, "{\"temperature\": \"%.1f\", \"huminity\":\"%.1f\"}", dht11_temp, dht11_hum);

    // size_t buf_len = strlen(buf);
    // httpd_resp_send(req, (const char*)buf, buf_len); 

    http_get_handle("dht11", 5);

    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        //ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}
const httpd_uri_t dht11 = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = dht11_get_data_handler,
    .user_ctx  = "DHT11"
};


static esp_err_t sw1_data_handler(httpd_req_t *req)
{
    char buf[100];
    //size_t buf_len = strlen(buf);
   
    int len =  httpd_req_recv(req, buf, 100);
    http_post_handle(buf, len);
    //printf("%s\n", buf);
    
      // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
static const httpd_uri_t sw1 = {
    .uri       = "/sw1",
    .method    = HTTP_POST,
    .handler   = sw1_data_handler,
    .user_ctx  = "sw1"
};


static esp_err_t slider_data_handler(httpd_req_t *req)
{
    char buf[100];
   /*read data*/
    int len =  httpd_req_recv(req, buf, 100);
   
    printf("%s\n", buf);
    
      // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
static const httpd_uri_t slider = {
    .uri       = "/slider",
    .method    = HTTP_POST,
    .handler   = slider_data_handler,
    .user_ctx  = "slider"
};

static esp_err_t rgb_data_handler(httpd_req_t *req)
{
     /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    int buf_len = httpd_req_get_url_query_len(req) + 1;
    char *buf;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            char param[64];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "color", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => color=%s", param);
                rgb_post_handle(param, strlen(param));
            }
        }
        free(buf);  
    }
    httpd_resp_send(req, "OK", 2);      
    return ESP_OK;
}
const httpd_uri_t rgb = {
    .uri       = "/rgb",
    .method    = HTTP_GET,
    .handler   = rgb_data_handler,
    .user_ctx  = "RGB"
};

//start_webserver function
void start_webserver(void) 
{

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server 
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);  // port 80
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers 
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello); //resource handler
        httpd_register_uri_handler(server, &dht11);
        httpd_register_uri_handler(server, &sw1);
        httpd_register_uri_handler(server, &slider);
        httpd_register_uri_handler(server, &rgb);
    }
    else{
        ESP_LOGI(TAG, "Error starting server!");
    }   
}

//stop_webserver function
void stop_webserver(void)
{
    // Stop the httpd server
    httpd_stop(server);
}

void http_get_set_callback(void *cb)
{
    if(cb)
    {
        http_get_handle = cb;
    }
}


void http_post_set_callback(void *cb)
{
    if(cb)
    {
        http_post_handle = cb;
    }
}


void rgb_post_set_callback(void *cb)
{
    if(cb)
    {
        rgb_post_handle = cb;
    }
}