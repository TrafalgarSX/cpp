#ifndef _FONT_CACHE_H_
#define _FONT_CACHE_H_

#include <string_view>
#include <vector>
#include <iostream>

#include <imgui.h>

struct font_atlas_cache
{
    ImVec2 TexUvWhitePixel;
    int out_width;
    int out_height;
    std::unique_ptr<unsigned char[]> image_data;
    std::vector<ImFontGlyph> glyphs;
    ImVec4 TexUvLines[IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];
};


void save_font_atlas(std::string_view path, bool override_if_exists = false, ImFontAtlas* font_atlas = nullptr);
font_atlas_cache load_font_atlas_cache(std::string_view path);
void restore_font_atlas(font_atlas_cache& cache, ImFontAtlas* font_atlas = nullptr);


#endif