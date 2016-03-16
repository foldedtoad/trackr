/*----------------------------------------------------------------------------*/
/*  dump.c                                                                    */
/*----------------------------------------------------------------------------*/

#include <stdint.h>
#include <ctype.h>

#include "dbglog.h"

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static char makechar(uint8_t byte)
{
  return (isprint(byte)) ? (char) byte : (char) '.';
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
void dump_bytes(uint8_t * Buffer, int Length)
{
   int    i;
   int    wholelines;
   uint8_t * ptr;

   if (Length == 0) {
      puts("");
      return;
   }

   ptr = Buffer;
   wholelines = Length / 16;

   for (i=0; i < wholelines; i++) {
      printf("%05X: %02X %02X %02X %02X %02X %02X %02X %02X "
                   "%02X %02X %02X %02X %02X %02X %02X %02X  ",
                (ptr-Buffer),
                (unsigned) ptr[0], (unsigned) ptr[1], 
                (unsigned) ptr[2], (unsigned) ptr[3],
                (unsigned) ptr[4], (unsigned) ptr[5], 
                (unsigned) ptr[6], (unsigned) ptr[7],
                (unsigned) ptr[8], (unsigned) ptr[9], 
                (unsigned) ptr[10],(unsigned) ptr[11],
                (unsigned) ptr[12],(unsigned) ptr[13],
                (unsigned) ptr[14],(unsigned) ptr[15] );

            printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
                makechar(ptr[0]),  makechar(ptr[1]), 
                makechar(ptr[2]),  makechar(ptr[3]),
                makechar(ptr[4]),  makechar(ptr[5]), 
                makechar(ptr[6]),  makechar(ptr[7]),
                makechar(ptr[8]),  makechar(ptr[9]), 
                makechar(ptr[10]), makechar(ptr[11]),
                makechar(ptr[12]), makechar(ptr[13]),
                makechar(ptr[14]), makechar(ptr[15]) );

      ptr    += 16;
      Length -= 16;
   }

   if (Length) {

      printf("%05X: ", (ptr-Buffer));
      for (i=0; i < Length; i++) {
         printf("%02X ", (unsigned) ptr[i]);
      }

      for (i=0; i < (16 - Length); i++) {
         printf("%c%c%c", ' ',' ',' ');
      }
      printf("%c", ' ');

      for (i=0; i < Length; i++) {
         printf("%c", makechar(ptr[i]));
      }

      puts("");
   }
   else
      puts("");
}
