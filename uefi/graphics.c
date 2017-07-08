// Graphics related functions

#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include "graphics.h"
#include "info.h"

EFI_STATUS init_graphics(OUT gfx_info_t *gfx_info)
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gfx;
    EFI_GUID gfx_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    UINT32 mode;

    Print(L"Entering init graphics, gfx_info: %X\n", gfx_info);

    status = uefi_call_wrapper(BS->LocateProtocol, 3, &gfx_guid, NULL, &gfx);
    if (EFI_ERROR(status) || gfx == NULL) {
        Print(L"Failed to locate gfx handle\n");
        return status;
    }

    mode = gfx->Mode->Mode;

    Print(L"Base mode: %d\n", mode);

    status = find_mode(gfx, &mode);
    if (EFI_ERROR(status)) {
        Print(L"Failed to locate best mode\n");
        return status;
    }

    Print(L"Discovered mode: %d\n", mode);

    status = uefi_call_wrapper(gfx->SetMode, 2, gfx, mode);
    if (EFI_ERROR(status)) {
        Print(L"Failed to set graphics mode\n");
        return status;
    }

    Print(L"Mode set\n");

    gfx_info->protocol = gfx;
    gfx_info->info = *(gfx->Mode->Info);
    gfx_info->buffer_base = gfx->Mode->FrameBufferBase;
    gfx_info->buffer_size = gfx->Mode->FrameBufferSize;

    return EFI_SUCCESS;
}

/*
 * Iterate through the available modes and find whichever one makes best use of the display
 *
 */
EFI_STATUS find_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *gfx, OUT UINT32 *mode)
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *base_info;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN size;

    Print(L"Entering find mode, gfx: %X\n", gfx);

    status = uefi_call_wrapper(gfx->QueryMode, 4, gfx, gfx->Mode->Mode, &size, &info);
    if (EFI_ERROR(status) || !info) {
        Print(L"Query for mode %d failed\n", gfx->Mode->Mode);
        return status;
    }

    Print(L"Base query mode success, max: %d\n", gfx->Mode->MaxMode);

    Print(L"info addresses: %X %X\n", &base_info, info);

    base_info = AllocatePool(sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

    memcpy(base_info, info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));

    Print(L"info values: %X %X\n", base_info, *info);

    Print(L"Entering loop\n");

    for (UINT32 iter = 0; iter < gfx->Mode->MaxMode; iter += 1) {

        Print(L"Loop iteration %d\n", iter);

        status = uefi_call_wrapper(gfx->QueryMode, 4, gfx, iter, &size, &info);
        if (EFI_ERROR(status)) {
            Print(L"Query for mode %d failed\n", iter);
            FreePool(base_info);
            return status;
        }

        // We want one of these pixel formats probably?
        if (info->PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
             info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {
            continue;
        }

        if (info->VerticalResolution >= base_info->VerticalResolution &&
             info->HorizontalResolution >= base_info->HorizontalResolution) {
            memcpy(base_info, info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
            *mode = iter;
        }
    }

    Print(L"Exiting find mode\n");
    
    FreePool(base_info);

    return EFI_SUCCESS;
}
