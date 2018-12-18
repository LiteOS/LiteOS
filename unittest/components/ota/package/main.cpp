
#include <cpptest.h>
#include <iostream>
#include <fstream>

#include "test_package_writer.h"
#include "test_package_checksum.h"
#include "test_package.h"
#include "test_package_head.h"
#include "opt/test_package_sha256.h"
#include "opt/test_package_sha256_rsa2048.h"

int main()
{
    Test::Suite ts;
#if 0
#endif
	ts.add(std::auto_ptr<Test::Suite>(new TestPackage) );
	ts.add(std::auto_ptr<Test::Suite>(new TestPackageChecksum) );
	ts.add(std::auto_ptr<Test::Suite>(new TestPackageHead) );
	ts.add(std::auto_ptr<Test::Suite>(new TestPackageWrite) );
	ts.add(std::auto_ptr<Test::Suite>(new TestPackageSha256) );
	ts.add(std::auto_ptr<Test::Suite>(new TestPackageSha256Rsa2048) );
#if 0
#endif
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
    return 0;
}
