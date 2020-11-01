#ifndef __HAL_MEDIA_FREQ_H__
#define __HAL_MEDIA_FREQ_H__

#define MEDIA_AVS_VOLT_STEP     10000 /* mv */
#define MEDIA_AVS_HPM_DELTA_MIN 1
#define MEDIA_AVS_HPM_DELTA_MAX 6

#define MEDIA_PROF_NUM 2
#define MEDIA_TEMP_NUM 2

#define MEDIA_SVB_NUM 4

#define OTP_MDA_HPM_IDDQ 0x12032004

unsigned int hal_get_hpm_value(void);
void hal_media_set_hpm_div(unsigned int div);
unsigned hal_media_update_vmax(unsigned int hpm_mda_value);
void hal_media_hpm_init(void);
void hal_media_tsensor_init(void);
int hal_media_get_average_hpm(void);
int hal_media_get_average_temperature(void);

#endif /* End of #ifndef __HAL_MEDIA_FREQ_H__ */

