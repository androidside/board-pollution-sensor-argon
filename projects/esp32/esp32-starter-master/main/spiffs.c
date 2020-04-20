
#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"
#include <string.h>

#include <stdio.h>
#include <dirent.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#define TAG "SPIFFS"

void configure_spiffs()
{
  esp_vfs_spiffs_conf_t config = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true,
  };

  esp_vfs_spiffs_register(&config);

  size_t total = 0, used = 0;
  esp_spiffs_info(NULL, &total, &used);
  ESP_LOGI(TAG, "total = %d, used = %d", total, used);
}



int databaseCountLines()
{
  int numberOfLines = 0; //We start it at -1
  configure_spiffs();

  FILE *file = fopen("/spiffs/database.txt", "r");
  if (file == NULL)
  {
    ESP_LOGE(TAG, "could not open the file");
  }
  else
  {
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
      numberOfLines++;
    }

    fclose(file);
  }
  ESP_LOGI(TAG, "Number of Lines = %d", numberOfLines);
  esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!
  return numberOfLines;
}

void databaseReadJson(char *destination, int index)
{
  configure_spiffs();
  int count = 0;

  FILE *file = fopen("/spiffs/database.txt", "r");
  if (file == NULL)
  {
    ESP_LOGE(TAG, "could not open the file");
  }
  else
  {
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
      if (count == index)
      {
        strcat(destination, line);
        break;
      }
      else
      {
        count++;
      }
    }

    fclose(file);
  }
  esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!
}

void databaseAppendJson(char *newJson)
{
  configure_spiffs();
  FILE *file = fopen("/spiffs/database.txt", "a");
  if (file == NULL)
  {
    ESP_LOGE(TAG, "could not open the file");
  }
  else
  {
    fputs("\n", file);
    fputs(newJson, file);
  }
  fclose(file);
  esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!  
}

void databaseReport()
{
  char destination[256];
  memset(destination, 0, 256);
  int numberOfLines = databaseCountLines();
  if (numberOfLines > 0)
  {
    for (int index = 0; index < numberOfLines; index++)
    {
      databaseReadJson(destination, index);
      ESP_LOGI(TAG, "Index = %d, Json = %s", index, destination);
      memset(destination, 0, 256);
    }
  }
  else
  {
    ESP_LOGE(TAG, "Number of elements = %d", numberOfLines);
  }
}

// void writeDatabase(char *source)
// {
//   esp_vfs_spiffs_conf_t config = {
//       .base_path = "/spiffs",
//       .partition_label = NULL,
//       .max_files = 5,
//       .format_if_mount_failed = true,
//   };

//   esp_vfs_spiffs_register(&config);

//   size_t total = 0, used = 0;
//   esp_spiffs_info(NULL, &total, &used);
//   ESP_LOGI(TAG, "total = %d, used = %d", total, used);

//   FILE *file = fopen("/spiffs/database.txt", "r");
//   if (file == NULL)
//   {
//     ESP_LOGE(TAG, "could not open the file");
//   }
//   else
//   {
//     char line[256];
//     while (fgets(line, sizeof(line), file) != NULL)
//     {
//       printf(line);
//       strcat(source, line);
//     }

//     fclose(file);
//   }
//   esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!
// }

/**************************************************/
/******************* -  ReadWole Database, limited by destination[SIZE] *******************/
/**************************************************/

// void database_read_json(char *destination)
// {
//   configure_spiffs();

//   FILE *file = fopen("/spiffs/database.txt", "r");
//   if (file == NULL)
//   {
//     ESP_LOGE(TAG, "could not open the file");
//   }
//   else
//   {
//     char line[256];
//     while (fgets(line, sizeof(line), file) != NULL)
//     {
//       printf(line);
//       strcat(destination, line);
//     }

//     fclose(file);
//   }
//   esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!
// }

/**************************************************/
/******************* -  Maniupulating Strings *******************/
/**************************************************/

//   https://www.onlinegdb.com/online_c_compiler

// #include <stdio.h>
// #include <string.h>

// void function(char *destination){

//    char src[100];

//    strcpy(src,  "This is source");
//    strcpy(destination, "This is destination");

//    strcat(destination, src);

//    printf("Final destination string : |%s|", destination);

// }

// int main () {
//     char text[100];
//     function(text);

//    return(0);
// }

// Result
// Final destination string : |This is destinationThis is source|



/**************************************************/
/******************* -  Method to read the INTEGER at the beginning of the file *******************/
/**************************************************/

// int databaseReadCounter()
// {
//   int numberOfElements = -1; //We start it at -1
//   configure_spiffs();
//   FILE *file = fopen("/spiffs/database.txt", "r");
//   if (file == NULL)
//   {
//     ESP_LOGE(TAG, "could not open the file");
//   }
//   else
//   {
//     char line[32];
//     if (fgets(line, sizeof(line), file) != NULL)
//     {
//       printf(line);
//       if (sscanf(line, "%d", &numberOfElements) != EOF)
//       {
//         ESP_LOGI(TAG, "There are %d elements in the database.txt", numberOfElements);
//       }
//       else
//       {
//         ESP_LOGE(TAG, "First value on database.txt is not an integer !");
//       }
//     }
//     else
//     {
//       ESP_LOGE(TAG, "File is empty !");
//     }
//     fclose(file);
//   }
//   esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!
//   return numberOfElements;
// }