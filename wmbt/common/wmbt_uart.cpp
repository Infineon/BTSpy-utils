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
#include "wmbt_uart.h"
extern "C"
{
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
};

extern 	int debug;

extern "C"
{
	void init_uart(int uart_fd, int baudRate);
	void dump(uchar *out, int len);
};


ComHelper::ComHelper() :
	m_handle(-1)
{
}

ComHelper::~ComHelper()
{
	if (m_handle != -1)
		ClosePort();
}

BOOL ComHelper::OpenPort( const char*  port, int baudRate )
{
	if( (m_handle = open(port, O_RDWR | O_NOCTTY)) == -1 )
	{
		printf("port %s could not be opened, error %d\n", port, errno);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf( "Opened port %s\n", port);
	}

	init_uart(m_handle, baudRate);
	return true;
}

void ComHelper::ClosePort( )
{
	close(m_handle);
	m_handle = -1;
}

DWORD ComHelper::Read(LPBYTE buffer, DWORD dwLen)
{
    int i = 0;
    int len = dwLen;
    DWORD count = 0;

    while ((count = read(m_handle, &buffer[i], len)) < len)
    {
        i += count;
        len -= count;
		count = 0;
    }

    return i+count;
}

DWORD ComHelper::Write(LPBYTE b, DWORD dwLen)
{
	DWORD ret = 0;
	ret =  write(m_handle, b, dwLen);
	return ret;
}

void ComHelper::Flush(DWORD dwFlags)
{
}
