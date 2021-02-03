
// include initguid.h as described in Visual C++ under Knowledge base topic:
// HOWTO: Avoid Error LNK2001 Unresolved External Using DEFINE_GUID
#include "INITGUID.H"

//
// Define an Interface Guid for genport device class.
// This GUID is used to register (IoRegisterDeviceInterface)
// an instance of an interface so that user application
// can control the genport device.
//


#
//PCC6 PCIe
// {EFB603A7-50A7-47dc-8101-A9A2E6FB7770}
DEFINE_GUID(DRIVER_GUID, 
0xefb603a7, 0x50a7, 0x47dc, 0x81, 0x1, 0xa9, 0xa2, 0xe6, 0xfb, 0x77, 0x70);
