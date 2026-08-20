/*
 * (c) 2016-2026, Infineon Technologies AG, or an affiliate of Infineon
 * Technologies AG. All rights reserved.
 * This software, associated documentation and materials ("Software") is
 * owned by Infineon Technologies AG or one of its affiliates ("Infineon")
 * and is protected by and subject to worldwide patent protection, worldwide
 * copyright laws, and international treaty provisions. Therefore, you may use
 * this Software only as provided in the license agreement accompanying the
 * software package from which you obtained this Software. If no license
 * agreement applies, then any use, reproduction, modification, translation, or
 * compilation of this Software is prohibited without the express written
 * permission of Infineon.
 *
 * Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
 * IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
 * THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
 * SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
 * Infineon reserves the right to make changes to the Software without notice.
 * You are responsible for properly designing, programming, and testing the
 * functionality and safety of your intended application of the Software, as
 * well as complying with any legal requirements related to its use. Infineon
 * does not guarantee that the Software will be free from intrusion, data theft
 * or loss, or other breaches ("Security Breaches"), and Infineon shall have
 * no liability arising out of any Security Breaches. Unless otherwise
 * explicitly approved by Infineon, the Software may not be used in any
 * application where a failure of the Product or any consequences of the use
 * thereof can reasonably be expected to result in personal injury.
 */
#include <Windows.h>
#include <string>
using namespace std;

//**************************************************************************************************
//*** Definitions for BTW Serial Bus
//**************************************************************************************************

// Helper class to print debug messages to Debug Console
class DebugHelper
{
public:
    void DebugOut() { OutputDebugStringA(m_Buffer); }
    void DebugOut(LPCSTR v) { OutputDebugStringA(v); }
    void DebugOut(LPCSTR fmt, LPCSTR v);
    void DebugOut(LPCSTR fmt, DWORD v1);
    void DebugOut(LPCSTR fmt, DWORD v1, DWORD v2);
    void PrintCommProp(COMMPROP& commProp);
    void PrintCommState(DCB& serial_config);

    static char m_Buffer[1024];
};

//
// Serial Bus class, use this class to read/write from/to the serial bus device
//
class ComHelper
{
public:
    ComHelper();
    virtual ~ComHelper();

    // oopen serialbus driver to access device
    BOOL OpenPort( int port, int baudRate );
    void ClosePort( );

    // read data from device
    DWORD Read(LPBYTE b, DWORD dwLen);

    // write data to device
    DWORD Write(LPBYTE b, DWORD dwLen);

    BOOL IsOpened( );

    void Flush(DWORD dwFlags);
private:

    // overlap IO for Read and Write
    OVERLAPPED m_OverlapRead;
    OVERLAPPED m_OverlapWrite;
    HANDLE m_handle;
};
