// Mirror of UEFI info.h, except with generics instead of EFI structures
// Also some EFI definitions reproduced here so we don't need to pull in the whole uefi sdk
#pragma once

#ifndef INFO_H
#define INFO_H

#include <stddef.h>
#include <stdint.h>

const CHAR16 *mem_types[] = {
    L"EfiReservedMemoryType",
    L"EfiLoaderCode",
    L"EfiLoaderData",
    L"EfiBootServicesCode",
    L"EfiBootServicesData",
    L"EfiRuntimeServicesCode",
    L"EfiRuntimeServicesData",
    L"EfiConventionalMemory",
    L"EfiUnusableMemory",
    L"EfiACPIReclaimMemory",
    L"EfiACPIMemoryNVS",
    L"EfiMemoryMappedIO",
    L"EfiMemoryMappedIOPortSpace",
    L"EfiPalCode"
};

// Taken from GraphicsOutput.h
typedef struct {
    uint32_t    RedMask;
    uint32_t    GreenMask;
    uint32_t    BlueMask;
    uint32_t    ReservedMask;
} EFI_PIXEL_BITMASK;

// Taken from GraphicsOutput.h
typedef enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef uint64_t EFI_PHYSICAL_ADDRESS;

typedef struct mem_map {
    void   *memory_map; // EFI_MEMORY_DESCRIPTOR normally
    uint32_t                   desc_version;
    uint64_t                   desc_size;
    uint64_t                   map_key;
    uint64_t                   num_entries;
} mem_map_t;

typedef struct gfx_info {
    uint16_t                                  fb_hres; // Horizontal Resolution
    uint16_t                                  fb_vres; // Vertical Resolution
    EFI_GRAPHICS_PIXEL_FORMAT               fb_pixfmt;
    EFI_PIXEL_BITMASK                       fb_pixmask; // Currently unused since we don't accept pixelpixelbitmask format
    uint32_t                                    fb_pixline;
    EFI_PHYSICAL_ADDRESS                        fb_base; // uint type, may have to change cast to void *
    uint64_t                                    fb_size;
} gfx_info_t;

#endif
