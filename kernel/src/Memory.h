#pragma once

#include "EfiMemory.h"

#include <stdint.h>
#include <stddef.h>

void SetMemory(void* block, uint8_t value, uint64_t size);
