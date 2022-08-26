/*
 * Copyright (C) 2010 - 2018 Novatek, Inc.
 * Copyright (C) 2020 XiaoMi, Inc.
 * $Revision: 43560 $
 * $Date: 2019-04-19 11:34:19 +0800 (週五, 19 四月 2019) $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */
#ifndef	_LINUX_NVT_TOUCH_H
#define	_LINUX_NVT_TOUCH_H

#include <linux/delay.h>
#include <linux/input.h>
#include <linux/of.h>
#include <linux/spi/spi.h>
#include <linux/uaccess.h>
#include <linux/regulator/consumer.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include "nt36xxx_mem_map.h"

#define NVT_DEBUG 1

//---GPIO number---
#define NVTTOUCH_RST_PIN 87
#define NVTTOUCH_INT_PIN 88


//---INT trigger mode---
//#define IRQ_TYPE_EDGE_RISING 1
//#define IRQ_TYPE_EDGE_FALLING 2
#define INT_TRIGGER_TYPE IRQ_TYPE_EDGE_RISING


//---SPI driver info.---
#define NVT_SPI_NAME "NVT-ts"

#if NVT_DEBUG
#define NVT_LOG(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)
#else
#define NVT_LOG(fmt, args...)    pr_info("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)
#endif
#define NVT_ERR(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)

//---Input device info.---
#define NVT_TS_NAME "NVTCapacitiveTouchScreen"


//---Touch info.---
#define TOUCH_DEFAULT_MAX_WIDTH 1080
#define TOUCH_DEFAULT_MAX_HEIGHT 2400
#define TOUCH_MAX_FINGER_NUM 10
#define TOUCH_KEY_NUM 0
#if TOUCH_KEY_NUM > 0
extern const uint16_t touch_key_array[TOUCH_KEY_NUM];
#endif
#define TOUCH_FORCE_NUM 1000

/* Enable only when module have tp reset pin and connected to host */
#define NVT_TOUCH_SUPPORT_HW_RST 1

//---Customerized func.---
#define NVT_TOUCH_PROC 1
#define NVT_TOUCH_EXT_PROC 1
#define MT_PROTOCOL_B 1
#define WAKEUP_GESTURE 1
#if WAKEUP_GESTURE
extern const uint16_t gesture_key_array[];
#endif
#define BOOT_UPDATE_FIRMWARE 1
#define FIRMWARE_NAME_LEN    256
#define BOOT_UPDATE_FIRMWARE_NAME         "novatek_ts_fw.bin"
#define BOOT_UPDATE_TIANMA_FIRMWARE_NAME  "novatek_ts_tianma_fw.bin"
#define BOOT_UPDATE_HUAXING_FIRMWARE_NAME    "novatek_ts_huaxing_fw.bin"
#define MP_UPDATE_FIRMWARE_NAME           "novatek_ts_mp.bin"
#define MP_UPDATE_TIANMA_FIRMWARE_NAME    "novatek_ts_tianma_mp.bin"
#define MP_UPDATE_HUAXING_FIRMWARE_NAME      "novatek_ts_huaxing_mp.bin"

//---ESD Protect.---
#define NVT_TOUCH_ESD_PROTECT 0
#define NVT_TOUCH_ESD_CHECK_PERIOD 1500	/* ms */
#define NVT_TOUCH_WDT_RECOVERY 0
#define NVT_TOUCH_ESD_DISP_RECOVERY 0

//enable 'check touch vendor' feature
#define CHECK_TOUCH_VENDOR

//---Touch Vendor ID---
#define TP_VENDOR_UNKNOWN   0x00
#define TP_VENDOR_HUAXING   0x01
#define TP_VENDOR_TIANMA    0x02

/* 2019.12.16 longcheer taocheng add (xiaomi game mode) start */
#define NVT_REG_MONITOR_MODE                0x7000
#define NVT_REG_THDIFF                      0x7100
#define NVT_REG_SENSIVITY                   0x7200
#define NVT_REG_EDGE_FILTER_LEVEL           0xBA00
#define NVT_REG_EDGE_FILTER_ORIENTATION     0xBC00
/* 2019.12.16 longcheer taocheng add (xiaomi game mode) end */

//new qcom platform use
#define _MSM_DRM_NOTIFY_H_

struct nvt_ts_data {
	struct spi_device *client;
	struct input_dev *input_dev;
	struct delayed_work nvt_fwu_work;
	uint16_t addr;
	int8_t phys[32];
#if defined(CONFIG_FB)
	struct workqueue_struct *workqueue;
	struct work_struct resume_work;
#ifdef _MSM_DRM_NOTIFY_H_
	struct notifier_block drm_notif;
#else
	struct notifier_block fb_notif;
#endif
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
#ifdef CHECK_TOUCH_VENDOR
	uint8_t touch_vendor_id;
#endif
	uint8_t boot_update_firmware_name[FIRMWARE_NAME_LEN];
	uint8_t mp_update_firmware_name[FIRMWARE_NAME_LEN];
	uint8_t fw_ver;
	uint8_t x_num;
	uint8_t y_num;
	uint16_t abs_x_max;
	uint16_t abs_y_max;
	uint8_t max_touch_num;
	uint8_t max_button_num;
	uint32_t int_trigger_type;
	int32_t irq_gpio;
	uint32_t irq_flags;
	int32_t reset_gpio;
	uint32_t reset_flags;
	struct mutex lock;
	const struct nvt_ts_mem_map *mmap;
	uint8_t carrier_system;
	uint8_t hw_crc;
	uint16_t nvt_pid;
	uint8_t rbuf[1025];
	uint8_t *xbuf;
	struct mutex xbuf_lock;
	bool irq_enabled;
#if WAKEUP_GESTURE
	bool delay_gesture;
	bool is_gesture_mode;
#ifdef CONFIG_PM
	bool dev_pm_suspend;
	struct completion dev_pm_suspend_completion;
#endif
	struct regulator *pwr_vdd; /* IOVCC 1.8V */
	struct regulator *pwr_lab; /* VSP +5V */
	struct regulator *pwr_ibb; /* VSN -5V */
#endif

	struct mutex reg_lock;
	struct device *nvt_touch_dev;
	struct class *nvt_tp_class;
/*2019.12.16 longcheer taocheng add (xiaomi game mode) end*/
};

#if NVT_TOUCH_PROC
struct nvt_flash_data{
	rwlock_t lock;
};
#endif

typedef enum {
	RESET_STATE_INIT = 0xA0,// IC reset
	RESET_STATE_REK,		// ReK baseline
	RESET_STATE_REK_FINISH,	// baseline is ready
	RESET_STATE_NORMAL_RUN,	// normal run
	RESET_STATE_MAX  = 0xAF
} RST_COMPLETE_STATE;

typedef enum {
    EVENT_MAP_HOST_CMD                      = 0x50,
    EVENT_MAP_HANDSHAKING_or_SUB_CMD_BYTE   = 0x51,
    EVENT_MAP_RESET_COMPLETE                = 0x60,
    EVENT_MAP_FWINFO                        = 0x78,
    EVENT_MAP_PROJECTID                     = 0x9A,
} SPI_EVENT_MAP;

//---SPI READ/WRITE---
#define SPI_WRITE_MASK(a)	(a | 0x80)
#define SPI_READ_MASK(a)	(a & 0x7F)

#define DUMMY_BYTES (1)
#define NVT_TRANSFER_LEN	(63*1024)

typedef enum {
	NVTWRITE = 0,
	NVTREAD  = 1
} NVT_SPI_RW;

#if NVT_TOUCH_ESD_DISP_RECOVERY
#define ILM_CRC_FLAG        0x01
#define CRC_DONE            0x04
#define F2C_RW_READ         0x00
#define F2C_RW_WRITE        0x01
#define BIT_F2C_EN          0
#define BIT_F2C_RW          1
#define BIT_CPU_IF_ADDR_INC 2
#define BIT_CPU_POLLING_EN  5
#define FFM2CPU_CTL         0x3F280
#define F2C_LENGTH          0x3F283
#define CPU_IF_ADDR         0x3F284
#define FFM_ADDR            0x3F286
#define CP_TP_CPU_REQ       0x3F291
#define TOUCH_DATA_ADDR     0x20000
#define DISP_OFF_ADDR       0x2800
#endif /* NVT_TOUCH_ESD_DISP_RECOVERY */

//---extern structures---
extern struct nvt_ts_data *ts;

//---extern functions---
int32_t CTP_SPI_READ(struct spi_device *client, uint8_t *buf, uint16_t len);
int32_t CTP_SPI_WRITE(struct spi_device *client, uint8_t *buf, uint16_t len);
void nvt_bootloader_reset(void);
void nvt_eng_reset(void);
void nvt_sw_reset(void);
void nvt_sw_reset_idle(void);
void nvt_boot_ready(void);
void nvt_bld_crc_enable(void);
void nvt_fw_crc_enable(void);
int32_t nvt_update_firmware(char *firmware_name);
int32_t nvt_check_fw_reset_state(RST_COMPLETE_STATE check_reset_state);
int32_t nvt_get_fw_info(void);
int32_t nvt_clear_fw_status(void);
int32_t nvt_check_fw_status(void);
int32_t nvt_set_page(uint32_t addr);
int32_t nvt_write_addr(uint32_t addr, uint8_t data);
#if NVT_TOUCH_ESD_PROTECT
extern void nvt_esd_check_enable(uint8_t enable);
#endif /* #if NVT_TOUCH_ESD_PROTECT */

#endif /* _LINUX_NVT_TOUCH_H */
