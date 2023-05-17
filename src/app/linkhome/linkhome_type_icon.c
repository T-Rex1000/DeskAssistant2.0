/*******************************************************************************
 * Size: 16 px
 * Bpp: 2
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#ifndef LINKHOME_TYPE_ICON
#define LINKHOME_TYPE_ICON 1
#endif

#if LINKHOME_TYPE_ICON

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E60F "" */
    0x0, 0x7f, 0xd0, 0x0, 0x38, 0x2, 0xc0, 0xc,
    0x0, 0x3, 0x2, 0x4b, 0x40, 0x18, 0x32, 0xc0,
    0x0, 0xc3, 0x34, 0x0, 0xc, 0x22, 0x0, 0x0,
    0x83, 0x0, 0x0, 0xc, 0x24, 0x0, 0x1, 0x80,
    0xc0, 0x0, 0x30, 0x7, 0x0, 0xd, 0x0, 0x18,
    0x2, 0x40, 0x0, 0xbf, 0xe0, 0x0, 0x1, 0x54,
    0x0, 0x0, 0x15, 0x40, 0x0, 0x2, 0xf8, 0x0,

    /* U+E615 "" */
    0x0, 0x20, 0x0, 0x0, 0x1c, 0x0, 0x0, 0xb,
    0x0, 0x0, 0x2, 0xc0, 0x0, 0x0, 0x70, 0xa4,
    0x0, 0xa, 0xff, 0x8b, 0xff, 0x80, 0x0, 0xbd,
    0x30, 0x0, 0x0, 0xe, 0x0, 0x0, 0x3, 0xc0,
    0x0, 0x0, 0xe0, 0x0, 0x0, 0x30, 0x0, 0x0,
    0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 14, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 56, .adv_w = 256, .box_w = 13, .box_h = 13, .ofs_x = 2, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x6
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 58895, .range_length = 7, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t linkhome_type_icon = {
#else
lv_font_t linkhome_type_icon = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LINKHOME_TYPE_ICON*/

