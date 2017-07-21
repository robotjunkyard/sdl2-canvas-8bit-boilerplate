# sdl2-canvas-8bit-boilerplate

SDL2's support for 8-bit palettized surfaces seems unreliable and 
inconsistent, so this is a custom solution for that problem.

Canvas::updateSDLTexture() is what ultimately blits the Canvas's
pixmap to SDL itself, using the palette that's assigned to a particular
Canvas-type object.  The example Palette used in this template code
is a slight modification of the 32-color 'db32' palette by Richard Fhager,
shifted one up since color 0 is treated as 'Transparency' by the
Canvas::blitBitmapMasked() method.
