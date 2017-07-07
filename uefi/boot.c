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
 * - Memory Map
 * - Video modes
 * - Modules
 * - 
 *
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

    Print(L"Before set\n");

    efivar_set(L"test", &ts, str, FALSE);

    Print(L"After set\n");

    ST->ConIn->Reset(ST->ConIn, FALSE);
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY) ;

    CHAR16 *strg = NULL;
    UINTN tsg = 0; 
    
    Print(L"Before get\n");

    efivar_get(L"test", &tsg, strg);

    Print(L"After get: results on next line\n");
    Print(strg);


    ST->ConIn->Reset(ST->ConIn, FALSE);
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY) ;

    status = uefi_call_wrapper(BS->ExitBootServices, 2, ImageHandle, mem_map.map_key);
    //TODO error handling
    
    status = uefi_call_wrapper(RT->SetVirtualAddressMap, 4, mem_map.num_entries, mem_map.desc_size, mem_map.desc_version, mem_map.memory_map);
    //TODO error handling
    

    return status;
}
