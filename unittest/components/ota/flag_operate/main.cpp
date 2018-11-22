#include <cpptest.h>
#include <iostream>
#include <fstream>

#include "test_flag_manager.h"
#include "test_upgrade_flag.h"
int main()
{
    printf("进入main函数\n");
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestFlagManager));
	ts.add(std::auto_ptr<Test::Suite>(new TestUpgradeFlag));
	
    std::ofstream html;
    html.open("Result.htm");
	printf("打开html文件 \n");
    Test::HtmlOutput output;
    printf("定义了 output\n");
    ts.run(output);
    printf("运行测试，将结果保存到output \n");
    output.generate(html);
	printf("将output结果保存到html中 \n");
    html.close();
	printf("关闭html文件 \n");
}
