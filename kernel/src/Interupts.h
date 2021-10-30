#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct InteruptFrame InteruptFrame;

// TODO: Find some way to make gcc stop ignoring __attribute__((interupt))
/* __attribute__((interupt)) */ void PageFault_Handler(InteruptFrame* interuptFrame);
/* __attribute__((interupt)) */ void DoubleFault_Handler(InteruptFrame* interuptFrame);
/* __attribute__((interupt)) */ void GeneralProtectionFault_Handler(InteruptFrame* interuptFrame);
