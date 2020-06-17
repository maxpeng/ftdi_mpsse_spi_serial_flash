#include <iostream>
//#include "libMPSSE_i2c.h"
#include "libMPSSE_spi.h"
#include "dump_util.h"
#include "fmt/format.h"


void check_status(FT_STATUS status);


int main() {
    // 0.1 initialize library.
    Init_libMPSSE();

    // 0.2 get number of available channels.
    uint32 numChannels;
    FT_STATUS status;
    status = SPI_GetNumChannels(&numChannels);
    std::cout << "number of channels: " << numChannels << std::endl;

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

    // 1. read page.
    // 1.1 send out read page command.
    uint32 sizeToTransfer=4;
    uint32 sizeTransfered=0;
    uint8 buffer[4];
    buffer[0] = 0x03;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    status = SPI_Write(ftHandle, buffer, sizeToTransfer, &sizeTransfered,
                       SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                       SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE);
    check_status(status);

    // 1.2 read in page data.
    uint8 buffer_read[256];
    sizeToTransfer = 256;
    status = SPI_Read(ftHandle, buffer_read, sizeToTransfer, &sizeTransfered,
                      SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES|
                      SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE);
    check_status(status);

    // 1.3 dump page data.
    fmt::print("read data:\n{0}\n", DumpUtil::hexlify(reinterpret_cast<const char *>(buffer_read), 256));

    return 0;
}


void check_status(FT_STATUS status)
{
    if (status != FT_OK) {
        fmt::print("{0}:{1}:{3}: status(0x%x) != FT_OK\n", __FILE__, __LINE__, __FUNCTION__, status);
        exit(-1);
    }
}


