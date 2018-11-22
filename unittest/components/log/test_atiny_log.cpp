#include "test_atiny_log.h"
#include "log/atiny_log.h"
#include "string.h"
extern "C"{

}
void TestAtinyLog::test_atiny_set_log_level(void)
{
	atiny_log_e level = LOG_INFO;
	atiny_set_log_level(level);
}
void TestAtinyLog::test_atiny_get_log_level(void)
{
	atiny_get_log_level();
}
void TestAtinyLog::test_atiny_get_log_level_name(void)
{
	char levelname[20] ;
	atiny_log_e level = LOG_INFO;
	strcpy(levelname,atiny_get_log_level_name(level));
	level = 10; //10>5
	strcpy(levelname,atiny_get_log_level_name(level));
}
TestAtinyLog::TestAtinyLog()
{
	TEST_ADD(TestAtinyLog::test_atiny_set_log_level);
	TEST_ADD(TestAtinyLog::test_atiny_get_log_level);
	TEST_ADD(TestAtinyLog::test_atiny_get_log_level_name);
}
TestAtinyLog::~TestAtinyLog()
{
	
}
void TestAtinyLog::setup(){
	std::cout << "setup";
}
void TestAtinyLog::tear_down(){
    std::cout << "tear_down";
}
