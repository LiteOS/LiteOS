#include <cpptest.h>
#include <iostream>
#include <fstream>

#include "test_otacrc.h"
int main()
{
    printf("进入main函数\n");
    Test::Suite ts;

//    ts.add(std::auto_ptr<Test::Suite>(new TestConnection));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectDevice));     // mce20
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectFirmware));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectObjectConnectivityStat));
//    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyUpdateInfo));
//    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyFotaState));

//    ts.add(std::auto_ptr<Test::Suite>(new TestAgenttiny));  // mce41
//    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyLog));
//    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyRpt));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectLocation));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectSecurity));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectServer));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectBinaryAppDataContainer));
//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectConnectivityMoni));

//    ts.add(std::auto_ptr<Test::Suite>(new TestObjectAccessControl));
//    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyFotaManager));   //mce7
 
	ts.add(std::auto_ptr<Test::Suite>(new TestOtaCRC));
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
