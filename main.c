#include <stdio.h>    // standard I/O library
#include <stdlib.h>   // malloc, calloc, realloc
#include <time.h>     // getdate()
#include <string.h>   // strcpy
#include <math.h>     // round

//  TITLE         :   SIMULATED MODULAR DIGITAL SENSOR LOGGER
//  AUTHOR        :   ichimaru001 
//  DATE          :   09/06/25
//  DESCRIPTION   :   SIMULATES READING DATA FROM DIFFERENT DIGITAL SENSORS 
//                    (TEMP, PRESSURE, LIGHT, ETC.)

// ** FUNCTION PROTOTYPES
void getTimeAndDate();
int checkLogCountCapacity();

// ** GLOBAL INITIALIZATION
typedef enum {
  TEMP_SENSOR,
  PRESSURE_SENSOR,
  LIGHT_SENSOR,
  HUMIDITY_SENSOR,
  PROXIMITY_SENSOR,
  GAS_SENSOR,
  NUM_SENSOR_TYPES
} sensorType;
const char *sensorNames[] = {
  "Temperature Sensor",
  "Pressure Sensor",
  "Light Sensor",
  "Humidity Sensor",
  "Proximity Sensor",
  "Gas Sensor",
};
typedef struct {
  int id;
  sensorType type;
  float value;
  char timestamp[20];
} sensorLog;

int main() {
  // ** INITIALIZATION
  char menuOptions[][25] = {
    "1. Add new sensor log",
    "2. View sensor logs",
    "3. Save logs to file",
    "4. Load logs from file",
    "5. Exit" };
  srand(time(NULL));

  // ** USER INPUT
  int userChoice = 0;

  // ** LOGS INITIALIZATION
  int logsCapacity = 2;
  int logCount = 0;
  sensorLog *logs = malloc(logsCapacity * sizeof(sensorLog));
  // * CHECK FOR MEMORY ALLOCATION FAILURE
  if (logs == NULL) {
    printf("Memory allocation failed!\n");
    return 1;
  }
  // *

  do
  {
    // ** SYSTEM  : INTRODUCTION & MENU
    printf("\nSIMULATED MODULAR DIGITAL SENSOR LOGGER\n");
    for (int i = 0; i < sizeof(menuOptions) / sizeof(menuOptions[0]);i++) {
      printf("%s\n", menuOptions[i]);
    }
    printf("Enter your choice: ");
    scanf(" %d", &userChoice);

    // ** USER  :   ADD NEW SENSOR LOG 
    if (userChoice == 1) {
      sensorLog newLog;
      int userSensorChoice = 0;
      float userSensorValue = 0.0f;

      checkLogCountCapacity(logCount, &logsCapacity, &logs);

      // * RANDOM ID
      int logID = (rand() % 10000) + 1;
      newLog.id = logID;
      // *

      // * CHOOSE SENSOR TYPE
      printf("\n");
      for (int i = 1; i <= NUM_SENSOR_TYPES; i++)
      {
        printf("%d. %s\n", i, sensorNames[i - 1]);
      }
      printf("Enter your choice: ");
      scanf(" %d", &userSensorChoice);
      newLog.type = userSensorChoice - 1;
      // *

      // * USER ENTER VALUE
      printf("Enter a value for this sensor: ");
      scanf(" %f", &userSensorValue);
      newLog.value = userSensorValue;
      // *

      // * GET TIMESTAMP
      char timeBuffer[20];
      getTimeAndDate(timeBuffer);
      strcpy(newLog.timestamp, timeBuffer);
      // *

      // * INCREMENT LOG COUNT AND PRINT LOG
      logs[logCount] = newLog;
      printf("\nID: %d\nTYPE: %s\nVALUE: %.2f\nTIMESTAMP: %s\n", logs[logCount].id, sensorNames[logs[logCount].type], logs[logCount].value, logs[logCount].timestamp);
      printf("\nSuccessfully added log with ID %d!\n", logs[logCount].id);
      logCount++;
      // *
    }
    // ** USER  :   VIEW ALL LOGS 
    if (userChoice == 2) {
      for (int i = 0; i < logCount; i++)
      {
        printf("\nID: %d\nTYPE: %s\nVALUE: %.2f\nTIMESTAMP: %s,\n", logs[i].id, sensorNames[logs[i].type], logs[i].value, logs[i].timestamp);
      }
      printf("\nSuccessfully loaded all logs!\n");
    }
    // ** USER  :   SAVE LOGS TO FILE 
    if (userChoice == 3) {
      // ** WRITE TO FILE "saved-logs.txt"
      FILE *pFile = fopen("saved-logs.txt", "w");

      // * CHECK FOR ERROR
      if (pFile == NULL) {
        printf("Error opening file\n");
        return 1;
      }
      // *

      for (int i = 0; i < logCount; i++)
      {
        fprintf(pFile, "ID: %d\nTYPE: %s\nVALUE: %.2f\nTIMESTAMP: %s\n", logs[i].id, sensorNames[logs[i].type], logs[i].value, logs[i].timestamp);
      }

      fclose(pFile);
      // **

      printf("\nSuccessfully saved logs to file!\n");
    }
    // ** USER  :   WRITE LOGS FROM FILE TO PROGRAM
    if (userChoice == 4) {
      // * READ FROM FILE "saved-logs.txt"
      FILE *pFile = fopen("saved-logs.txt", "r");
      char buffer[4096] = { 0 };

      // * CHECK FOR ERROR
      if (pFile == NULL) {
        printf("Error opening file\n");
        return 1;
      }
      // *

      // ** INITIALIZATION OF BUFFERS
      char idBuffer[50];
      char typeBuffer[50];
      char valueBuffer[50];
      char timestampBuffer[50];
      // **

      // * INITIALIZE LOG COUNT
      logCount = 0;

      while (
        (fgets(idBuffer, sizeof(idBuffer), pFile)) &&
        (fgets(typeBuffer, sizeof(typeBuffer), pFile)) &&
        (fgets(valueBuffer, sizeof(valueBuffer), pFile)) &&
        (fgets(timestampBuffer, sizeof(timestampBuffer), pFile))
        != NULL) {

        // REMOVES '\n' from end of each log
        timestampBuffer[strcspn(timestampBuffer, "\n")] = '\0';

        int id;
        char type[30];
        float value;
        char timestamp[30];

        // * PARSING DATA
        sscanf(idBuffer, "ID: %d", &id);
        sscanf(typeBuffer, "TYPE: %[^\n]", type);
        sscanf(valueBuffer, "VALUE: %f", &value);
        sscanf(timestampBuffer, "TIMESTAMP: %[^\n]", timestamp);
        // *

        printf("\nID: %d\nTYPE: %s\nVALUE: %.2f\nTIMESTAMP: %s\n", id, type, value, timestamp);

        // * CREATING NEW LOG
        sensorLog newLog;
        newLog.id = id;
        for (int i = 0;i < NUM_SENSOR_TYPES;i++) {
          if (strcmp(type, sensorNames[i]) == 0) {
            newLog.type = i;
            break;
          }
        }
        newLog.value = value;
        strcpy(newLog.timestamp, timestamp);
        // *

        // * ESTABLISHING NEW LOG
        logs[logCount] = newLog;
        logCount++;
        // *

        checkLogCountCapacity(logCount, &logsCapacity, &logs);
      }


      fclose(pFile);
      // **
    }
  } while (userChoice != 5);


  free(logs);
  return 0;
}

int checkLogCountCapacity(int logCount, int *logsCapacity, sensorLog **logs) {
  // ** CHECK LOG COUNT AND LOG CAPACITY
  if (logCount >= *logsCapacity) {
    printf("\nLog capacity has been reached!\nMemory expanded!\n");
    *logsCapacity *= 2;
    *logs = realloc(*logs, *logsCapacity * sizeof(sensorLog));
    // ** CHECK FOR MEMORY ALLOCATION FAILURE
    if (*logs == NULL) {
      printf("Memory allocation failed!\n");
      return 1;
    }
    // **
  }
  // **
};
void getTimeAndDate(char *buffer) {
  // **INITIALIZATION TIME
  time_t rawtime = 0; // Jan 1 1970 (Epoch)
  struct tm *pTime = NULL;

  time(&rawtime);
  pTime = localtime(&rawtime);
  // **

  sprintf(buffer,
    "%02d-%02d-%02d %02d:%02d:%02d",
    pTime->tm_mday,
    (pTime->tm_mon) + 1,
    (pTime->tm_year) + 1900,
    pTime->tm_hour,
    pTime->tm_min,
    pTime->tm_sec);
};