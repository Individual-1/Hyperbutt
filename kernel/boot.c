/*
 * TODO: everything
 *
 * Entry into kernel load and initialization from uefi bootloader code
 * We need to:
 * 1. Decompress and load our kernel into memory
 * 2. Set up various structures that our kernel needs to function
 * 3. Jump to kernel
 */

#include <stddef.h>
#include <stdint.h>

#include "info.h"

/*
 * As stated above, we need to:
 * 1. Decompress and load our kernel into memory
 * 2. Set up various structures that our kernel needs to function
 * 3. Jump to kernel
 */
void setup_kernel(mem_map_t *mem_map, gfx_info_t *gfx_info)
{
    // idk do something I guess
}
