#include <cpptest.h>
#include <iostream>
#include <fstream>

// wsy
#include "test_object_device.h"
#include "test_object_firmware.h"
#include "test_connection.h"
#include "test_object_connectivity_stat.h"
#include "test_atiny_fota_manager.h"
#include "test_atiny_fota_state.h"

// mmj
#include "test_agenttiny.h"
//#include "test_atiny_log.h"
#include "test_atiny_rpt.h"
#include "test_object_location.h"
#include "test_object_security.h"
#include "test_object_server.h"
#include "test_object_binary_app_data_container.h"
#include "test_object_connectivity_moni.h"
#include "test_firmware_update.h"
#include "test_util_timer.h"

// zk
#include "test_object_access_control.h"
//#include "pack_storage_device_api_s.h"
#include "test_platform_adapter.h"

int main()
{
    Test::Suite ts;
#if 0
#endif
    ts.add(std::auto_ptr<Test::Suite>(new TestAgenttiny));  
    ts.add(std::auto_ptr<Test::Suite>(new TestConnection));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectDevice));     // mce20
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectFirmware));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectObjectConnectivityStat));
    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyFotaManager));
    ts.add(std::auto_ptr<Test::Suite>(new TestFirmwareUpdate));	
    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyFotaState));
    ts.add(std::auto_ptr<Test::Suite>(new TestAtinyRpt));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectLocation));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectSecurity));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectServer));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectBinaryAppDataContainer));
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectConnectivityMoni));
    ts.add(std::auto_ptr<Test::Suite>(new Test_UTIL_TIMER));	
    ts.add(std::auto_ptr<Test::Suite>(new TestObjectAccessControl));
    ts.add(std::auto_ptr<Test::Suite>(new Test_Platform_Adapter));
#if 0
#endif
    //ts.add(std::auto_ptr<Test::Suite>(new TestAtinyLog));
    //ts.add(std::auto_ptr<Test::Suite>(new TestAtinyFotaManager));   //mce7
    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
