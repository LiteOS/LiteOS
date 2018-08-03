#include <cpptest.h>
#include <iostream>
#include <fstream>

#include "test_fota_package_sha256.h"
#include "test_fota_package_sha256_rsa2048.h"
int main()
{
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageSha256));
	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageSha256Rsa2048));
	
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
