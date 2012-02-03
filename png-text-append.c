
#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int
main(int argc, char *argv[])
{
  if (argc < 3)
    {
      fprintf(stderr,
	      "Usage: %s <key> <value>\n\nReads a .png file from standard input, appends a text chunk, writes to standard output",
	      argv[0]);
      return(1);
    }

  /* allocate png structures */
  png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
						NULL, NULL, NULL);

  png_infop read_info_ptr = png_create_info_struct(read_ptr);
  png_infop end_info_ptr = png_create_info_struct(read_ptr);

  png_init_io(read_ptr, stdin);
  png_read_info(read_ptr, read_info_ptr);


  /* allocate png write structures */
  png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
						  NULL, NULL, NULL);

  png_infop write_info_ptr = png_create_info_struct(write_ptr);
  png_infop write_end_info_ptr = png_create_info_struct(write_ptr);

  png_init_io(write_ptr, stdout);

  /* copy header information to output image */
  png_uint_32 width, height;
  int bitdepth, color_type, interlace_type, compression_type, filter_type;
  png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bitdepth, &color_type, &interlace_type, &compression_type, &filter_type);
  png_set_IHDR(write_ptr,
	       write_info_ptr,
	       width,
	       height,
	       bitdepth,
	       PNG_COLOR_TYPE_RGB,
	       PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_DEFAULT,
	       PNG_FILTER_TYPE_DEFAULT);

  png_textp text_ptr;
  int num_text;
  png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text);

  /* FIXME read image */
  png_set_keep_unknown_chunks(read_ptr, PNG_HANDLE_CHUNK_ALWAYS, NULL, 0);

  png_read_png(read_ptr, read_info_ptr, 0, NULL);

  png_read_end(read_ptr, end_info_ptr);

  /* Allocate new text structure, making room for one more */
  png_textp new_text_ptr = (png_textp)malloc(sizeof(png_text) * (num_text + 1));
  {
    int i;
    for (i = 0; i < num_text; ++i)
      new_text_ptr[i] = text_ptr[i];
  }

  memset(&new_text_ptr[num_text], 0, sizeof(png_text));
  new_text_ptr[num_text].compression = PNG_TEXT_COMPRESSION_NONE;
  new_text_ptr[num_text].key         = argv[1];
  new_text_ptr[num_text].text        = argv[2];
  new_text_ptr[num_text].text_length = strlen(argv[2]);

  png_set_text(write_ptr, write_info_ptr, new_text_ptr, num_text + 1);

  png_write_info(write_ptr, write_info_ptr);

  /* space for image data */


  png_write_end(write_ptr, write_end_info_ptr);

  return 0;
}
