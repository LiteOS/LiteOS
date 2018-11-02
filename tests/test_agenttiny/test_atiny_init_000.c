#include "test_agenttiny.h"

void test_atiny_init_000(void **state)
{
	void *phandle;
	atiny_param_t uwAtiny_params;
	memset(&uwAtiny_params, 0, sizeof(atiny_param_t));

	int ret = 0;

	ret = atiny_init(NULL, NULL);
	assert_int_equal(ret, ATINY_ARG_INVALID);

	ret = atiny_init(&uwAtiny_params, NULL);
	assert_int_equal(ret, ATINY_ARG_INVALID);

	ret = atiny_init(NULL, &phandle);
	assert_int_equal(ret, ATINY_ARG_INVALID);

}

