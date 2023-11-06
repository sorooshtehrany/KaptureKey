#pragma once

#include "..\Include\stm8_eval_spi_sd.h"

int sdcard_init(void);
int sdcard_open_file(const char*);
int sdcard_read_bytes(u8*, u32);
int sdcard_write_bytes(u8*, u16);
int sdcard_set_file_pointer(unsigned long);
unsigned long int sdcard_get_wave_size(const char*);
unsigned long int sdcard_get_physical_address(const char*);