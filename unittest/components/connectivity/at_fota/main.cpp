#include <cpptest.h>
#include <iostream>
#include <fstream>

// mmj
#include "test_at_fota.h"
#include "test_at_fota_hal.h"

int main()
{
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestAtFota));
	ts.add(std::auto_ptr<Test::Suite>(new TestAtFotaHal));
	
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
