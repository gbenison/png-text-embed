
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#define PNG_SIG_SIZE 8
const char* png_sig = "\211\120\116\107\015\012\032\012";

int little_endian = 0;

static void
endian_swap(uint32_t* x)
{
  if (little_endian)
    {
      char* buf = (char*)x;
      char tmp;
      tmp = buf[0]; buf[0]=buf[3]; buf[3] = tmp;
      tmp = buf[1]; buf[1]=buf[2]; buf[2] = tmp;
    }
}

int
main(int argc, char *argv[])
{
  /* is this a little-endian machine? */
  {
    uint32_t endian_test = 1;
    little_endian = ((*(char*)(&endian_test)) != 0);
  }

  FILE *infile = stdin;
  if (argc > 1) infile = fopen(argv[1], "r");
  assert(infile != NULL);

  char sig[PNG_SIG_SIZE];
  assert (fread(sig, 1, PNG_SIG_SIZE, infile) == PNG_SIG_SIZE);

  assert (strncmp(png_sig, sig, PNG_SIG_SIZE) == 0);

  while(1) {
    uint32_t length;
    char name[4];
    uint32_t crc;

    int n_read_length = fread(&length, 1, 4, infile);
    if (n_read_length != 4) break;

    endian_swap(&length);
    fread(name, 1, 4, infile);

    fseek(infile, length, SEEK_CUR);
    fread(&crc, 1, 4, infile);
    endian_swap(&crc);

  }

  return(0);
}

