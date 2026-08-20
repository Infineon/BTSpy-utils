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
#include "sha256.h"
#include "multprecision.h"
#include "ecc_pp.h"
#include <stdio.h>
#include <string.h>
#define _CRT_RAND_S
#include <stdlib.h>
#include <time.h>

#define HASH_SIZE       32

extern BOOL32 ecdsa_sign(unsigned char* digest, unsigned char* signature, unsigned char* privateKey, unsigned char* randomK);

int main (int argc, char *argv[])
{
    mbedtls_sha256_context sha2_ctx;
    unsigned char *buf;
    unsigned char digest[HASH_SIZE];
    unsigned char signature[KEY_LENGTH_BYTES*2];
    unsigned char privateKey[KEY_LENGTH_BYTES];
    unsigned char randomK[KEY_LENGTH_BYTES];
    char filename[512];
    int readBytes;

    FILE *fKey;
    FILE *fPatch;
    FILE *fSigned;
    int fileSize;
    int fileReadBytes;
    int i = 0;

    mbedtls_sha256_init(&sha2_ctx);

#if defined(SHA2_SELF_TEST)
    sha2_self_test(1);
#endif

    // check input parameter
    if(argc != 2)
    {
        printf("usage: %s <patch>\n", argv[0]);
        return 0;
    }

    // read private key
    if ((fopen_s(&fKey, "ecdsa256_key.pri.bin", "rb")) != 0)
    {
        printf("can not open key file %s\n", argv[1]);
        return -1;
    }
    fseek(fKey, 0, SEEK_END);
    fileSize = ftell(fKey);
    rewind(fKey);
    fileReadBytes = (int)fread(privateKey, 1, fileSize, fKey);
    fclose(fKey);

    // check private key size
    if(fileReadBytes != KEY_LENGTH_BYTES)
    {
        printf("wrong key length\n");
        return -1;
    }
    srand((UINT32)time(NULL));
    for (i = 0; i < KEY_LENGTH_DWORDS; i++)
    {
        UINT32 r;
#ifdef WIN32
        rand_s(&r);
#else
        r = rand_s(&r);
#endif
        *(UINT32 *)&randomK[4 * i] = r;
    }

    /*
    * Write the signed with ids patch into <filename>.signed
    */
    _snprintf(filename, 512, "%s.signed", argv[1]);

    if ((fopen_s(&fSigned, filename, "wb")) != 0)
    {
        printf(" failed\n  ! Could not create %s\n\n", filename);
        return -1;
    }

    if ((fopen_s(&fPatch, argv[1], "rb")) != 0)
    {
        printf("can not open patch file %s\n", argv[1]);
        return -1;
    }

    // initialize sha256 context
    mbedtls_sha256_starts_ret(&sha2_ctx, 0);

    // read patch size
    fseek(fPatch, 0, SEEK_END);
    fileSize = ftell(fPatch);
    rewind(fPatch);

    // allocate memory and read the file
    if ((buf = (unsigned char *)malloc(fileSize)) == NULL)
    {
        printf("no memory\n");
        return -1;
    }
    readBytes = (int)fread(buf, fileSize, 1, fPatch);
    printf("filesize = %d, readBytes = %d\n", fileSize, readBytes);
    fwrite(buf, fileSize, 1, fSigned);
    mbedtls_sha256_update_ret(&sha2_ctx, buf, fileSize);
    // generate digest
    mbedtls_sha256_finish_ret(&sha2_ctx, digest);
    free(buf);

    // generate signature
    if(!ecdsa_sign(digest, signature, privateKey, randomK))
    {
        // fail to generate signature
        printf("sign failure, please try again with different input\n");
    }
    else
    {
        // dump signature to output
        if (fwrite(signature, 1, KEY_LENGTH_BYTES * 2, fSigned) != KEY_LENGTH_BYTES * 2)
        {
            printf("failed to write signature to the file\n");
            return -1;
        }
    }

    fclose(fPatch);
    fclose(fSigned);

    printf("Signed file %s\n", filename);
    return 0;
}
