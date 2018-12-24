#include <iostream>
#include <fstream>
#include <cpptest.h>

#include "test_fatfs.h"
#include "test_spiffs.h"


int main()
{
    try
    {
        Test::Suite ts;
        ts.add(std::auto_ptr<Test::Suite>(new TestFatfs));
        ts.add(std::auto_ptr<Test::Suite>(new TestSpiffs));

        std::ofstream html;
        html.open("Result.htm");

        printf("********** main **********\n");
        Test::HtmlOutput output;
        ts.run(output);
        output.generate(html);
        html.close();
        printf("********** main **********\n");
    }
    catch (...)
    {
        printf("unexpected exception encountered\n");
        return 1;
    }

    return 0;
}
