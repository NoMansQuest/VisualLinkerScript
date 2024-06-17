#include <cassert>
#include <cstring>
#include <iostream>

#include "MD5.h"
#include "MD5_loc.h"

namespace VisualLinkerScript::MD5 {

    CMD5Hasher::CMD5Hasher() {
        initialise();
    }

    CMD5Hasher::CMD5Hasher(const void* input, const unsigned int input_length, void* signatureBuffer) {
        /* initialize the computation context */
        initialise();

        /* process whole buffer but last input_length % MD5_BLOCK bytes */
        ProcessBlock(input, input_length);

        /* put result in desired memory area */
        Finish(signatureBuffer);
    }

    void CMD5Hasher::ProcessBlock(const void* input, const unsigned int input_length) {
        if (!finished) {
            unsigned int processed = 0;

            /*
             * If we have any data stored from a previous call to process then we use these
             * bytes first, and the new data is large enough to create a complete block then
             * we process these bytes first.
             */
            if (stored_size and input_length + stored_size >= VisualLinkerScript::MD5::BLOCK_SIZE) {
                unsigned char block[VisualLinkerScript::MD5::BLOCK_SIZE];
                memcpy(block, stored, stored_size);
                memcpy(block + stored_size, input, VisualLinkerScript::MD5::BLOCK_SIZE - stored_size);
                processed = VisualLinkerScript::MD5::BLOCK_SIZE - stored_size;
                stored_size = 0;
                process_block(block);
            }

            /*
             * While there is enough data to create a complete block, process it.
             */
            while (processed + VisualLinkerScript::MD5::BLOCK_SIZE <= input_length) {
                process_block((unsigned char*)input + processed);
                processed += VisualLinkerScript::MD5::BLOCK_SIZE;
            }

            /*
             * If there are any unprocessed bytes left over that do not create a complete block
             * then we store these bytes for processing next time.
             */
            if (processed != input_length) {
                memcpy(stored + stored_size, (char*)input + processed, input_length - processed);
                stored_size += input_length - processed;
            }
            else {
                stored_size = 0;
            }
        }
        else {
            // throw error when trying to process after completion?
        }
    }

    void CMD5Hasher::Finish(void* signature_) {
        if (!finished) {
            if (message_length[0] + stored_size < message_length[0])
                message_length[1]++;
            message_length[0] += stored_size;

            int pad = VisualLinkerScript::MD5::BLOCK_SIZE - (sizeof(unsigned int) * 2) - stored_size;
            if (pad <= 0)
                pad += VisualLinkerScript::MD5::BLOCK_SIZE;

            /*
             * Modified from a fixed array to this assignment and memset to be
             * more flexible with block-sizes -- Gray 10/97.
             */
            if (pad > 0) {
                stored[stored_size] = 0x80;
                if (pad > 1)
                    memset(stored + stored_size + 1, 0, pad - 1);
                stored_size += pad;
            }

            /*
             * Put the 64-bit file length in _bits_ (i.e. *8) at the end of the
             * buffer. appears to be in beg-endian format in the buffer?
             */
            unsigned int size_low = ((message_length[0] & 0x1FFFFFFF) << 3);
            memcpy(stored + stored_size, &size_low, sizeof(unsigned int));
            stored_size += sizeof(unsigned int);

            /* shift the high word over by 3 and add in the top 3 bits from the low */
            unsigned int size_high = (message_length[1] << 3) | ((message_length[0] & 0xE0000000) >> 29);
            memcpy(stored + stored_size, &size_high, sizeof(unsigned int));
            stored_size += sizeof(unsigned int);

            /*
             * process the last block of data.
             * if the length of the message was already exactly sized, then we have
             * 2 messages to process
             */
            process_block(stored);
            if (stored_size > VisualLinkerScript::MD5::BLOCK_SIZE)
                process_block(stored + VisualLinkerScript::MD5::BLOCK_SIZE);

            /* Arrange the results into a signature */
            get_result(static_cast<void*>(signature));

            /* store the signature into a readable sring */
            sig_to_string(signature, str, MD5_STRING_SIZE);

            if (signature_ != NULL) {
                memcpy(signature_, static_cast<void*>(signature), MD5_SIZE);
            }

            finished = true;
        }
        else {
            // add error?
        }
    }

    void CMD5Hasher::GetSignature(void* signatureBuffer) {
        if (finished) {
            memcpy(signatureBuffer, signature, MD5_SIZE);
        }
        else {
            //error?
        }
    }

    void CMD5Hasher::GetSignatureString(void* str_) {
        if (finished) {
            memcpy(str_, str, MD5_STRING_SIZE);
        }
        else {
            // error?
        }
    }

    void CMD5Hasher::initialise() {
        /*
         * ensures that unsigned int is 4 bytes on this platform, will need modifying
         * if we are to use on a different sized platform.
         */
        assert(MD5_SIZE == 16);

        A = 0x67452301;
        B = 0xefcdab89;
        C = 0x98badcfe;
        D = 0x10325476;

        message_length[0] = 0;
        message_length[1] = 0;
        stored_size = 0;

        finished = false;
    }

    void CMD5Hasher::process_block(const unsigned char* block) 
    {

        if (message_length[0] + VisualLinkerScript::MD5::BLOCK_SIZE < message_length[0])
            message_length[1]++;
        message_length[0] += BLOCK_SIZE;

        // Copy the block into X. */
        unsigned int X[16];
        for (unsigned int i = 0; i < 16; i++) {
            memcpy(X + i, block + 4 * i, 4);
        }

        unsigned int AA = A, BB = B, CC = C, DD = D;

        VisualLinkerScript::MD5::FF(A, B, C, D, X[0], 0, 0);
        VisualLinkerScript::MD5::FF(D, A, B, C, X[1], 1, 1);
        VisualLinkerScript::MD5::FF(C, D, A, B, X[2], 2, 2);
        VisualLinkerScript::MD5::FF(B, C, D, A, X[3], 3, 3);
        VisualLinkerScript::MD5::FF(A, B, C, D, X[4], 0, 4);
        VisualLinkerScript::MD5::FF(D, A, B, C, X[5], 1, 5);
        VisualLinkerScript::MD5::FF(C, D, A, B, X[6], 2, 6);
        VisualLinkerScript::MD5::FF(B, C, D, A, X[7], 3, 7);
        VisualLinkerScript::MD5::FF(A, B, C, D, X[8], 0, 8);
        VisualLinkerScript::MD5::FF(D, A, B, C, X[9], 1, 9);
        VisualLinkerScript::MD5::FF(C, D, A, B, X[10], 2, 10);
        VisualLinkerScript::MD5::FF(B, C, D, A, X[11], 3, 11);
        VisualLinkerScript::MD5::FF(A, B, C, D, X[12], 0, 12);
        VisualLinkerScript::MD5::FF(D, A, B, C, X[13], 1, 13);
        VisualLinkerScript::MD5::FF(C, D, A, B, X[14], 2, 14);
        VisualLinkerScript::MD5::FF(B, C, D, A, X[15], 3, 15);

        VisualLinkerScript::MD5::GG(A, B, C, D, X[1], 0, 16);
        VisualLinkerScript::MD5::GG(D, A, B, C, X[6], 1, 17);
        VisualLinkerScript::MD5::GG(C, D, A, B, X[11], 2, 18);
        VisualLinkerScript::MD5::GG(B, C, D, A, X[0], 3, 19);
        VisualLinkerScript::MD5::GG(A, B, C, D, X[5], 0, 20);
        VisualLinkerScript::MD5::GG(D, A, B, C, X[10], 1, 21);
        VisualLinkerScript::MD5::GG(C, D, A, B, X[15], 2, 22);
        VisualLinkerScript::MD5::GG(B, C, D, A, X[4], 3, 23);
        VisualLinkerScript::MD5::GG(A, B, C, D, X[9], 0, 24);
        VisualLinkerScript::MD5::GG(D, A, B, C, X[14], 1, 25);
        VisualLinkerScript::MD5::GG(C, D, A, B, X[3], 2, 26);
        VisualLinkerScript::MD5::GG(B, C, D, A, X[8], 3, 27);
        VisualLinkerScript::MD5::GG(A, B, C, D, X[13], 0, 28);
        VisualLinkerScript::MD5::GG(D, A, B, C, X[2], 1, 29);
        VisualLinkerScript::MD5::GG(C, D, A, B, X[7], 2, 30);
        VisualLinkerScript::MD5::GG(B, C, D, A, X[12], 3, 31);

        VisualLinkerScript::MD5::HH(A, B, C, D, X[5], 0, 32);
        VisualLinkerScript::MD5::HH(D, A, B, C, X[8], 1, 33);
        VisualLinkerScript::MD5::HH(C, D, A, B, X[11], 2, 34);
        VisualLinkerScript::MD5::HH(B, C, D, A, X[14], 3, 35);
        VisualLinkerScript::MD5::HH(A, B, C, D, X[1], 0, 36);
        VisualLinkerScript::MD5::HH(D, A, B, C, X[4], 1, 37);
        VisualLinkerScript::MD5::HH(C, D, A, B, X[7], 2, 38);
        VisualLinkerScript::MD5::HH(B, C, D, A, X[10], 3, 39);
        VisualLinkerScript::MD5::HH(A, B, C, D, X[13], 0, 40);
        VisualLinkerScript::MD5::HH(D, A, B, C, X[0], 1, 41);
        VisualLinkerScript::MD5::HH(C, D, A, B, X[3], 2, 42);
        VisualLinkerScript::MD5::HH(B, C, D, A, X[6], 3, 43);
        VisualLinkerScript::MD5::HH(A, B, C, D, X[9], 0, 44);
        VisualLinkerScript::MD5::HH(D, A, B, C, X[12], 1, 45);
        VisualLinkerScript::MD5::HH(C, D, A, B, X[15], 2, 46);
        VisualLinkerScript::MD5::HH(B, C, D, A, X[2], 3, 47);

        VisualLinkerScript::MD5::II(A, B, C, D, X[0], 0, 48);
        VisualLinkerScript::MD5::II(D, A, B, C, X[7], 1, 49);
        VisualLinkerScript::MD5::II(C, D, A, B, X[14], 2, 50);
        VisualLinkerScript::MD5::II(B, C, D, A, X[5], 3, 51);
        VisualLinkerScript::MD5::II(A, B, C, D, X[12], 0, 52);
        VisualLinkerScript::MD5::II(D, A, B, C, X[3], 1, 53);
        VisualLinkerScript::MD5::II(C, D, A, B, X[10], 2, 54);
        VisualLinkerScript::MD5::II(B, C, D, A, X[1], 3, 55);
        VisualLinkerScript::MD5::II(A, B, C, D, X[8], 0, 56);
        VisualLinkerScript::MD5::II(D, A, B, C, X[15], 1, 57);
        VisualLinkerScript::MD5::II(C, D, A, B, X[6], 2, 58);
        VisualLinkerScript::MD5::II(B, C, D, A, X[13], 3, 59);
        VisualLinkerScript::MD5::II(A, B, C, D, X[4], 0, 60);
        VisualLinkerScript::MD5::II(D, A, B, C, X[11], 1, 61);
        VisualLinkerScript::MD5::II(C, D, A, B, X[2], 2, 62);
        VisualLinkerScript::MD5::II(B, C, D, A, X[9], 3, 63);

        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    void CMD5Hasher::get_result(void* result) {
        memcpy((char*)result, &A, sizeof(unsigned int));
        memcpy((char*)result + sizeof(unsigned int), &B, sizeof(unsigned int));
        memcpy((char*)result + 2 * sizeof(unsigned int), &C, sizeof(unsigned int));
        memcpy((char*)result + 3 * sizeof(unsigned int), &D, sizeof(unsigned int));
    }

    void sig_to_string(const void* signature_, char* str_, const int str_len) {
        unsigned char* sig_p;
        char* str_p;
        char* max_p;
        unsigned int high, low;

        str_p = str_;
        max_p = str_ + str_len;

        for (sig_p = (unsigned char*)signature_; sig_p < (unsigned char*)signature_ + MD5_SIZE; sig_p++) {
            high = *sig_p / 16;
            low = *sig_p % 16;
            /* account for 2 chars */
            if (str_p + 1 >= max_p) {
                break;
            }
            *str_p++ = VisualLinkerScript::MD5::HEX_STRING[high];
            *str_p++ = VisualLinkerScript::MD5::HEX_STRING[low];
        }
        /* account for 2 chars */
        if (str_p < max_p) {
            *str_p++ = '\0';
        }
    }

    void sig_from_string(void* signature_, const char* str_) {
        unsigned char* sig_p;
        const char* str_p;
        char* hex;
        unsigned int high, low, val;

        hex = (char*)VisualLinkerScript::MD5::HEX_STRING;
        sig_p = static_cast<unsigned char*>(signature_);

        for (str_p = str_; str_p < str_ + MD5_SIZE * 2; str_p += 2) {
            high = strchr(hex, *str_p) - hex;
            low = strchr(hex, *(str_p + 1)) - hex;
            val = high * 16 + low;
            *sig_p++ = val;
        }
    }
} 