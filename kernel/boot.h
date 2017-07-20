#pragma once

#ifndef KBOOT_H
#define KBOOT_H

#include "info.h" // Kernel info.h with mirroring, typedef guard should prevent conflict with UEFI side

void setup_kernel(mem_map_t *mem_map, gfx_info_t *gfx_info);

#endif
