#include <cpptest.h>
#include <iostream>
#include <fstream>

// mmj
#include "test_sota.h"
#include "test_sota_hal.h"

int main()
{
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestSota));
	ts.add(std::auto_ptr<Test::Suite>(new TestSotaHal));
	
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
