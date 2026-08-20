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
/*
 ********************************************************************
 *    File Name: multprecision.h
 *
 *    Abstract: elliptic curve long integer math library
 *
 *    Functions:
 *            --
 *
 *    $History:$
 *
 ********************************************************************
*/
#ifndef MULTPRECISION_H
#define MULTPRECISION_H

#include "types.h"

//#define ARM_ASM

#define DWORD_BITS 		        32
#define DWORD_BYTES 	        4
#define DWORD_BITS_SHIFT        5

#define KEY_LENGTH_BITS			256
#define KEY_LENGTH_DWORDS   	(KEY_LENGTH_BITS/DWORD_BITS)
#define KEY_LENGTH_BYTES		(KEY_LENGTH_DWORDS*DWORD_BYTES)


/* return 1 if a > b, return -1 if a < b, return 0 if a == b */
int MP_CMP(DWORD *a, DWORD *b);

/* return 1 if a is zero, return 0 otherwise */
int MP_isZero(DWORD *a);

/* set c = 0 */
void MP_Init(DWORD *c);

/* assign c = a */
void MP_Copy(DWORD *c, DWORD *a);

/* return most significant bit postion */
UINT32 MP_MostSignBits(DWORD *a);

/* compute aminus = u ^ -1 mod modulus */
void MP_InvMod(DWORD *aminus, DWORD *u, const DWORD* modulus);

/* c = a + b */
DWORD MP_Add(DWORD *c, DWORD *a, DWORD *b);

/* c = a + b mod p */
void MP_AddMod(DWORD *c, DWORD *a, DWORD *b);

/* c = a - b */
DWORD MP_Sub(DWORD *c, DWORD *a, DWORD *b);

/* c = a - b mod p */
void MP_SubMod(DWORD *c, DWORD *a, DWORD *b);

/* c = a >> 1 */
void MP_RShift(DWORD * c, DWORD * a);

/* c = a << 1 */
DWORD MP_LShift(DWORD * c, DWORD * a);

/* c = a << 1  mod a */
void MP_LShiftMod(DWORD * c, DWORD * a);

/* c = a * b mod p */
void MP_MersennsMultMod(DWORD *c, DWORD *a, DWORD *b);

/* c = a * a mod p */
void MP_MersennsSquaMod(DWORD *c, DWORD *a);

/* c = a * b */
void MP_Mult(DWORD *c, DWORD *a, DWORD *b);

/* c = a * b * r^-1 mod n */
void MP_MultMont(DWORD *c, DWORD *a, DWORD *b);


#endif
