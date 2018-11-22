#ifndef _TEST_ATINY_LOG_H_
#define _TEST_ATINY_LOG_H_
#endif
#include <cpptest.h>
#include "stub.h"

class TestAtinyLog:public Test::Suite{
	public:
		void test_atiny_set_log_level(void);
		void test_atiny_get_log_level(void);
		void test_atiny_get_log_level_name(void);
		
		TestAtinyLog();
		~TestAtinyLog();
	protected:
		void tear_down();
		void setup();
	
};
