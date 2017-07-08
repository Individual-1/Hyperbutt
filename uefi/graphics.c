// Graphics related functions

#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

#include "graphics.h"
#include "info.h"

// TODO: collapse status->efi_error blocks maybe
EFI_STATUS init_graphics(OUT gfx_info_t *gfx_info)
{
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gfx;
    UINT32 mode;
    EFI_HANDLE *handles = NULL;
    UINTN nr_handles;
    UINTN iter;

    Print(L"Entering init graphics, gfx_info: %X\n", gfx_info);

    status = LibLocateHandle(ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &nr_handles, &handles);
    if (EFI_ERROR(status)) {
        Print(L"Failed to locate gfx handles\n");
        return status;
    }

    // iterate over list of handles and pull down information for them, how do we decide which one to use?
    // right now we just find the first handle that has our desired h and v res and use that
    for (iter = 0; iter < nr_handles; iter++) {
        EFI_HANDLE *handle = handles[iter];
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = NULL;
        UINTN size;

        status = uefi_call_wrapper(BS->HandleProtocol, 3, handle, &gEfiGraphicsOutputProtocolGuid, (void **) &gfx);
        if (EFI_ERROR(status)) {
                continue;
        }

        status = find_mode(gfx, &mode);
        if (EFI_ERROR(status)) {
            Print(L"No mode found for handle %d\n", iter);
            continue;
        }

        status = uefi_call_wrapper(gfx->SetMode, 2, gfx, mode);
        if (EFI_ERROR(status)) {
            Print(L"Failed to set mode for handle %d mode %d\n", iter, mode);
            break;
        }

        status = uefi_call_wrapper(gfx->QueryMode, 4, gfx, mode, &size, &info);
        if (EFI_ERROR(status)) {
            Print(L"Failed to read set mode for handle %d mode %d\n", iter, mode);
            break;
        } else {
            // Copy out all data we care about to our gfx_info struct
            gfx_info->fb_width = info->HorizontalResolution;
            gfx_info->fb_height = info->VerticalResolution;
            gfx_info->fb_base = gfx->Mode->FrameBufferBase;
            gfx_info->fb_size = gfx->Mode->FrameBufferSize;
            gfx_info->fb_pixfmt = info->PixelFormat;
            gfx_info->fb_pixmask = info->PixelInformation;
            gfx_info->fb_pixline = info->PixelsPerScanLine;
            break;
        }

    }

    if (handles) {
        FreePool(handles);
    }

    return status;
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

    // Get the base mode for comparison against later
    status = uefi_call_wrapper(gfx->QueryMode, 4, gfx, gfx->Mode->Mode, &size, &info);
    if (EFI_ERROR(status)) {
        Print(L"Query for mode %d failed\n", gfx->Mode->Mode);
        return status;
    }

    // Set base values

    base_info = AllocatePool(sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
    if (!base_info) {
        return EFI_OUT_OF_RESOURCES;
    }

    CopyMem(base_info, info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
    *mode = gfx->Mode->Mode;

    for (UINT32 iter = 0; iter < gfx->Mode->MaxMode; iter += 1) {

        status = uefi_call_wrapper(gfx->QueryMode, 4, gfx, iter, &size, &info);
        if (EFI_ERROR(status)) {
            Print(L"Query for mode %d failed\n", iter);
            goto exit;
        }

        // We want one of these pixel formats probably?
        if (info->PixelFormat != PixelRedGreenBlueReserved8BitPerColor &&
             info->PixelFormat != PixelBlueGreenRedReserved8BitPerColor) {
            continue;
        }

        // Skip if this mode is greater than what we want
        if (info->VerticalResolution > DESIRED_V_RES &&
             info->HorizontalResolution > DESIRED_H_RES) {
            continue;
        }

        // If this is an exact match then bail out, otherwise if this mode is taller than the old one then use it instead
        if (info->VerticalResolution == DESIRED_V_RES &&
             info->HorizontalResolution == DESIRED_H_RES) {
            *mode = iter;
            break;
        } else if (info->VerticalResolution > base_info->VerticalResolution) {
            CopyMem(base_info, info, sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
        }
    }

exit:
    Print(L"Exiting find mode\n");
    if (base_info) {
        FreePool(base_info);
    }

    return status;
}