/**
 * recover.c
 * 
 * By Lee-Loi Chieng
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 512
// use stdint.h library referenced in bmp.h to use typedef
typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    // open input file 
    FILE* memorycard = fopen("card.raw", "r");
    if (memorycard == NULL)
    {
        printf("Could not open Memory card.\n");
        return 2;
    }
    // read 512 byte buffer blocks
    BYTE BUFFER[SIZE];
    
    // initalise an outfile
    FILE* outfile = NULL;
    
    // keep track of jpegs
    int jpgcount = 0;
    
    while (!feof(memorycard))
    {
        fread(&BUFFER, sizeof(BYTE), SIZE, memorycard);
        
        if (feof(memorycard))
        {
            fclose(outfile);
            fclose(memorycard);
            return 0;
        }

        // find beginning of jpeg first 3 bytes
        if (BUFFER[0] == 0xff && BUFFER[1] == 0xd8 && BUFFER[2] == 0xff)
        {
            // checks fourth jpg byte is 0xe# (# = 0-9 and a-f)
            if (BUFFER[3] >= 0xe0 && BUFFER[3] <= 0xef)
            
            {
                // checks if outfile is  open
                if (outfile != NULL)
                {
                    fclose(outfile);
                }
                // create a new jpg, use jpg count for filename        
                char title[8];
                sprintf(title, "%03d.jpg", jpgcount);
                outfile = fopen(title, "w");
        
                // update jpgcount
                jpgcount++;
        
                // writing to jpeg
                fwrite (&BUFFER[0], sizeof(BUFFER), 1, outfile);
            }
               
        }
        else if (outfile != NULL)
        {
        // if in process of writing blocks, keep writing
            fwrite (&BUFFER[0], sizeof(BUFFER), 1, outfile);
        }
    }
}
