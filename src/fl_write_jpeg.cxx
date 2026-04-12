//
// Fl_JPEG_Image support functions for the Fast Light Tool Kit (FLTK).
//
// Copyright 2005-2025 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

#include <config.h>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/fl_utf8.h>               // fl_fopen()
#include <stdio.h>

extern "C" {
#ifdef HAVE_LIBJPEG
#  include <jpeglib.h>
#endif // HAVE_LIBJPEG
} // extern "C"

/**
  \file fl_write_jpeg.cxx

  JPEG image support functions.

*/

/**
  Write an RGB image to a JPEG image file.

  This is a very basic and restricted function to create a JPEG image file
  from an RGB image (Fl_RGB_Image).

  The image data must be aligned w/o gaps, i.e. ld() \b MUST be zero or
  equal to data_w() * d().

  The image file is always written with the original image size data_w()
  and data_h(), even if the image has been scaled.

  Image depth 1 (gray) and 3 (RGB) are supported. Images with depth 2 or 4
  (with alpha channel) are not supported by the JPEG format.

  \note Currently there is no error handling except for errors when opening
    the file. This may be changed in the future.

  \param[in]  filename  Output filename, extension should be '.jpg' or '.jpeg'
  \param[in]  img       RGB image to be written

  \return     success (0) or error code: negative values are errors

  \retval      0        success, file has been written
  \retval     -1        jpeg library not available
  \retval     -2        file open error
  \retval     -3        unsupported image depth (alpha channel not supported)

  \see fl_write_jpeg(const char *, const char *, int, int, int, int)
*/

int fl_write_jpeg(const char *filename, Fl_RGB_Image *img) {
  return fl_write_jpeg(filename,
                       img->data()[0],
                       img->data_w(),
                       img->data_h(),
                       img->d(),
                       img->ld());
}

/**
  Write raw image data to a JPEG image file.

  \see fl_write_jpeg(const char *filename, const char *pixels, int w, int h, int d, int ld)
*/
int fl_write_jpeg(const char *filename, const unsigned char *pixels, int w, int h, int d, int ld) {
  return fl_write_jpeg(filename, (const char *)pixels, w, h, d, ld);
}

/**
  Write raw image data to a JPEG image file.

  This is a very basic and restricted function to create a JPEG image file
  from raw image data, e.g. a screenshot.

  The image data must be aligned w/o gaps after each row (ld = 0 or ld = w * d)
  or \p ld must be the total length of each row, i.e. w * d + gapsize.
  If ld == 0 then ld = w * d is assumed.

  The total data size must be (w * d + gapsize) * h = ld' * h
  where ld' = w * d if ld == 0.

  Image depth 1 (gray) and 3 (RGB) are supported. Images with depth 2 or 4
  (with alpha channel) are not supported by the JPEG format.

  For further restrictions and return values please see
  fl_write_jpeg(const char *filename, Fl_RGB_Image *img).

  \param[in]  filename  Output filename, extension should be '.jpg' or '.jpeg'
  \param[in]  pixels    Image data
  \param[in]  w         Image data width
  \param[in]  h         Image data height
  \param[in]  d         Image depth: 1 = GRAY, 3 = RGB (2 and 4 with alpha not supported)
  \param[in]  ld        Line delta: default (0) = w * d

  \return     success (0) or error code, see ...

  \see fl_write_jpeg(const char *filename, Fl_RGB_Image *img)
*/
int fl_write_jpeg(const char *filename, const char *pixels, int w, int h, int d, int ld) {

#ifdef HAVE_LIBJPEG

  FILE *fp;
  J_COLOR_SPACE color_space;

  // JPEG does not support alpha channels
  if (d == 2 || d == 4) {
    return -3;
  }

  if ((fp = fl_fopen(filename, "wb")) == NULL) {
    return -2;
  }

  switch (d) {
    case 1:  color_space = JCS_GRAYSCALE; break;
    case 3:  color_space = JCS_RGB;       break;
    default: color_space = JCS_RGB;
  }

  if (ld == 0)
    ld = w * d;

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, fp);

  cinfo.image_width = w;
  cinfo.image_height = h;
  cinfo.input_components = d;
  cinfo.in_color_space = color_space;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 95, TRUE);  // Quality 95 is a good balance

  jpeg_start_compress(&cinfo, TRUE);

  JSAMPROW row_pointer;
  const char *ptr = pixels;

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW)ptr;
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    ptr += ld;
  }

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  fclose(fp);
  return 0;

#else
  return -1;
#endif
}
