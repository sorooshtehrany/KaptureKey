#include <string.h>
#include "..\Include\sdcard.h"
#include "..\Include\sdcard_low.h"
#include "..\Include\sdcard_fat.h"

static FATFS fs;

int 
sdcard_init(void) {
    MSD_CardInfo info;
    FRESULT res = (FRESULT) 0;
    
    MSD_DeInit();
    MSD_Init();
    MSD_GetCardInfo(&info);
    
    res = pf_mount(&fs);
    return res;
}

int
sdcard_open_file(const char *name) {
    FRESULT res = (FRESULT) 0;
    
    res = pf_open(name);
    
    return res;
}

int 
sdcard_read_bytes(u8 *buffer, u32 size) {
    FRESULT res = (FRESULT) 0;
    u16 read_bytes = 0;
    
    res = pf_read(buffer, size, &read_bytes);
  
    return res;
}

int
sdcard_write_bytes(u8 *buffer, u16 size) {
    FRESULT res = (FRESULT) 0;
    u16 write_bytes = 0;
    
    res = pf_write(buffer, size, &write_bytes);
  
    return res;
}

int
sdcard_set_file_pointer(unsigned long size) {
    FRESULT res = (FRESULT) 0;
    
    res = pf_lseek(size);
  
    return res;
}

unsigned long int 
sdcard_get_wave_size(const char *name) {
    char i;
    unsigned char buffSize[9]={0};
    unsigned long int size=0;
    u16 br = 0;

    pf_open(name);
    if (pf_read(buffSize, 9, &br) == FR_OK) {
        for ( i=7;i>=4;--i ) {
            size += buffSize[i];   
            if ( i!=4 ) 
                size = size << 8;
        } 
    }
    return size;
}

unsigned long int
sdcard_get_physical_address(const char *name) {
    u8 buffer[1];
    FATFS _fs;
    unsigned long int addr = 0;
    
    memcpy(&_fs, &fs, sizeof(fs));//copy fs structure to local FATFS for return it back in end of function
    
    sdcard_open_file(name);
    sdcard_read_bytes(buffer, 1);//Read one byte for updating FS structure
    addr = fs.dsect * 512;
    
    memcpy(&fs, &_fs, sizeof(fs));//return back local FATFS to global FATFS
    
    return addr;
}