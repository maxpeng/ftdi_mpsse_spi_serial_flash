#include <stdio.h>
#include "libMPSSE_spi.h"
#include "dump_util.h"


#define PAGE_SIZE   256
#define FLASH_STATUS_BIT_BUSY  0x01u
#define FLASH_STATUS_BIT_WEN   0x02u


void check_status(FT_STATUS status);

void read_id(FT_HANDLE ftHandle);
void read_page(FT_HANDLE ftHandle, unsigned int address);
uint8 read_status(FT_HANDLE ftHandle);
void enable_write(FT_HANDLE ftHandle);
void erase_sector(FT_HANDLE ftHandle, unsigned int address);
void write_page(FT_HANDLE ftHandle, unsigned int address, uint8 *data, uint32 len);


int main() {
    // 0.1 initialize library.
    Init_libMPSSE();

    // 0.2 get number of available channels.
    uint32 numChannels;
    FT_STATUS status;
    status = SPI_GetNumChannels(&numChannels);
    printf("Number of SPI channels: %d\n", numChannels);

    // 0.3 open channel.
    FT_HANDLE ftHandle;
    status = SPI_OpenChannel(0, &ftHandle);
    check_status(status);

    // 0.4 initialize channel.
    ChannelConfig channelConf = {0};
    channelConf.ClockRate = 12000000;
    channelConf.LatencyTimer = 255;
    channelConf.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_DBUS3 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
    channelConf.Pin = 0x00000000;/*FinalVal-FinalDir-InitVal-InitDir (for dir 0=in, 1=out)*/
    status = SPI_InitChannel(ftHandle, &channelConf);
    check_status(status);

    // 1. read ID
    read_id(ftHandle);

    // 2. read page 0.
    read_page(ftHandle, 0x00);
    read_status(ftHandle);

    // 3. erase sector 0.
    // 3.1 enable write.
    enable_write(ftHandle);
    uint8 flash_status;
    do {
        flash_status = read_status(ftHandle);
    } while ((flash_status & FLASH_STATUS_BIT_WEN) != FLASH_STATUS_BIT_WEN);

    // 3.2 erase sector.
    erase_sector(ftHandle, 0x00);

    // 3.3 wait until erasing is done.
    do {
        flash_status = read_status(ftHandle);
    } while (flash_status & FLASH_STATUS_BIT_BUSY);
    // 3.4 read page 0 to make sure it is blank.
    read_page(ftHandle, 0x00);

    // 4. write page 0.
    // 4.1 enable write.
    enable_write(ftHandle);

    // 4.2 write page 0.
    uint8 data[256];
    for (int i=0; i < 256; i++) {
        data[i] = i;
    }
    write_page(ftHandle, 0x00, data, 256);

    // 4.3 wait until programming is done.
    do {
        flash_status = read_status(ftHandle);
    } while (flash_status & FLASH_STATUS_BIT_BUSY);

    // 4.4 read page 0.
    read_page(ftHandle, 0x00);

    // 5. clean up.
    SPI_CloseChannel(ftHandle);
    Cleanup_libMPSSE();

    return 0;
}


void check_status(FT_STATUS status)
{
    if (status != FT_OK) {
        printf("{%s}:{%u}:{%s}: status(0x%02x) != FT_OK\n", __FILE__, __LINE__, __FUNCTION__, status);
        exit(-1);
    }
}


void read_id(FT_HANDLE ftHandle)
{
    printf("\n# read ID:\n");

    FT_STATUS status;
    uint32 sizeToTransfer = 1;
    uint32 sizeTransferred = 0;
    uint8 buffer_write[1] = {0x9f};
    status = SPI_Write(ftHandle, buffer_write, sizeToTransfer, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
    check_status(status);

    uint8 buffer_read[3] = {0};
    sizeToTransfer = 3;
    sizeTransferred = 0;
    status = SPI_Read(ftHandle, buffer_read, sizeToTransfer, &sizeTransferred,
                      SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                      SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);
    dump_buf(buffer_read, sizeTransferred);
}


void read_page(FT_HANDLE ftHandle, unsigned int address)
{
    printf("\n\n# read page at address 0x%06x:", address);
    // send out read page command.
    FT_STATUS status;
    uint32 sizeToTransfer = 4;
    uint32 sizeTransferred = 0;
    uint8 buffer_write[4];
    buffer_write[0] = 0x03;
    buffer_write[1] = (address >> 16u) & 0xffu;
    buffer_write[2] = (address >> 8u) & 0xffu;
    buffer_write[3] = address & 0xffu;

    status = SPI_Write(ftHandle, buffer_write, sizeToTransfer, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
    check_status(status);

    // read in page data.
    uint8 buffer_read[PAGE_SIZE];
    sizeToTransfer = PAGE_SIZE;
    status = SPI_Read(ftHandle, buffer_read, sizeToTransfer, &sizeTransferred,
                      SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                      SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);

    // dump page data.
    printf("\nread data:\n");
    dump_buf(buffer_read, PAGE_SIZE);
}


uint8 read_status(FT_HANDLE ftHandle)
{
    printf("\n\n# read status:\n");

    FT_STATUS status;
    uint32 sizeToTransfer = 1;
    uint32 sizeTransferred = 0;
    uint8 buffer_write[1] = {0x05};
    status = SPI_Write(ftHandle, buffer_write, sizeToTransfer, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
    check_status(status);

    uint8 buffer_read[1] = {0};
    sizeToTransfer = 1;
    sizeTransferred = 0;
    status = SPI_Read(ftHandle, buffer_read, sizeToTransfer, &sizeTransferred,
                      SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES |
                      SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);
    dump_buf(buffer_read, sizeTransferred);

    return buffer_read[0];
}


void enable_write(FT_HANDLE ftHandle)
{
    printf("\n\n# enable write");

    FT_STATUS status;
    uint32 sizeToTransfer = 1;
    uint32 sizeTransferred = 0;
    uint8 buffer_write[1] = {0x06};
    status = SPI_Write(ftHandle, buffer_write, sizeToTransfer, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE |
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);
}


void erase_sector(FT_HANDLE ftHandle, unsigned int address)
{
    printf("\n\n# erase sector at address 0x%06x:", address);
    // send out read page command.
    FT_STATUS status;
    uint32 sizeToTransfer = 4;
    uint32 sizeTransferred = 0;
    uint8 buffer_write[4];
    buffer_write[0] = 0x20;
    buffer_write[1] = (address >> 16u) & 0xffu;
    buffer_write[2] = (address >> 8u) & 0xffu;
    buffer_write[3] = address & 0xffu;

    status = SPI_Write(ftHandle, buffer_write, sizeToTransfer, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES |
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE |
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);
}


void write_page(FT_HANDLE ftHandle, unsigned int address, uint8 *data, uint32 len)
{
    printf("\n\n# write page at address 0x%06x:\ndata to write:\n", address);
    dump_buf(data, len);

    // send out read page command.
    FT_STATUS status;
    uint32 sizeToTransfer = 4;
    uint32 sizeTransferred = 0;
    uint8 buffer_write[4];
    buffer_write[0] = 0x02;
    buffer_write[1] = (address >> 16u) & 0xffu;
    buffer_write[2] = (address >> 8u) & 0xffu;
    buffer_write[3] = address & 0xffu;

    status = SPI_Write(ftHandle, buffer_write, sizeToTransfer, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
    check_status(status);

    sizeTransferred = 0;
    status = SPI_Write(ftHandle, data, len, &sizeTransferred,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES |
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);
}
