#ifndef SIMPLEGUI_COLOR_H
#define SIMPLEGUI_COLOR_H

#include <stdint.h>



/*
* Helper class to make color calculations easier
* Uses 565 RGB model
*/
class Color {

    public:

    static const uint8_t RMAX = 0x1f;
    static const uint8_t GMAX = 0x3f;
    static const uint8_t BMAX = 0x1f;

    static uint8_t r(uint16_t c) {
        return (c & 0xf800) >> 11;
    }

    static uint8_t g(uint16_t c) {
        return (c & 0x07e0) >> 5;
    }

    static uint8_t b(uint16_t c) {
        return (c & 0x001f);
    }

    static uint8_t r8(uint32_t c) {
        return (c & 0xff0000) >> 16;
    }

    static uint8_t g8(uint32_t c) {
        return (c & 0x00ff00) >> 8;
    }

    static uint8_t b8(uint32_t c) {
        return (c & 0x0000ff);
    }

    static uint16_t color(uint8_t r, uint8_t g, uint8_t b) {
        return (r << 11) + (g << 5) + b;
    }

    /*
    * Map 888 to 565
    */
    static uint16_t map(uint8_t r, uint8_t g, uint8_t b) {
        return colorPercent((r * 100) / 255, (g * 100) / 255, (b * 100) / 255);
    }

    static uint16_t map(uint32_t c) {
        return colorPercent((r8(c) * 100) / 255, (g8(c) * 100) / 255, (b8(c) * 100) / 255);
    }

    static uint16_t colorPercent(uint8_t r, uint8_t g, uint8_t b) {

        if(r > 100) r = 100;
        if(g > 100) g = 100;
        if(b > 100) b = 100;

        return Color::color(
            (Color::RMAX * r) / 100,
            (Color::GMAX * g) / 100,
            (Color::BMAX * b) / 100
        );
    }
    /*
    * Return a new color value with each component shaded towards its
    * maximum value for 100%
    * */
    static uint16_t fadeToWhite(uint16_t c, uint8_t percent) {
        uint8_t r = Color::r(c);
        uint8_t g = Color::g(c);
        uint8_t b = Color::b(c);
        uint16_t dr = Color::RMAX - r;
        uint16_t dg = Color::GMAX - g;
        uint16_t db = Color::BMAX - b;

        if(percent > 100) percent = 100;

        return Color::color(
            r + (dr * percent) / 100,
            g + (dg * percent) / 100,
            b + (db * percent) / 100
        );
    }
    /*
    * Return a new color value with each component shaded towards its
    * minimum value for 100%
    * */
    static uint16_t fadeToBlack(uint16_t c, uint8_t percent) {
        uint8_t r = Color::r(c);
        uint8_t g = Color::g(c);
        uint8_t b = Color::b(c);

        if(percent > 100) percent = 100;

        return Color::color(
            r - (r * percent) / 100,
            g - (g * percent) / 100,
            b - (b * percent) / 100
        );
    }
};

#endif