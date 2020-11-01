#ifndef __HAL_MEDIA_REGULATOR_H__
#define __HAL_MEDIA_REGULATOR_H__

void hal_media_domain_set_voltage(int step, int max_uV, int set_volt);
int hal_media_domain_get_regulator_step(int min_uV, int max_uV);
int hal_media_domain_get_voltage(int step, int max_uV);

#endif

