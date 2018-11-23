#include <iostream>
#include <fstream>
#include <cpptest.h>

#include "test_fatfs.h"
#include "test_spiffs.h"
#include "test_jffs2.h"


int main()
{
    try
    {
        Test::Suite ts;
        ts.add(std::auto_ptr<Test::Suite>(new TestFatfs));
        ts.add(std::auto_ptr<Test::Suite>(new TestSpiffs));
        ts.add(std::auto_ptr<Test::Suite>(new TestJffs2));

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
