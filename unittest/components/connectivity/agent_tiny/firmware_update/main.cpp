#include <cpptest.h>
#include <iostream>
#include <fstream>

// mmj
#include "test_firmware_update.h"

int main()
{
    Test::Suite ts;

    ts.add(std::auto_ptr<Test::Suite>(new TestFirmwareUpdate));

    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
