#include <cpptest.h>
#include <iostream>
#include <fstream>
#include "test_esp8266_wifi_spi.h"

int main(){
  Test::Suite ts;
  ts.add(std::auto_ptr<Test::Suite>(new Test_Esp8266_Wifi_Spi));

  std::ofstream html;
  html.open("Result.htm");
  
  Test::HtmlOutput output;
  ts.run(output);
  output.generate(html);
  html.close();
}
