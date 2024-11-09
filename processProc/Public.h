/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_processProc,
    0xb57265cd,0x04d5,0x4213,0xaa,0xb0,0xda,0x9d,0x0c,0x10,0x65,0xd5);
// {b57265cd-04d5-4213-aab0-da9d0c1065d5}
