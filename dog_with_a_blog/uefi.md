# UEFI

## Why UEFI?

* Allows for booting without a third-party bootloader
    * And can still function with a bootloader that supports UEFI applications... probably?
* Dumps you directly into 64 bit long mode without segmentation in uniprocessor mode
    * Don't need to jump through the 16 bit real mode -> 32 bit protected mode -> 64 bit long mode
    * Technically you can enter long mode directly from real mode, but UEFI is still less painful
* UEFI simplifies populating common structures like the memory map or ACPI tables
* 
