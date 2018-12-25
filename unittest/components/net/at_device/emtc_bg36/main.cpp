#include <cpptest.h>
#include <iostream>
#include <fstream>
#include "test_emtc_bg36.h"

int main(){
  Test::Suite ts;
  ts.add(std::auto_ptr<Test::Suite>(new Test_EMTC_BG36));

  std::ofstream html;
  html.open("Result.htm");
  
  Test::HtmlOutput output;
  ts.run(output);
  output.generate(html);
  html.close();
  //cout<<"test emtc bg36 over"<<endl;
  printf("test emtc bg36 over\n");
}
