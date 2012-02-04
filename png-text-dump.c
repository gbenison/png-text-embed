#include <stdio.h>
#include <png.h>

int
main(int argc, char *argv[])
{
  FILE *infile = stdin;
  if (argc > 1)
    infile = fopen(argv[1], "r");

  if (infile == NULL)
    {
      fprintf(stderr, "Could not open %s\n", argv[1]);
      return 1;
    }

  /* allocate png structures */
  png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
						NULL, NULL, NULL);

  png_infop read_info_ptr = png_create_info_struct(read_ptr);
  png_infop end_info_ptr = png_create_info_struct(read_ptr);

  png_init_io(read_ptr, infile);
  png_read_png(read_ptr, read_info_ptr, 0, NULL);
  
  png_textp text_ptr;
  int num_text;
  png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text);

  /* echo text contents. */
  int i;
  for (i = 0; i < num_text; ++i)
    {
      printf("==== text chunk %d ====\n", i + 1);
      printf("%s -> %s\n\n", text_ptr[i].key, text_ptr[i].text);
    }

  return 0;
}
