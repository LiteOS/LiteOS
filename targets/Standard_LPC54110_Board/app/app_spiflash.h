
#ifndef _APP_SPIFLASH_H_
#define _APP_SPIFLASH_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum mx25r_err {
  mx25r_err_ok = 0,
  mx25r_err_out_of_range,
  mx25r_err_alignement,
} mx25r_err_t;

typedef int (*transfer_cb_t)(void *transfer_prv, uint8_t *tx_data, uint8_t *rx_data, size_t dataSize, bool eof);

struct mx25r_instance
{
  void *prv;
  transfer_cb_t callback;
  uint8_t cmd[5];
};

#if defined(__GNUC__)
#else
__packed
#endif
struct mx25r_rdid_result
{
    char manufacturer;
    char device[2];
#if defined(__GNUC__)
} __attribute__((packed));
#else
};
#endif


#if defined(__GNUC__)
#else
__packed
#endif
struct mx25r_rdsr_result
{
    char sr0;
    char sr1;
#if defined(__GNUC__)
} __attribute__((packed));
#else
};
#endif

#if defined(__GNUC__)
#else
__packed
#endif
struct mx25r_read_result
{
    uint32_t word[4];
#if defined(__GNUC__)
} __attribute__((packed));
#else
};
#endif

union mx25r_result {
    struct mx25r_rdid_result rdid;
    struct mx25r_rdsr_result rdsr;
    struct mx25r_read_result read;
};

extern mx25r_err_t mx25r_init(struct mx25r_instance *instance, transfer_cb_t callback, void *callback_prv);
extern mx25r_err_t mx25r_cmd_rdid(struct mx25r_instance *instance, struct mx25r_rdid_result *result);
extern mx25r_err_t mx25r_cmd_read(struct mx25r_instance *instance, uint32_t address, uint8_t *buffer, uint32_t size);
extern mx25r_err_t mx25r_cmd_nop(struct mx25r_instance *instance);
extern mx25r_err_t mx25r_cmd_rdsr(struct mx25r_instance *instance, struct mx25r_rdsr_result *result);
extern mx25r_err_t mx25r_cmd_wrdi(struct mx25r_instance *instance);
extern mx25r_err_t mx25r_cmd_wren(struct mx25r_instance *instance);
extern mx25r_err_t mx25r_cmd_write(struct   mx25r_instance *instance,
                                   uint32_t address_256_align,
                                   uint8_t  *buffer,
                                   uint32_t size_256_max);
extern mx25r_err_t mx25r_cmd_sector_erase(struct mx25r_instance *instance, uint32_t address);


extern uint8_t spiflash_init(void);


#endif
