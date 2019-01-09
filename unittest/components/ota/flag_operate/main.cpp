#include <cpptest.h>
#include <iostream>
#include <fstream>

#include "test_flag_manager.h"
#include "test_upgrade_flag.h"
int main()
{
    
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestFlagManager));
	ts.add(std::auto_ptr<Test::Suite>(new TestUpgradeFlag));
	
    std::ofstream html;
    html.open("Result.htm");
	
    Test::HtmlOutput output;
    printf("i am here1\n");
    ts.run(output);
    
    output.generate(html);
	
    html.close();
	
}
