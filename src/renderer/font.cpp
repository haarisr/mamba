
#include "font.hpp"
#include "ext/import-font.h"
#include "fonts/fonts.hpp"
#include "msdf-atlas-gen/FontGeometry.h"
#include "renderer/texture.hpp"
#include <cstdint>
#include <stdexcept>

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdfgen-ext.h>
#include <msdfgen.h>

namespace mamba::Renderer {

Font Font::create() {

    auto ft = msdfgen::initializeFreetype();
    if (!ft)
        throw std::runtime_error("Cannot load freetype");
    auto font = msdfgen::loadFontData(ft, Fonts::ROBOTO.data(), Fonts::ROBOTO.size());
    if (!font)
        throw std::runtime_error("Cannot load font");

    // --- Charset (ASCII) ---
    msdf_atlas::Charset charset;
    for (uint32_t c = 32; c < 128; ++c)
        charset.add(c);

    // --- Font geometry ---
    // constexpr double em_size = 40.0;
    // constexpr double pixel_range = 4.0;
    // constexpr double miter = 2.0;
    // constexpr int padding = 2;

    msdf_atlas::FontGeometry font_geometry;
    font_geometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);

    // // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
    // const double maxCornerAngle = 3.0;
    // for (GlyphGeometry& glyph : glyphs)
    //     glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

    // TightAtlasPacker class computes the layout of the atlas.
    msdf_atlas::TightAtlasPacker packer;
    // setScale for a fixed size or setMinimumScale to use the largest that fits
    packer.setMinimumScale(24.0);
    // setPixelRange or setUnitRange
    packer.setPixelRange(2.0);
    packer.setMiterLimit(1.0);
    // Compute atlas layout - pack glyphs

    const auto& glyphs = font_geometry.getGlyphs();
    packer.pack(const_cast<msdf_atlas::GlyphGeometry*>(glyphs.begin()), glyphs.size());

    // Get final atlas dimensions
    int width = 0, height = 0;
    packer.getDimensions(width, height);

    // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
    msdf_atlas::ImmediateAtlasGenerator<
        float, // pixel type of buffer for individual glyphs depends on generator function
        4,     // number of atlas color channels
        msdf_atlas::mtsdfGenerator, // function to generate bitmaps for individual glyphs
        msdf_atlas::BitmapAtlasStorage<uint8_t, 4> // class that stores the atlas bitmap
        >
        generator(width, height);

    // GeneratorAttributes can be modified to change the generator's default settings.
    msdf_atlas::GeneratorAttributes attributes;
    generator.setAttributes(attributes);
    generator.setThreadCount(4);
    // Generate atlas bitmap
    generator.generate(glyphs.begin(), glyphs.size());

    const auto& bitmap = static_cast<msdfgen::BitmapConstRef<uint8_t, 4>>(generator.atlasStorage());

    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(ft);

    return Font(Texture::create(bitmap.pixels, bitmap.width, bitmap.height, 4),
                std::move(font_geometry));
}

} // namespace mamba::Renderer
