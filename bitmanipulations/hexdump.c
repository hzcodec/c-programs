/*
    Auther       : Heinz Samuelsson
    Date         : 2017-11-08
    File         : hexdump.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];       // stores the ASCII data
    unsigned char *pc = addr;     // cast to make the code cleaner.

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {

        // Multiple of 16 means new line (with line offset).
        if ((i % 16) == 0) {

            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}
 
int main(int argc, char *argv[])
{
 
    double d1 = 234.5;
    char s1[] = "a 15char string";
    char s2[] = "This is a slightly longer string";

    unsigned int kalle = 0x1234;
    int olle = 0xffaa;
    uint32_t pelle = 0x66442288;
    uint64_t stisse = 0x1122334455667788;

    hexDump("d1", &d1, sizeof d1);
    hexDump("s1", &s1, sizeof s1);
    hexDump("s2", &s2, sizeof s2);

    hexDump("kalle", &kalle, sizeof(kalle));
    hexDump("olle", &olle, sizeof(olle));
    hexDump("pelle", &pelle, sizeof(pelle));
    hexDump("stisse", &stisse, sizeof(stisse));

    return 0;
}
