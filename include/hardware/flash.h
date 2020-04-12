#ifndef _HARDWARE_FLASH_H_
#define _HARDWARE_FLASH_H_

#include <cstring>
#include "stm32f3xx_hal.h"
#include "utils/utils.h"

#define PAGE_SIZE (uint16_t)0x800

/*
 * This file is ported from stmlib. The problem with original is that all
 * MI modules (at least until Clouds2 is released) are based on Stdperiph, while
 * we use HAL.
 */

namespace fieldkitfx {

static FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t PageError = 0;

// Class for storing calibration and state data in a same sector of flash
// without having to rewrite the calibration data every time the state is
// changed.
//
// Data is stored in a RIFF-ish format, with the peculiarity that the size
// field of the header is 16-bit instead of 32-bit - the remaining 16 bits being
// used to store a naive checksum of the chunk data.
//
// +----+-----------------+----+------------+----+------------+----+--
// |HEAD|CALIBRATION CHUNK|HEAD|STATE CHUNK1|HEAD|STATE CHUNK2|HEAD|..
// +----+-----------------+----+------------+----+------------+----+--
//
// The first chunk stores the large, "slow" data like calibration or presets.
// Whenever this data needs to be saved, the entire sector of flash is erased.
//
// The subsequent chunks store successive revision of the "fast" data like
// module state. Whenever this data changes, a new chunk is appended to the list
// until the flash sector is filled - in which case the sector is erased, and
// the calibration data + first version of state is written.
template <uint32_t flash_start, uint32_t flash_end, typename PersistentData, typename StateData>
class ChunkStorage {
private:
    struct ChunkHeader {
        uint32_t tag;
        uint16_t size;
        uint16_t checksum;
    };

    enum { FLASH_STORAGE_SIZE = flash_end - flash_start };

public:
    ChunkStorage() {
    }
    ~ChunkStorage() {
    }
    // Loads the latest saved data. In case the sector is blank/corrupted,
    // reinitializes the sector and returns false.
    bool Init(PersistentData* persistent_data, StateData* state_data) {
        persistent_data_ = persistent_data;
        state_data_ = state_data;

        if (ReadChunk(0, persistent_data)) {
            for (next_state_chunk_index_ = 1;
                 chunk_address(next_state_chunk_index_ + 1) <= flash_end;
                 ++next_state_chunk_index_) {
                if (!ReadChunk(next_state_chunk_index_, state_data)) {
                    break;
                }
            }
            if (next_state_chunk_index_ != 1) {
                return true;
            }
        }
        Format();
        return false;
    }

    void SaveState() {
        if (chunk_address(next_state_chunk_index_ + 1) > flash_end) {
            Format();
        }
        else {
            HAL_FLASH_Unlock();
            WriteChunk(next_state_chunk_index_, state_data_);
            next_state_chunk_index_++;
        }
    }

    void SavePersistentData() {
        Format();
    }

private:
    void Format() {
        HAL_FLASH_Unlock();

        EraseInitStruct.TypeErase = TYPEERASE_PAGES;
        EraseInitStruct.PageAddress = flash_start;
        EraseInitStruct.NbPages = (flash_end - flash_start) / PAGE_SIZE;

        if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
            // Error
        }

        WriteChunk(0, persistent_data_);
        WriteChunk(1, state_data_);
        next_state_chunk_index_ = 2;
    }

    template <typename T>
    bool ReadChunk(size_t index, T* data) {
        const char* flash_ptr = (const char*)(chunk_address(index));
        ChunkHeader* h = (ChunkHeader*)(flash_ptr);
        if (h->tag == T::tag && h->size == sizeof(T) &&
            Checksum(flash_ptr + sizeof(ChunkHeader), h->size) == h->checksum) {
            memcpy(data, flash_ptr + sizeof(ChunkHeader), h->size);
            return true;
        }
        else {
            return false;
        }
    }

    template <typename T>
    void WriteChunk(size_t index, const T* data) {
        ChunkHeader h;
        h.tag = T::tag;
        h.size = sizeof(T);
        h.checksum = Checksum(data, sizeof(T));

        FlashWrite(chunk_address(index), &h);
        FlashWrite(chunk_address(index) + sizeof(ChunkHeader), data);
    }

    template <typename T>
    void FlashWrite(uint32_t address, const T* data) {
        const uint32_t* words = (const uint32_t*)(data);
        size_t size = (sizeof(T) + 3) & ~0x03;
        while (size) {
            if (HAL_FLASH_Program(TYPEPROGRAM_WORD, address, *words) != HAL_OK) {
                // Error
            };
            address += 4;
            size -= 4;
            ++words;
        }
    }

    template <typename T>
    inline size_t ChunkSize() {
        return sizeof(ChunkHeader) + ((sizeof(T) + 3) & ~0x03);
    }

    uint32_t chunk_address(size_t index) {
        if (index == 0) {
            return flash_start;
        }
        else {
            return flash_start + ChunkSize<PersistentData>() +
                ChunkSize<StateData>() * (index - 1);
        }
    }

    uint16_t Checksum(const void* data, size_t size) const {
        uint16_t sum = 0;
        const char* bytes = static_cast<const char*>(data);
        while (size--) {
            sum += *bytes++;
        }
        return sum ^ 0xffff;
    }

private:
    PersistentData* persistent_data_;
    StateData* state_data_;
    uint16_t next_state_chunk_index_;

    DISALLOW_COPY_AND_ASSIGN(ChunkStorage);
};

}

#endif