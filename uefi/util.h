#pragma once

#include <efi.h>
#include <efilib.h>

EFI_STATUS efivar_set(CHAR16 *name, UINTN *size, VOID *data, BOOLEAN persist);

EFI_STATUS efivar_get(CHAR16 *name, UINTN *size, VOID **data);
