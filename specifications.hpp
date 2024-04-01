#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#undef main

struct RGBA_t {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;

	operator uint32_t() const {
		// "erm you should actually use SDL_MapRGB() and SDL_GetRGB()"
		// shut up nerd
		// ASSUMING PIXELS ARE STORED IN ARGB FORMAT
		// wtf kind of fucked up format...
		// its not even ARGB its like xRGB, first byte does jack shit im pretty sure
		return 
			  ((A&0xff) << 24)
			| ((R&0xff) << 16)
			| ((G&0xff) << 8 )
			| ((B&0xff)      );
	}
};

namespace ball
{
	/** PROGRAM SPECIFICATIONS **/

	// SCREEN WIDTH AND HEIGHT MUST BE THE SAME
	// preferably also a multiple of 16

	// screen width
	static constexpr uint32_t SCREENWIDTH = 1040;
	// screen height
	static constexpr uint32_t SCREENHEIGHT = 1040;

	// mandelbrot stuff
	// 512, -1.5,  0.7, -1.0, 1.0
	static uint64_t maxIter = 512; // max iterations
	static double minReal = -1.5; // min real val
	static double maxReal = 0.7; // max real val
	static double minImag = -1.0; // min imaginary val
	static double maxImag = 1.0; // max imaginary val

	// how much to zoom in
	//static constexpr double zoomFactor = .9;
	// how much zoomed in
	static double currentZoomFactor = 1.5;
	// center of screen
	static double screenCenter[2] = {-.75,0};

	// any mandelbrot val under this wont be re-rendered if fast render is being used
	static uint32_t fastRenderVal = 9;
	// coloring method
	static uint16_t colorMethod = 0;

	// amount of threads used
	static constexpr uint16_t THREAD_AMOUNT = 16;

	static SDL_Window* WINDOW = NULL;
	static SDL_Renderer* RENDERER = NULL;
	static SDL_Surface* SURFACE = NULL;

	static RGBA_t PIXELS[SCREENHEIGHT][SCREENWIDTH] {};
	static uint32_t lastMandel[SCREENHEIGHT][SCREENWIDTH] {};
}

/** USEFUL CODE **/

static constexpr double PI = 3.1415926535929;

#define loop(x) for (uint32_t i = 0; i < x; ++i)

namespace Colors {
	static RGBA_t white   = {255,255,255,255};
	static RGBA_t black   = {0  ,0  ,0  ,255};
	static RGBA_t red     = {255,0  ,0  ,255};
	static RGBA_t green   = {0  ,255,0  ,255};
	static RGBA_t blue    = {0  ,0  ,255,255};
	static RGBA_t orange  = {255,127,0  ,255};
	static RGBA_t yellow  = {255,255,0  ,255};
	static RGBA_t purple  = {127,0  ,255,255};
	static RGBA_t magenta = {255,0  ,255,255};
	static RGBA_t teal    = {0  ,255,255,255};
	static RGBA_t brown   = {127,63 ,0  ,255};
}

class Needed {
public:
	// yeah i stole these two functions

	uint32_t getPixel(SDL_Surface* surface, uint32_t x, uint32_t y) {
		//Convert the pixels to 32 bit
		uint32_t* pixels = (uint32_t*)surface->pixels;
		//Get the requested pixel
		return pixels[(y*surface->w) + x];
	}
	void putPixel(SDL_Surface* surface, uint32_t x, uint32_t y, uint32_t pixel) {
		//Convert the pixels to 32 bit
		uint32_t* pixels = (uint32_t*)surface->pixels;
		//Set the pixel
		pixels[(y*surface->w) + x] = pixel;
	}

	void putPixelBlock(SDL_Surface* surface, RGBA_t array[ball::SCREENHEIGHT][ball::SCREENWIDTH]) {
		// convert the pixels to 32 bit
		uint32_t* pixels = (uint32_t*)surface->pixels;
		// set all pixels
		for (uint32_t y = 0; y < ball::SCREENHEIGHT; ++y) {
			for (uint32_t x = 0; x < ball::SCREENWIDTH; ++x) {
				pixels[(y*surface->w) + x] = (uint32_t)array[y][x];
			}
		}
	}

	// no workie
	void setColor(SDL_Surface* surface, const RGBA_t& color) {
		if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
		SDL_memset(surface->pixels, (uint32_t)color, surface->h*surface->pitch);
		if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
	}
};