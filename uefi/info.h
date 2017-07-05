// UEFI info gathering definitions
#pragma once

#include <efi.h>
#include <efilib.h>
#include <stddef.h>

typedef struct mem_map {
    EFI_MEMORY_DESCRIPTOR   *memory_map;
    UINT32                  desc_version;
    UINTN                   desc_size;
    UINTN                   map_key;
    UINTN                   num_entries;
} mem_map_t;

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
    L"EfiPalCode",
};
