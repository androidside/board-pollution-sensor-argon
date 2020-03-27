#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "module_jordi.h"


void app_main(void)
{
  moduleFunction(4,9);
  printf("Result is = %d\n", result);

  
  
  /**************************************************/
 /******************* - Function Pointers -  *******************/
/**************************************************/

// typedef struct person_t{
// char firstName[20];
// char lastName[20];
// int age;
// void (*DoWork)(char *dataRetrieved)
// }person_t;

// void updatePerson(person_t *person, char *dataRetrieved){
//   strcpy(person->firstName, "From Data Retrieved");
//   strcpy(&person->lastName, "From Data Retrieved");
//   person->DoWork=doWorkForPerson;
//   person->DoWork("some params");
//   person->age =  29;
//   }

 
// void connectAndGetInfo(char * url,void (*DoWork)(char *dataRetrieved))
// //function pointer needs to specify what type of data is expecting on the call!
// {
//   //connect to wireless
//   // connect to endpoint
//   // allocate memory
//   char *dataRetrieved = (char *)malloc(1024);
//   // fill buffer with data: getRequest(&dataRetrieved);
//   // do work and get a person object
//   DoWork(dataRetrieved);
//   // clean up memory and other resources
//   free((void *)dataRetrieved);
// }

// void doWorkForPerson(char *dataRetrieved)
// {
//   person_t person;
//   updatePerson(&person, *dataRetrieved);
//   printf("My name is %s %s and I am %d years old\n",person.firstName, person.lastName, person.age);
// }

// void app_main(void)
// {
//   person_t person;
//   connectAndGetInfo("http://getperson.com", doWorkForPerson);



  /**************************************************/
 /******************* - Structure and Pointer -  *******************/
/**************************************************/


// typedef struct person_t{
// char firstName[20];
// char lastName[20];
// int age;
// }person_t;

// void updatePerson(person_t *person, char firstName[20], char lastName[20], int age){
//   strcpy(person->firstName, firstName);
//   strcpy(&person->lastName, lastName);
//   person->age =  29;
//   }

//   void exclamIt(char *phrase){
//     strcat(phrase,"!");
//   }

// void app_main(void)
// {
//   person_t person;
//   char phrase[20] = "Hello World";
//   exclamIt(phrase);
//   printf("%s\n",phrase);
//   updatePerson(&person, "Bobby", "Fisher", 29);

//   printf("My name is %s %s and I am %d years old\n",person.firstName, person.lastName, person.age);

// }





    
/**************************************************/
 /******************* - Keyboard input -  *******************/
/**************************************************/
  // char c = 0;
  // char str[100];
  // memset(str,0,sizeof(str));
  // while(c!='\n')
  // {
  //   c = getchar(); //returns an integer! NOT A BLOCKING FUNCTION, if no character is found, it's gonna be 0xff
  //   if(c != 0xff)
  //   {
  //     str[strlen(str)] = c;
  //     printf("%c", c);
  //   }

  //   vTaskDelay(100 / portTICK_PERIOD_MS); // so the watchdog timer doesn't get upset with us sitting in an infinite loop
  // }

  // printf("You typed: %s\n", str);
  


/**************************************************/
 /******************* - Blinkey -  *******************/
/**************************************************/
  
  // #include "freertos/FreeRTOS.h"
  // #include "freertos/task.h"
  // #include "esp_system.h"
  // #include "driver/gpio.h"
  // #define PIN 2

  // gpio_pad_select_gpio(PIN);
  // gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
  
  // while (true)
  // {   
  //   gpio_set_level(PIN, 1);
  //   vTaskDelay(1000 / portTICK_PERIOD_MS); 
  //   gpio_set_level(PIN, 0);
  //   vTaskDelay(1000 / portTICK_PERIOD_MS); 
  //   printf("Hello World\n");
  // }
  
  
/**************************************************/
 /******************* - Random -  *******************/
/**************************************************/

// #define TAG "DICE"

// int dice_role()
// {
//   int random = esp_random();
//   int positiveNumber = abs(random);
//   int diceNumber = (positiveNumber % 6) + 1;
//   return diceNumber;
//}

  // int i = 0;
  // while (true)
  // {
  // vTaskDelay(1000/ portTICK_PERIOD_MS);
  // ESP_LOGI(TAG, "random number %d", dice_role());

  // }
  


/**************************************************/
 /******************* - TASK DELAY -  *******************/
/**************************************************/  
// #define TAG "DELAY"
  // int i = 0;
  // while (true)
  // {
  // vTaskDelay(1000/ portTICK_PERIOD_MS);
  // ESP_LOGI(TAG, "in loop %d", i++);

  // }
  

/**************************************************/
 /******************* - LOGS -  *******************/
/**************************************************/

  // printf("Hello world!\n");
  // esp_log_level_set("LOG", ESP_LOG_INFO);
  // ESP_LOGE("LOG", "This is an error" );
  // ESP_LOGW("LOG", "This is a warning" );
  // ESP_LOGI("LOG", "This is an info" );
  // ESP_LOGD("LOG", "This is Debug" );
  // ESP_LOGV("LOG", "This is Verbose" );

  // int number = 0 ;

  // ESP_LOGE("TAG2", "This is an error %d", number++ );

  // ESP_LOGW("TAG2", "This is a warning %d", number++ );
  // ESP_LOGI("TAG2", "This is an info %d", number++ );
  // ESP_LOGD("TAG2", "This is Debug %d", number++ );
  // ESP_LOGV("TAG2", "This is Verbose %d", number++ );
    
    
}
      