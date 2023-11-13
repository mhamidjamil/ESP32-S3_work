// Cspiffs.h
#ifndef CSPIFFS_H
#define CSPIFFS_H
#include "SPIFFS.h"
#include <Arduino.h>

#define CONFIG_FILE "/config.txt"

class Cspiffs {

private:
  bool SPI_DEBUGGING = false;

public:
  Cspiffs();

  void readSPIFFS(); // will print what's inside the file

  String getSPIFFSData(); // will return the data inside the file

  String getCompleteString(String str, String target);
  /* will return value of variable in string like:
  if it recevice this string:
  "#setting <ultra sound alerts 0> <display 1>","ultra"
  it will return "ultra sound alerts 0" */

  String getFileVariableValue(String varName);
  // will return value of variable if present in file

  String getFileVariableValue(String varName, bool createNew);
  // will return value of variable if not present then it will create new

  void updateSPIFFS(String variableName, String newValue);
  // will update variable value present in file

  void updateSPIFFS(String variableName, String newValue,
                    bool createIfNotExists);
  /* will update variable value present in file if not present then it will
   create new one */

  String getVariableName(String str, String startFrom);
  /* will return variable name from string
    it's main purpose is to fetch correct variabel name as defined in rules
  */

  String getFirstLine(String str);
  // will return full line of required string

  String fetchNumber(String str, char charToInclude);
  // will return required number from string

  void enable_SPI_debugging();                  // enable SPI debug
  void disable_SPI_debugging();                 // disable SPI debug
  void println(String tempStr);                 // actually Serail.println()
  void println(String tempStr, bool condition); // only print if it has to be
  void print(String tempStr);                   // similler to Serail.print()
};

#endif
