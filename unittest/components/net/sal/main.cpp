#include <cpptest.h>
#include <iostream>
#include <fstream>

// zz
#include "test_atiny_socket.h"


int main()
{
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestAtiny_Socket));

    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
