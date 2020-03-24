/*
 * memory.h
 *
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <cstdint>
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#include "engine/frame.h"
#include "hardware/23lc1024.h"
#include "utils/utils.h"

/*
 * Pin state
 */
#define HIGH 1
#define LOW 0

namespace fieldkitfx {
/*
 * memory spi controller
 */
extern SPI_HandleTypeDef memorySPIController;

/*
 * General SPI functions
 */
void Memory_initSPI();
void Memory_flushSPIFIFO();

class RamChip {
private:
    SPI_HandleTypeDef* spiController;
    /*
     * chip select related parameters
     */
    GPIO_TypeDef* CSGPIOPort;
    uint16_t CSPinNumber;
    DISALLOW_COPY_AND_ASSIGN(RamChip);

public:
    RamChip() = default;
    void init();
    void setCS(uint8_t state);
    // Cell
    void write(uint32_t address, uint8_t data);
    uint8_t read(uint32_t address);
    // Array
    void write(uint32_t startAddress, uint8_t* data, uint32_t size);
    void read(uint32_t startAddress, uint8_t* data, uint32_t size);
    // Frame
    void write(uint16_t frameNumber, Frame* frame);
    void read(uint16_t frameNumber, Frame* frame);
    void RWError();
};

}

#endif /* MEMORY_H_ */
