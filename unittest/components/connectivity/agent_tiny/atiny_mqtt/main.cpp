#include <cpptest.h>
#include <iostream>
#include <fstream>

// zz
#include "test_hmac.h"
#include "test_flash_manager.h"
#include "test_mqtt_client.h"



int main()
{
    Test::Suite ts;

	ts.add(std::auto_ptr<Test::Suite>(new TestHmac));
	ts.add(std::auto_ptr<Test::Suite>(new TestFlashmanager));
    ts.add(std::auto_ptr<Test::Suite>(new TestMQTT_Client));   

    std::ofstream html;
    html.open("Result.htm");

    Test::HtmlOutput output;
    ts.run(output);
    output.generate(html);
    html.close();
}
