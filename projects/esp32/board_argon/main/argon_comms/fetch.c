
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"
#include "fetch.h"

#define TAG "HTTPCLIENT"

char *buffer = NULL;
int indexBuffer = 0;

esp_err_t clientEventHandler(esp_http_client_event_t *evt)
{
    struct fetchParams_t *fetchParams = (struct fetchParams_t *)evt->user_data;
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA Len=%d", evt->data_len);
        //%.*s, it expects two arguments, the length of the string and the string itself, printf("%.*s\n", str_len, str);
        printf("%.*s\n", evt->data_len, (char *)evt->data);
        //If first time getting information
        if (buffer == NULL)
        {
            buffer = (char *)malloc(evt->data_len);
        }
        else
        {
            buffer = (char *)realloc(buffer, evt->data_len + indexBuffer);
        }
        memcpy(&buffer[indexBuffer], evt->data, evt->data_len);
        indexBuffer = indexBuffer + evt->data_len;
        break;

    //When all data has been received there are a few things we need to do
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        buffer = (char *)realloc(buffer, indexBuffer + 1);
        //Null terminate the buffer so it's easier to treat is as a string
        memcpy(&buffer[indexBuffer], "\0", 1);
        ESP_LOGI(TAG, "%s", buffer);
        //OnGotData(buffer);
        //We call OnGotData and we parse in the incoming buffer, and as an output we ask to allocate it to fetchParams->message
        if (fetchParams->OnGotData != NULL)
        {
            fetchParams->OnGotData(buffer, fetchParams->message);
        }
        free(buffer);
        buffer = NULL;
        indexBuffer = 0;
        break;

    default:
        break;
    }

    return ESP_OK;
}

void fetch(char *url, struct fetchParams_t *fetchParams)
{
    esp_http_client_config_t clientConfig = {
        .url = url,
        .event_handler = clientEventHandler,
        .user_data = fetchParams};

    esp_http_client_handle_t client = esp_http_client_init(&clientConfig);

    if (fetchParams->method == POST)
    {

        esp_http_client_set_method(client, HTTP_METHOD_POST);
    }

    //We set all the headers of the request
    for (int i = 0; i < fetchParams->headerCount; i++)
    {
        esp_http_client_set_header(client, fetchParams->header[i].key, fetchParams->header[i].val);
    }

    //We set the body of the request
    if (fetchParams->body != NULL)
    {
        esp_http_client_set_post_field(client, fetchParams->body, strlen(fetchParams->body));
    }

    esp_err_t err = esp_http_client_perform(client);
    fetchParams->status = esp_http_client_get_status_code(client);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "fetch() :  GET or POST HTTP status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "fetch() : HTTP GET or POST request failed: %s", esp_err_to_name(err));
    }
    
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

//Send reading struct over HTTP

int postReading(struct reading_t *reading)
{
    ESP_LOGI(TAG, "fetch.c : postReading()");

    struct fetchParams_t fetchParams;
    fetchParams.OnGotData = NULL;
    fetchParams.method = POST;

    header_t headerContentType = {
        .key = "Content-Type",
        .val = "application/json"};

    fetchParams.header[0] = headerContentType;
    fetchParams.headerCount = 1;

    //I cannot directly pass fetchParams.body because it is a pointer in memory, I would have to do a malloc. In this case I decide to create a buffer
    char buffer[1024];
    //Populate the body of the message, we create a method
    createBodyReading(reading, buffer);
    fetchParams.body = buffer ;
    //We update out fetch.c to acomodate the new parameters
    //fetch("http://192.168.1.159:5000/postReadingESP32", &fetchParams);
    fetch("http://100.15.123.61:5001/postReadingESP32", &fetchParams);    

    ESP_LOGI(TAG, "postReading () : status code = %d", fetchParams.status);

    return fetchParams.status;
}

//3 parameters, number, message out to create the string
// void createBody(char *number, char *message, char *outputString){
// Add JSON structure to the string, we can use cJSON (look at documentation) or we can do it as follows
void createBodyReading(struct reading_t *reading, char *body)
{
    //I have to escape the quotes
    //For formatting purposes on the sprintf() function I have to add quotes at the beginning and at the end of each line
    sprintf(body,
            "    {"
            "   \"datetime\" : \"%s\",   "
            "    \"intensity\" : \"%d\",   "
            "    \"latitude\" : \"%f\",   "
            "    \"longitude\" : \"%f\",   "
            "    \"sensor_id\" : \"%d\",   "
            "   \"vgas\" : \"%f\",   "
            "   \"vgas0\" : \"%f\",   "
            "   \"temperature\" : \"%f\",   "
            "   \"ppm\" : \"%f\",   "
            "   \"rgain\" : \"%d\"   "
            "    }",
            reading->datetime, reading->intensity, reading->latitude, reading->longitude, reading->sensor_id, reading->vgas, reading->vgas0, reading->temperature, reading->ppm, reading->rgain);
    //I need to put each individual line in an individual quote "line 1"
    //ESP_LOGI(TAG, " createBodyReading() : HTTP POST Body String = %s", body);
}

//  void createBody(char *number, char *message, char *out)
// {
//   sprintf(out,
//           "{"
//           "  \"messages\": ["
//           "      {"
//           "      "
//           "          \"content\": \"%s\","
//           "          \"destination_number\": \"%s\","
//           "          \"format\": \"SMS\""
//           "      }"
//           "  ]"
//           "}",
//           message, number);
// }

// I (3533) HTTPCLIENT: HTTP_EVENT_ON_FINISH
// I (3543) HTTPCLIENT: {
//     "success": {
//         "total": 1
//     },
//     "contents": {
//         "quotes": [
//             {
//                 "quote": "If you are born poor, it is not your mistake. But if you die poor it is your mistake.",
//                 "length": "85",
//                 "author": "Bill Gates",
//                 "tags": {
//                     "0": "fail",
//                     "1": "inspire",
//                     "2": "poor",
//                     "4": "winning"
//                 },
//                 "category": "inspire",
//                 "language": "en",
//                 "date": "2020-04-08",
//                 "permalink": "https://theysaidso.com/quote/bill-gates-if-you-are-born-poor-it-is-not-your-mistake-but-if-you-die-poor-it-is",
//                 "id": "T2jjeTPKQNASih_HgzXOYgeF",
//                 "background": "https://theysaidso.com/img/qod/qod-inspire.jpg",
//                 "title": "Inspiring Quote of the day"
//             }
//         ]
//     },
//     "baseurl": "https://theysaidso.com",
//     "copyright": {
//         "year": 2022,
//         "url": "https://theysaidso.com"
//     }