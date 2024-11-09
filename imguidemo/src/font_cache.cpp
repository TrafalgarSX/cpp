#include <font_cache.h>

#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <iostream>
#include <string_view>
#include <vector>


// TexUvWhitePixel
// TexUvLines
// n (size of glyphs)
// n * ImFontGlyph...
// out_width
// out_height
// TexDataAlpha8
void save_font_atlas(std::string_view path, bool override_if_exists,
                     ImFontAtlas *font_atlas) {
  if (font_atlas == nullptr)
    font_atlas = ImGui::GetIO().Fonts;

  if (std::filesystem::exists(path) && !override_if_exists)
    throw std::runtime_error("Target already exists");

  std::ofstream file(path.data(), std::ios::out | std::ios::binary);

  file.write(reinterpret_cast<const char *>(&font_atlas->TexUvWhitePixel),
             sizeof(font_atlas->TexUvWhitePixel));
  file.write(reinterpret_cast<const char *>(font_atlas->TexUvLines),
             sizeof(font_atlas->TexUvLines));

  auto size = font_atlas->Fonts[0]->Glyphs.size();
  file.write(reinterpret_cast<const char *>(&size), sizeof(int));
  file.write(reinterpret_cast<const char *>(font_atlas->Fonts[0]->Glyphs.Data),
             sizeof(ImFontGlyph) * size);

  unsigned char *out_pixels{};
  int out_width{};
  int out_height{};
  font_atlas->GetTexDataAsAlpha8(&out_pixels, &out_width, &out_height);
  file.write(reinterpret_cast<const char *>(&out_width), sizeof(int));
  file.write(reinterpret_cast<const char *>(&out_height), sizeof(int));
  file.write(reinterpret_cast<const char *>(out_pixels),
             out_width * out_height);
}

font_atlas_cache load_font_atlas_cache(std::string_view path) {
  if (!std::filesystem::exists(path))
    throw std::runtime_error("Target doesn't exists");

  std::ifstream file(path.data(), std::ios::in | std::ios::binary);

  font_atlas_cache cache{};

  file.read(reinterpret_cast<char *>(&cache.TexUvWhitePixel),
            sizeof(cache.TexUvWhitePixel));
  file.read(reinterpret_cast<char *>(cache.TexUvLines),
            sizeof(cache.TexUvLines));

  int glyph_size{};
  file.read(reinterpret_cast<char *>(&glyph_size), sizeof(int));
  cache.glyphs.resize(glyph_size);
  cache.glyphs.shrink_to_fit();
  file.read(reinterpret_cast<char *>(cache.glyphs.data()),
            sizeof(ImFontGlyph) * glyph_size);

  file.read(reinterpret_cast<char *>(&cache.out_width), sizeof(int));
  file.read(reinterpret_cast<char *>(&cache.out_height), sizeof(int));

  cache.image_data =
      std::make_unique<unsigned char[]>(cache.out_width * cache.out_height);
  file.read(reinterpret_cast<char *>(cache.image_data.get()),
            cache.out_width * cache.out_height);

  return cache;
}

void restore_font_atlas(font_atlas_cache &cache, ImFontAtlas *font_atlas) {
  if (font_atlas == nullptr)
    font_atlas = ImGui::GetIO().Fonts;

  ImFontConfig dummy_config{};
  dummy_config.FontData = new unsigned char;
  dummy_config.FontDataSize = 1;
  dummy_config.SizePixels = 1;
  auto *font = font_atlas->AddFont(&dummy_config);
  font->FontSize = 16.0F;
  font->ConfigData = &dummy_config;
  font->ConfigDataCount = 1;
  font->ContainerAtlas = font_atlas;

  font_atlas->ClearTexData();
  font_atlas->TexPixelsAlpha8 = cache.image_data.release();
  font_atlas->TexWidth = cache.out_width;
  font_atlas->TexHeight = cache.out_height;
  font_atlas->TexUvWhitePixel = cache.TexUvWhitePixel;
  std::memcpy(font_atlas->TexUvLines, cache.TexUvLines,
              sizeof(cache.TexUvLines));

  for (auto &&glyph : cache.glyphs) {
    font->AddGlyph(&dummy_config, glyph.Codepoint, glyph.X0, glyph.Y0, glyph.X1,
                   glyph.Y1, glyph.U0, glyph.V0, glyph.U1, glyph.V1,
                   glyph.AdvanceX);
    font->SetGlyphVisible(glyph.Codepoint, glyph.Visible);
  }

  font->BuildLookupTable();
  font_atlas->TexReady = true;
}

void font_cache(const std::string &font_path, std::string_view cache_path,
                bool override_if_exists) {
  ImFontConfig config;
  config.OversampleH = 1;
  config.OversampleV = 1;
  config.SizePixels = 16;
  config.RasterizerMultiply = 2.F;

  ImGuiIO io = ImGui::GetIO();

  io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;

  const ImWchar range[] = {0x20, 0xFFFF, 0x0};

  io.Fonts->AddFontFromFileTTF(font_path.c_str(), 0, &config, range);
  io.Fonts->Build();
  save_font_atlas(cache_path, override_if_exists);
}