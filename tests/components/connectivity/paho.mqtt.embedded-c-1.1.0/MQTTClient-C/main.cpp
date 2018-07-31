#include <cpptest.h>
#include <iostream>
#include <fstream>

// mmj
#include "test_MQTTClient.h"

// wsy
#include "test_MQTTliteos.h"

int main()
{
    Test::Suite ts;

    ts.add(std::auto_ptr<Test::Suite>(new TestMQTTClient)); // mce18
	ts.add(std::auto_ptr<Test::Suite>(new TestMQTTLiteos));

    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
