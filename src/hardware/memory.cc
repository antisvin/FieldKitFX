/*
 * memory.c
 *
 */

#include "hardware/memory.h"

namespace fieldkitfx {
void RamChip::init() {
    spiController = &memorySPIController;
    CSGPIOPort = GPIOA;
    CSPinNumber = GPIO_PIN_15;

    /*
     * First setup the Chip select Line
     */
    GPIO_InitTypeDef GPIOStruct;

    GPIOStruct.Pin = CSPinNumber;
    GPIOStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOStruct.Pull = GPIO_NOPULL;
    GPIOStruct.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(CSGPIOPort, &GPIOStruct);

    setCS(HIGH);

    // initialize the MODE register
    uint8_t temp[2] { RAM_COMMAND_WRMR, 64 };

    setCS(LOW);
    HAL_SPI_Transmit(spiController, temp, 2, 100);
    setCS(HIGH);
}

void Memory_initSPI(void) {
    __HAL_RCC_SPI3_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    memorySPIController.Instance = SPI3;

    HAL_SPI_DeInit(&memorySPIController);

    memorySPIController.Init.Mode = SPI_MODE_MASTER; // Master mode
    memorySPIController.Init.BaudRatePrescaler =
        SPI_BAUDRATEPRESCALER_2; // 32MHz/2 => 16.5Mbps
    memorySPIController.Init.Direction = SPI_DIRECTION_2LINES; // 2 way communication
    memorySPIController.Init.DataSize = SPI_DATASIZE_8BIT; // 8 bit transmission
    memorySPIController.Init.FirstBit = SPI_FIRSTBIT_MSB; // MSB first
    memorySPIController.Init.CLKPolarity = SPI_POLARITY_LOW; // clock idle low
    memorySPIController.Init.CLKPhase = SPI_PHASE_1EDGE; // sampling on rising edge
    memorySPIController.Init.NSS = SPI_NSS_SOFT; // Software NSS
    memorySPIController.Init.NSSPMode = SPI_NSS_PULSE_DISABLED;
    memorySPIController.Init.CRCCalculation =
        SPI_CRCCALCULATION_DISABLE; // No CRC calculation
    memorySPIController.Init.TIMode = SPI_TIMODE_DISABLE;

    HAL_SPI_Init(&memorySPIController);

    /*
     * SPI GPIO setup
     */
    GPIO_InitTypeDef GPIOStruct;

    GPIOStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIOStruct.Mode = GPIO_MODE_AF_PP;
    GPIOStruct.Speed = GPIO_SPEED_HIGH;
    GPIOStruct.Pull = GPIO_PULLDOWN;
    GPIOStruct.Alternate = GPIO_AF6_SPI3;

    HAL_GPIO_Init(GPIOC, &GPIOStruct);
}

void Memory_flushSPIFIFO(SPI_HandleTypeDef* spiController) {
    HAL_SPIEx_FlushRxFifo(spiController);
}

void RamChip::setCS(uint8_t state) {
    if (state)
        HAL_GPIO_WritePin(CSGPIOPort, CSPinNumber, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(CSGPIOPort, CSPinNumber, GPIO_PIN_RESET);
}

void RamChip::write(uint32_t address, uint8_t data) {
    address &= 0x00ffffff; // to discard the MSByte of the 32bit number

    uint8_t temp[] { RAM_COMMAND_WRITE,
        address >> 16, // the MSByte of the 24bit address
        address >> 8, address, data };

    setCS(LOW);
    HAL_SPI_Transmit(spiController, temp, 5, 100);
    setCS(HIGH);
}

uint8_t RamChip::read(uint32_t address) {
    uint8_t recvVal[5] = { 0xff };

    address &= 0x00ffffff; // to discard the MSByte of the 32bit number

    uint8_t tempW[] { RAM_COMMAND_READ,
        address >> 16, // the MSByte of the 24bit address
        address >> 8, address, 0xff };

    Memory_flushSPIFIFO(spiController);

    setCS(LOW);
    HAL_SPI_TransmitReceive(spiController, tempW, recvVal, 5, 100);
    setCS(HIGH);

    return recvVal[4];
}

void RamChip::write(uint32_t startAddress, uint8_t* data, uint32_t size) {
    startAddress &= 0x00ffffff; // to discard the MSByte of the 32bit number

    uint8_t temp[] { RAM_COMMAND_WRITE,
        startAddress >> 16, // the MSByte of the 24bit address
        startAddress >> 8, startAddress };

    setCS(LOW);
    HAL_SPI_Transmit(spiController, temp, 4, 100);
    HAL_SPI_Transmit(spiController, data, size, 100);
    setCS(HIGH);
}

void RamChip::read(uint32_t startAddress, uint8_t* data, uint32_t size) {
    startAddress &= 0x00ffffff; // to discard the MSByte of the 32bit number

    uint8_t tempW[] { RAM_COMMAND_READ,
        startAddress >> 16, // the MSByte of the 24bit address
        startAddress >> 8, startAddress };

    setCS(LOW);
    HAL_SPI_Transmit(spiController, tempW, 4, 100);
    Memory_flushSPIFIFO(spiController);
    HAL_SPI_Receive(spiController, data, size, 100);
    setCS(HIGH);
}

void RamChip::read(uint16_t frameNumber, Frame* frame) {
    read(frameNumber * USER_AUDIO_IO_BUFFER_SIZE, frame->samples,
        USER_AUDIO_IO_BUFFER_SIZE);
}

void RamChip::write(uint16_t frameNumber, Frame* frame) {
    write(frameNumber * USER_AUDIO_IO_BUFFER_SIZE, frame->samples,
        USER_AUDIO_IO_BUFFER_SIZE);
}

void RamChip::RWError() {
    __asm("bkpt");
}

SPI_HandleTypeDef memorySPIController;

}
