#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include "esp_http_server.h"
#include "cJSON.h"
#include "driver/gpio.h"
#include "lm75a.h"
#include "esp_spiffs.h"

#define TAG "SERVER"
#define LED 12

static esp_err_t on_url_hit(httpd_req_t *req){
    
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    //We configure spiffs
    esp_vfs_spiffs_conf_t config = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false
    };
    esp_vfs_spiffs_register(&config);
    char path[600];
    //if the uri typed in is forward slash (/) we are going to set our path to our index.html
    //function strcmp returns 0 if strings are equal
    if(strcmp(req->uri, "/") == 0){
        strcpy(path, "/spiffs/index.html");
    }
    //otherwise it is going to be /spiffs and since uri always starts with a / we can directly set it to /spiffs%s
    else{
        sprintf(path, "/spiffs%s", req->uri);
    }

    ESP_LOGI(TAG, "path on spiffs %s",path);

    //style.css MIME Type Fix
    //The best way to verify if it is a css file is to point to the dot and see what is after that particular dot
    char *ptr = strrchr(path, '.');
    if(strcmp(ptr, ".css") == 0 ){

        ESP_LOGI(TAG, "setting mime type to .css");
        //this will tell chrome to use this particular find that is found as a .css
        httpd_resp_set_type(req, "text/css");
    }
    
    //let's attempt to open up the file now !!!
    FILE *file = fopen(path, "r"); //read it only
    //if the file doesn't exist we should return a 404 NOT FOUND error
    if (file == NULL){
        httpd_resp_send_404(req);
    }
    else{
        //read the file and then send it through
        char lineRead[256];
        while (fgets(lineRead, sizeof(lineRead), file)){

            httpd_resp_sendstr_chunk(req, lineRead);
        }
        //When we have finished sending through the entire file we send a chunk with a NULL and that will terminate the connection
        httpd_resp_sendstr_chunk(req, NULL);
    }

    // char *message = "{\"message\":\"Hello World\"}"; //It can be a JSON or a string for instance
    // httpd_resp_send (req, message, strlen(message));
    esp_vfs_spiffs_unregister(NULL); //We must remember to de-initialize it, NULL will take the first valid partition that we have !
    return ESP_OK;
}

static esp_err_t on_get_temperature(httpd_req_t *req){
    
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    float temperature = readTemperature();
    char message[100];
    sprintf(message,"{\"temperature\":\"%f\"}", temperature);//It can be a JSON or a string for instance
    httpd_resp_send (req, message, strlen(message));
    return ESP_OK;
}

static esp_err_t on_get_database(httpd_req_t *req){
    
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char database[100];
    char message[116];
    memset(database, 0, sizeof(database));
    strcpy(database, "DatabaseTextWillGoHere");
    sprintf(message, "{\"database\":\"%s\"}", database);
    httpd_resp_send (req, message, strlen(message));
    return ESP_OK;
}


//** REGISTER POST FUNCTION **/
static esp_err_t on_led_set(httpd_req_t *req){
    
    ESP_LOGI(TAG, "url %s was hit", req->uri);
    char buffer[150]; //Create a buffer to be able to hold the information that was posted to us
    memset(&buffer, 0, sizeof(buffer)); //Once we copy the request the buffer won't be NULL terminated so we have to initialize it with all 0s
    httpd_req_recv(req, buffer, req->content_len); // Content length, actual size of the POST method request
    cJSON *payload =cJSON_Parse(buffer); //Convert input to cJSON object
    cJSON *isLedOn = cJSON_GetObjectItem(payload,"isLedOn"); //Look for object isLedOn that is part of the object
    gpio_set_level(LED, cJSON_IsTrue(isLedOn)); //We can use cJSON_IsTrue to read wether is true or false
    cJSON_Delete(payload); //We always have to delete the cJSON object to clear memory
    //Before this end point can get called we need to initialize the GPIO, we do that in the void InitializeLed() function below

    ESP_LOGI(TAG,"%s", buffer);
    //Set up the HTTP response to the request 
    httpd_resp_set_status(req, "204 NO CONTENT"); //204 NO CONTENT is a standard HTTP response when we do not want to send a response but yet everything was OK
    //return  the result
    httpd_resp_send(req, NULL, 0); //We will parse in the request, the buffer will be NULL (we don't send anything back)
    return ESP_OK;
}

void InitializeLed(){
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void RegisterEndPoints(void){

    InitializeLed(); //We initialize the GPIO so we can set the LED level on the function
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG(); //Very useful macro to set default configuration parameters for our server
    config.uri_match_fn = httpd_uri_match_wildcard; //to handle wild card routing
    
    ESP_LOGI(TAG, "starting server");
    if(httpd_start(&server, &config) != ESP_OK){
        ESP_LOGE(TAG, "COULD NOT START THE SERVER");
    }

   httpd_uri_t temperature_end_point_config = {
        .uri = "/api/temperature", // using /api is a convention we use
        .method = HTTP_GET,
        .handler = on_get_temperature
    };
    //We need to register the configuration with our server
    httpd_register_uri_handler(server, &temperature_end_point_config);

    
   httpd_uri_t database_end_point_config = {
        .uri = "/api/database", // using /api is a convention we use
        .method = HTTP_GET,
        .handler = on_get_database
    };
    //We need to register the configuration with our server
    httpd_register_uri_handler(server, &database_end_point_config);


    //** POST REQUEST EXAMPLE **///
   httpd_uri_t led_end_point_config = {
        .uri = "/api/led", // using /api is a convention we use
        .method = HTTP_POST,
        .handler = on_led_set
    };
    //We need to register the configuration with our server
    httpd_register_uri_handler(server, &led_end_point_config);

    //Register and endpoint. F12, look into the definition there is a callback function where we are going to do some work once the URL gets hit

    httpd_uri_t first_end_point_config = {
        .uri = "/*", // We add a * so we handle wildcards
        .method = HTTP_GET,
        .handler = on_url_hit
    };
    //We need to register the configuration with our server
    httpd_register_uri_handler(server, &first_end_point_config); //it is httpd_ because 'd' stands for deamon, which means that is going to sit on the background and wait for incoming requests


}
