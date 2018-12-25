#include <cpptest.h>
#include <iostream>
#include <fstream>


// wsy
#include "test_MQTTliteos.h"

int main()
{
    Test::Suite ts;

    ts.add(std::auto_ptr<Test::Suite>(new TestMQTTLiteos));

    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
