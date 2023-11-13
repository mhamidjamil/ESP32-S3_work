#include "Cspiffs.h"

Cspiffs::Cspiffs() {
  Serial.begin(115200);
  println("\t\t\t###___Constructer___###");
}

void Cspiffs::readSPIFFS() {
  String tempStr = "";
  if (!SPIFFS.begin(true)) {
    println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file = SPIFFS.open(CONFIG_FILE);
  if (!file) {
    println("Failed to open file for reading");
    return;
  }
  println("Reading file ...");
  println("File Content:");
  while (file.available()) {
    char character = file.read();
    tempStr += character; // Append the character to the string
  }
  println(tempStr);
  file.close();
}

String Cspiffs::getSPIFFSData() {
  String tempStr = "";
  if (!SPIFFS.begin(true)) {
    println("An Error has occurred while mounting SPIFFS");
    return tempStr;
  }
  File file = SPIFFS.open(CONFIG_FILE);
  if (!file) {
    println("Failed to open file for reading");
    return tempStr;
  }
  while (file.available()) {
    char character = file.read();
    tempStr += character; // Append the character to the string
  }
  file.close();
  return tempStr;
}

String Cspiffs::getCompleteString(String str, String target) {
  if (str.length() < 1)
    return "";
  String tempStr = "";
  int i = str.indexOf(target);
  if (i == -1) {
    println("String is empty" + str);
    return tempStr;
  } else if (str.length() <= 20) { // to avoid spiffs on terminal
    println("@--> Working on : " + str + " finding : " + target);
  }
  for (; i < str.length(); i++) {
    if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z') ||
        (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == ' ') ||
        (str[i] == '.') && (str[i] != '>') && (str[i] != '\n') ||
        (str[i] == ':') || (str[i] == '=') || (str[i] == '_'))
      tempStr += str[i];
    else {
      println("Returning (complete string) 1: " + tempStr, SPI_DEBUGGING);
      return tempStr;
    }
  }
  println("Returning (complete string) 2: " + tempStr, SPI_DEBUGGING);
  return tempStr;
}

String Cspiffs::getFileVariableValue(String varName) {
  return getFileVariableValue(varName, false);
}

String Cspiffs::getFileVariableValue(String varName, bool createNew) {
  String targetLine = getCompleteString(getSPIFFSData(), varName);
  if (targetLine.length() < 1)
    return "";
  println("Trying to fetch data from line : " + targetLine, SPI_DEBUGGING);
  if (targetLine.indexOf(varName) == -1 && !createNew) {
    println("Variable not found in file returning -1");
    return "-1";
  } else if (targetLine.indexOf(varName) == -1 && createNew) {
    println("Variable not found in file creating new & returning 0");
    updateSPIFFS(varName, "0");
    return "0";
  }
  String targetValue = targetLine.substring(varName.length(), -1);
  String variableValue = fetchNumber(targetValue, '.');
  println("Returning value of {" + varName + "} : " + variableValue,
          SPI_DEBUGGING);
  return variableValue;
}

void Cspiffs::updateSPIFFS(String variableName, String newValue) {
  updateSPIFFS(variableName, newValue, false);
}

void Cspiffs::updateSPIFFS(String variableName, String newValue,
                           bool createIfNotExists) {
  if (!SPIFFS.begin()) {
    println("Failed to mount file system");
    return;
  }

  String previousValue = getFileVariableValue(variableName);
  println("@ Replacing previous value : ->" + previousValue +
          "<- with new value : ->" + newValue + "<-");
  // Open the file for reading
  File file = SPIFFS.open(CONFIG_FILE, "r");
  if (!file) {
    println("Failed to open file for reading");
    return;
  }

  // Create a temporary buffer to store the modified content
  String updatedContent = "";
  bool valueReplaced = false;
  // Read and modify the content line by line
  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (line.indexOf(variableName) != -1) {
      line.replace(previousValue, newValue);
      valueReplaced = true;
    }
    updatedContent += line + "\n";
  }

  // Close the file
  file.close();

  println("Updated content : " + updatedContent);

  // Reopen the file for writing, which will erase the previous content
  file = SPIFFS.open(CONFIG_FILE, "w");
  if (!file) {
    println("Failed to open file for writing");
    return;
  }

  if (!valueReplaced) {
    println("Variable not found in file");
    if (createIfNotExists) {
      println(" creating new variable");
      updatedContent += ("\n" + variableName + ": " + newValue);
    }
  }

  // Write the modified content back to the file
  file.print(updatedContent);
  file.close();
  valueReplaced ? println("Variable {" + variableName + "} has been updated.")
  : createIfNotExists
      ? println("Variable {" + variableName + "} has been created.")
      : println(
            "Variable {" + variableName +
            "} not found in file and creating new variable is not allowed.");
}

String Cspiffs::getVariableName(String str, String startFrom) {
  String newStr = "";
  str = getFirstLine(str);
  bool found = false;
  for (int i = str.indexOf(startFrom) + startFrom.length(); i < str.length();
       i++) {
    if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') ||
        (found && str[i] >= '0' && str[i] <= '9') || (str[i] == '_')) {
      newStr += str[i];
      found = true;
    } else if (found) {
      break;
    }
  }
  return newStr;
}

String Cspiffs::getFirstLine(String str) {
  String newStr = "";
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '\n' || str[i] != '\r')
      newStr += str[i];
    else
      break;
  }
  return newStr;
}

String Cspiffs::fetchNumber(String str, char charToInclude) {
  String number = "";
  bool numberStart = false;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] >= '0' && str[i] <= '9' ||
        (str[i] == charToInclude && numberStart)) {
      number += str[i];
      numberStart = true;
    } else if (numberStart) {
      return number;
    }
  }
  return number;
}

void Cspiffs::enable_SPI_debugging() { SPI_DEBUGGING = true; }

void Cspiffs::disable_SPI_debugging() { SPI_DEBUGGING = false; }

void Cspiffs::println(String tempStr) { Serial.println(tempStr); }

void Cspiffs::println(String tempStr, bool condition) {
  condition ? Serial.println(tempStr) : void();
  // TODO: try to implement logger logic in place of void()
}

void Cspiffs::print(String tempStr) { Serial.print(tempStr); }