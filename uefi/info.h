// UEFI info gathering definitions
#pragma once

#ifndef INFO_H
#define INFO_H

#include <efi.h>
#include <efilib.h>
#include <stddef.h>

typedef struct mem_map_s {
    EFI_MEMORY_DESCRIPTOR   *memory_map;
    UINT32                  desc_version;
    UINTN                   desc_size;
    UINTN                   map_key;
    UINTN                   num_entries;
} mem_map_t;

typedef struct gfx_info_s {
    EFI_GRAPHICS_OUTPUT_PROTOCOL            *protocol;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    info;
    EFI_PHYSICAL_ADDRESS                    buffer_base;
    UINTN                                   buffer_size;
} gfx_info_t;

static const CHAR16 *mem_types[] = {
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
    L"EfiPalCode",
};

#endif
