// Uefi boot stub using gnu-efi

#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include "info.h"
#include "util.h"

mem_map_t mem_map;

/*
 * EFI stub
 *
 * Gather information into efi vars then load our kernel into memory and switch execution to it (after exiting boot services)
 *
 * * Memory Map
 * * Video modes
 * * Modules
 *
 * TODO before we exit bootservices:
 * * setup graphics (do mode detection in boot services)
 * * Get handles to pci devices
 * * Populate the memory map
 * * Load in kernel and put it in memory somewhere
 * * Set up GDT and disable interrupts
 * * exit boot services then jump to kernel
 */
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) 
{
    EFI_STATUS status;
    EFI_INPUT_KEY key;

    // init efi lib for use
    InitializeLib(ImageHandle, SystemTable);

    Print(L"test string\n");

    // populate memory map
    mem_map.memory_map = LibMemoryMap(&mem_map.num_entries, &mem_map.map_key, &mem_map.desc_size, &mem_map.desc_version);

    // test code please ignore
    ST->ConIn->Reset(ST->ConIn, FALSE);
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY) ;

    
    CHAR16 str[] = L"string";
    UINTN ts = sizeof(str);

    status = efivar_set(L"test", &ts, str, FALSE);

    status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mem_map.map_key);
    //TODO error handling
    
    status = uefi_call_wrapper(RT->SetVirtualAddressMap, 4, mem_map.num_entries, mem_map.desc_size, mem_map.desc_version, mem_map.memory_map);
    //TODO error handling
    

    return status;
}
