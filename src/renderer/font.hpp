#pragma once
#include "msdf-atlas-gen/FontGeometry.h"
#include "renderer/texture.hpp"
namespace mamba::Renderer {

class Font {
  public:
    static Font create();
    const Texture& getAtlasTexture() const { return m_texture; }

  private:
    Font(Texture&& texture, msdf_atlas::FontGeometry&& geometry)
        : m_texture(std::move(texture)), m_font_geometry(std::move(geometry)) {}
    Texture m_texture;
    msdf_atlas::FontGeometry m_font_geometry;
};
} // namespace mamba::Renderer
