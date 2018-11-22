#include <cpptest.h>
#include <iostream>
#include <fstream>
#include "test_bc95.h"

int main(){
  Test::Suite ts;
  ts.add(std::auto_ptr<Test::Suite>(new TestBC95));

  std::ofstream html;
  html.open("Result.htm");
  
  Test::HtmlOutput output;
  ts.run(output);
  output.generate(html);
  html.close();
}
