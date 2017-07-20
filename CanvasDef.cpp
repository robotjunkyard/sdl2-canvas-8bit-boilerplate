#include "CanvasDef.hpp"
#include <tuple>
#include <utility>

void Canvas::updateSDLTexture(SDL_Texture* sdlTexture) const
{
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            const unsigned int idx = (width() * y) + x;
            const byte color8 = _pixels[idx];
            const uint32_t truecolor = _palette.getColor(color8).as_uint32();

            _destPixels32[idx] = truecolor;
        }
    }

    SDL_UpdateTexture(sdlTexture, NULL, _destPixels32, _width * 4); // oh!  pitch is BYTES, not PIXELS!  so... "* 4" !
}

void Canvas::blitBitmapMasked(const Bitmap& bitmap, int destx, int desty)
{
    // figure out clipping
    const int canvW = width(),
              canvH = height(),
              sourceWidth = bitmap.width(),
              sourceHeight = bitmap.height(),
              topSpriteCanvasX = destx,
              topSpriteCanvasY = desty,
              endSpriteCanvasX = destx + sourceWidth,
              endSpriteCanvasY = desty + sourceHeight,
              topSpriteClippedX = (topSpriteCanvasX < 0) ? 0 : topSpriteCanvasX,
              topSpriteClippedY = (topSpriteCanvasY < 0) ? 0 : topSpriteCanvasY,
              endSpriteClippedX = std::min<int>(canvW, endSpriteCanvasX),
              endSpriteClippedY = std::min<int>(canvH, endSpriteCanvasY),
              bdx = std::max<int>(destx, 0), // beginning X offset of dest canvas
        bdy = std::max<int>(desty, 0), // beginning Y offset of dest canvas
        axB = topSpriteClippedX - topSpriteCanvasX, // beginning X offset of source bitmap
        ayB = topSpriteClippedY - topSpriteCanvasY, // beginning Y offset of source bitmap
        s_across_x = endSpriteClippedX - topSpriteClippedX,
              s_across_y = endSpriteClippedY - topSpriteClippedY;

    for (int y = 0; y < s_across_y; y++) {
        for (int x = 0; x < s_across_x; x++) {
            const int canvas_dest_x = x + bdx,
                      canvas_dest_y = y + bdy,
                      sprite_src_x = x + axB,
                      sprite_src_y = y + ayB;

            const byte canvasPixel = _pixels[(canvas_dest_y * width()) + canvas_dest_x],
                       spritePixel = bitmap.pixelAt(sprite_src_x, sprite_src_y),
                       mask = reduceToMask(spritePixel),
                       mixed = (spritePixel & mask) ^ (canvasPixel & ~mask);

            setPixel(canvas_dest_x, canvas_dest_y, mixed);
        }
    }
}

void Canvas::blitBitmapNonmasked(const Bitmap& bitmap, int destx, int desty)
{
    // figure out clipping
    const int canvW = width(),
              canvH = height(),
              sourceWidth = bitmap.width(), sourceHeight = bitmap.height(),
              topSpriteCanvasX = destx,
              topSpriteCanvasY = desty,
              endSpriteCanvasX = destx + sourceWidth,
              endSpriteCanvasY = desty + sourceHeight,
              topSpriteClippedX = (topSpriteCanvasX < 0) ? 0 : topSpriteCanvasX,
              topSpriteClippedY = (topSpriteCanvasY < 0) ? 0 : topSpriteCanvasY,
              endSpriteClippedX = std::min<int>(canvW, endSpriteCanvasX),
              endSpriteClippedY = std::min<int>(canvH, endSpriteCanvasY);

    const int bdx = std::max<int>(destx, 0), // beginning X offset of dest canvas
        bdy = std::max<int>(desty, 0), // beginning Y offset of dest canvas
        axB = topSpriteClippedX - topSpriteCanvasX, // beginning X offset of source bitmap
        ayB = topSpriteClippedY - topSpriteCanvasY, // beginning Y offset of source bitmap
        s_across_x = endSpriteClippedX - topSpriteClippedX, s_across_y = endSpriteClippedY - topSpriteClippedY;

    if (s_across_x <= 0) {
        return;
    }

    for (int y = 0; y < s_across_y; y++) {
        const int canvas_dest_y = y + bdy,
                  canvas_dest_x = bdx,
                  sprite_src_y = y + ayB,
                  sprite_src_x = axB;

        byte* const canvasRowBegin = &_pixels[(canvas_dest_y * width()) + canvas_dest_x];
        const byte* const spriteRowBegin = bitmap.pixelPtrAt(sprite_src_x, sprite_src_y);

        memcpy(canvasRowBegin, spriteRowBegin, s_across_x);
    }
}

void Palette::swapRedBlue()
{
    for (auto& color : _colors)
        std::swap(color.r, color.b);
    for (auto& color : _initColors)
        std::swap(color.r, color.b);
}
