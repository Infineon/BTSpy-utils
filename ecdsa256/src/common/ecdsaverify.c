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
#include <stdlib.h>

#define HASH_SIZE       32

extern BOOL32 ecdsa_verify(unsigned char* digest, unsigned char* signature, Point* key);


int main (int argc, char *argv[])
{
    mbedtls_sha256_context sha2_ctx;
    unsigned char *buf;
    unsigned char testDigest[KEY_LENGTH_BYTES * 2];
    unsigned char *pSignature;
    unsigned char signature[KEY_LENGTH_BYTES*2];
    unsigned char pubKey[KEY_LENGTH_BYTES * 2];
    FILE *fPatch;
    FILE *fPublicKey;
    int fileSize;
    int totalSize;
    int i = 0;

    if(argc != 2)
    {
        printf("usage: %s <patch.signed>\n", argv[0]);
        return 0;
    }

    // open public key file
    if ((fopen_s(&fPublicKey, "ecdsa256_key.pub.bin", "rb")) != 0)
    {
        printf("can not open output signature file %s\n", argv[2]);
        return -1;
    }
    else
    {
        // read the key
        totalSize = (int)fread(pubKey, 1, KEY_LENGTH_BYTES * 2, fPublicKey);
        fclose(fPublicKey);

        // check key length
        if (totalSize != KEY_LENGTH_BYTES * 2)
        {
            printf("public key legnth incorrect\n");
            return -1;
        }
    }

    // open patch file
    if ((fopen_s(&fPatch, argv[1], "rb")) != 0)
    {
        printf("can not open patch file %s\n", argv[1]);
        return -1;
    }

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

    totalSize = (int)fread(buf, fileSize, 1,  fPatch);
    fclose(fPatch);

    mbedtls_sha256_init(&sha2_ctx);

    // everything is ready. Buffer starts at location buf.  The length is
    // totalSize.  The last 64 bytes is the signature.

    // initialize sha256 context
    mbedtls_sha256_starts_ret(&sha2_ctx, 0);

    // generate digest, last bytes are the signature, do not include.
    mbedtls_sha256_update_ret(&sha2_ctx, buf, fileSize - (KEY_LENGTH_BYTES * 2));
    mbedtls_sha256_finish_ret(&sha2_ctx, testDigest);

    // signature is the last (KEY_LENGTH_BYTES * 2) bytes of the file
    pSignature = &buf[fileSize - (KEY_LENGTH_BYTES * 2)];
    for (i = 0; i < KEY_LENGTH_BYTES * 2; ++i)
        signature[i] = *pSignature++;

    if (!ecdsa_verify(testDigest, signature, (Point *)pubKey))
    {
        printf("verify failure\n");
    }
    else
    {
        printf("verify success\n");
    }
    return 0;
}
