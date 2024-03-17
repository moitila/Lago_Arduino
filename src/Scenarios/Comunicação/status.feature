# language: en

Feature: Displaying System Status on HomePage
  As a Arduino
  I will receive commands from ESP 01 via Serial port on pins 10 and 11, and I will respond by forcing the data in JSON format

  Background:
    Given the ESP01 is connected to the Arduino via serial communication
    And the ESP01 web server is running

#   <MessageLog> is compound for "dd/mm/yy hh:mm:ss - Arduino Send to ESP01 - "#CMD rsp status <JSON_Data>"
<JSON_Data> = {
  "lakeLevel": 10,
  "distMinimaAguaLago": 4,
  "distMaximaAguaLago": 20,
  "filterLevel": 5,
  "distMinimaAguaFiltro": 4,
  "distMaximaAguaFiltro": 20,
  "pumpLakeStatus": true,
  "pumpFilterStatus": false
}

  Scenario: Arduino is listen commands in the ecSerial(10,11)
    When the ESP01 sends a "#CMD get status" request to Arduino
    And the Arduino receives the request
    And the Arduino validate message, to identify if a valid command
    And the Arduino assembles a JSON with the current status values including:
      | field               | example value |
      | lakeLevel           | 10            |
      | distMinimaAguaLago  | 4             |
      | distMaximaAguaLago  | 20            |
      | filterLevel         | 5             |
      | distMinimaAguaFiltro| 4             |
      | distMaximaAguaFiltro| 20            |
      | pumpLakeStatus      | true          |
      | pumpFilterStatus    | false         |
    And the Arduino sends the JSON to the ESP01 preceded by the response command "#CMD rsp status <JSON_Data>"
    And the Arduino verify if logSerialMode is on, and if is on, send the <MessageLog> to serial
    And the Arduino don´t wait for response in this command


  
