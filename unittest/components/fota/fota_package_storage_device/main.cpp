#include <cpptest.h>
#include <iostream>
#include <fstream>

#include "test_fota_firmware_writer.h"
#include "test_fota_package_checksum.h"
#include "test_fota_package_head.h"
#include "test_fota_package_storage_device.h"
#include "option/test_fota_package_sha256.h"
#include "option/test_fota_package_sha256_rsa2048.h"
int main()
{
    Test::Suite ts;


	ts.add(std::auto_ptr<Test::Suite>(new TestFotaFirmwareWrite));
	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageCheckSum));
	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageHead));
	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageStorageDivice));
	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageSha256));
	ts.add(std::auto_ptr<Test::Suite>(new TestFotaPackageSha256Rsa2048));
	
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
