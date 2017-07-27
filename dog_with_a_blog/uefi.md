# UEFI

## Why UEFI?

* Allows for booting without a third-party bootloader
    * And can still function with a bootloader that supports UEFI applications... probably?
* Dumps you directly into 64 bit long mode without segmentation in uniprocessor mode
    * Don't need to jump through the 16 bit real mode -> 32 bit protected mode -> 64 bit long mode
    * Technically you can enter long mode directly from real mode, but UEFI is still less painful
    * Provides a consistent, (somewhat) stable environment for an OS/UEFI application to build off of
* UEFI simplifies populating common structures like the memory map or ACPI tables
* An OS can retain access to UEFI functions while in long mode, versus BIOS where once you left real mode you lost all access

## Setting up gnu-efi

I initially made use of the gnu-efi libraries and headers since they slotted into a standard linux build chain pretty cleanly.

Pros
* Easy to set up and integrate into a build chain
* Not particularly intrusive to build a project around, just include the appropriate headers
* efilib.h provides some helpful wrappers around common functionality like GetMemoryMap, taking care of allocating the correctly sized buffer (sorta, they guess at a size and increase if necessary)
* Not too many header files to include, with just efi.h and efilib.h you get access to most of the common calls you would need, along with a nice InitializeLib call which sets up ST, BS, and RT (SystemTable, BootServices, RuntimeServices) pointers for you

Cons
* Doesn't seem quite as extensive as EDK2 in terms of functionality provided
* You need to do some hackery with copying sections around and tacking a PE header on the final executable to get it to work with UEFI

## Transitioning to EDK2

After using gnu-efi for a while, I decided to move the UEFI app over to EDK2 because I hate myself. While my code didn't have to change too much (swapping out headers, adding a ton more headers, swapping out wrappers), the work required to integrate my codebase into EDK2 (you integrate into them, not the other way around) was fairly substantial. The setup involves putting together various configuration files which describe different parts of the application and compile process. The EDK2 wiki describes it in great detail (https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Specifications) so I won't, but putting together configs for my tiny project took about 4 hours of frankensteining examples together and fixing problems as they arose during compile. At the end of all of it, I feel I can somewhat understand how everything fits together and slightly appreciate what it all offers. Doesn't change that it is incredibly over-engineered though.

Pros
* Vast amounts of functionality available via various wrappers and APIs
* Backed up by Intel and numerous open source contributors
* Once you're set up, the build system offers a lot of options for compiling and packaging your application

Cons
* Initial integration into their build system is a harrowing endeavour starting with no knowledge
* Why do I need to modify 3 configuration files to add one header file
    * One place to reference that you'll be using the library, one place to take that name and map it to a module, and one more to indicate where that module is
* A lot of flexibility also means a lot of room for error
