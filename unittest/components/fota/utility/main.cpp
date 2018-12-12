#include <cpptest.h>
#include <iostream>
#include <fstream>

//#include "test_fota_firmware_writer.h"
//#include "test_fota_package_checksum.h"
//#include "test_fota_package_head.h"
#include "test_ota_crc.h"
#include "test_ota_sha256.h"
int main()
{
        printf("Hello world!\n");
    Test::Suite ts;

ts.add(std::auto_ptr<Test::Suite>(new TestOtaCrc));
ts.add(std::auto_ptr<Test::Suite>(new TestOtaSha256));
	
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
