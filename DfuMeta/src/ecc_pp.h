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
 *    File Name: ecc_pp.h
 *
 *    Abstract: ECDSA signature sign and verify algorithms
 *
 *    Functions:
 *            --
 *
 *    $History:$
 *
 ********************************************************************
*/

#ifndef ECC_PP_H
#define ECC_PP_H

#include "multprecision.h"

/* EC point with projective coordinates */
struct _point
{
	DWORD x[KEY_LENGTH_DWORDS];
	DWORD y[KEY_LENGTH_DWORDS];
	DWORD z[KEY_LENGTH_DWORDS];
};
typedef struct _point Point;

/* EC point with affine coordinates */
struct _pointAff
{
	DWORD x[KEY_LENGTH_DWORDS];
	DWORD y[KEY_LENGTH_DWORDS];
};
typedef struct _point PointAff;

/* EC curve domain parameter type */
typedef struct
{
	// prime modulus
    DWORD p[KEY_LENGTH_DWORDS];

    // order
    DWORD n[KEY_LENGTH_DWORDS];

	// base point, a point on E of order r
    Point G;

}EC;

extern EC curve;
#define modp (DWORD*)(curve.p)
#define modn (DWORD*)(curve.n)

extern UINT32 nprime[];
extern UINT32 rr[];


/* Point multiplication with NAF method */
void ECC_PM_B_NAF(Point *q, Point *p, DWORD *n);
#define ECC_PM(q, p, n)	ECC_PM_B_NAF(q, p, n)

/* ECDSA sign */
BOOL32 ecdsa_sign(unsigned char* digest, unsigned char* signature, unsigned char* privateKey, unsigned char* randomK);

/* ECDSA verification */
BOOL32 ecdsa_verify(unsigned char* digest, unsigned char* signature, Point* key);

/* Pre-computed window size */
#define FP_WINDOW_SIZE      4

/* Bit scanned when using Pre-computed method */
#define FP_MAP_SIZE         (KEY_LENGTH_BITS/FP_WINDOW_SIZE)

/* Built-in public key */
extern Point ecdsa_q;

/* Built-in pre-computed base point */
extern PointAff ecdsa_pre_g[];

/* Built-in pre-computed public key */
extern PointAff ecdsa_pre_q[];

/* Macro for endianess swap */
#define BE_SWAP(buf, index) \
    ((buf[index+0] << 24) | \
     (buf[index+1] << 16) | \
     (buf[index+2] << 8) | \
     (buf[index+3] << 0))


#endif
