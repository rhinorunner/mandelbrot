/*
Zaine Rehman
1-3-2024 through 2-22-2024

The previous program worked but had very messy code
I also want to implement SIMD and possibly multithreading
And just make it better in general

HAHA NEVERMIND IT IS NOW MANDELBROT
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include <SDL.h>
#undef main // needed or else it breaks

#ifdef __SSE2__
	#include <emmintrin.h>
#else
	#error SSE2 not supported; speed will be limited
#endif

#ifdef __AVX__
	#include <immintrin.h>
#else
	#warning AVX not supported; speed will be limited
#endif

#include "specifications.hpp"
#include "extras.hpp"

using namespace ball;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

void doMandel(
	double startX, double startY, 
	double endX, double endY, 
	bool doEasy
) {
	for (uint32_t i = startX; i < endX; i += 1) {
		for (uint32_t j = startY; j < endY; j += 1) {
			if (doEasy && (lastMandel[i][j] < fastRenderVal)) continue;

			double cr = mapToReal(j,SCREENWIDTH,-currentZoomFactor,currentZoomFactor);
			double ci = mapToImaginary(i,SCREENHEIGHT,-currentZoomFactor,currentZoomFactor);

			uint32_t n = findMandelbrot(screenCenter[0]+cr,screenCenter[1]+ci,maxIter);

			RGBA_t color {0,0,0,255};

			switch (colorMethod) {
			case 0:
				if (n < maxIter)
					color = HSVtoRGB(n/(maxIter/1.0) * 360, 100, 100);
				break;
			case 1:
				color.R = (uint8_t)((n/maxIter)*255);
				color.G = (uint8_t)((n/maxIter)*255);
				color.B = (uint8_t)((n/maxIter)*255);
				break;
			case 2:
				color.R = (uint8_t)((n/maxIter)*255);
				color.G = (uint8_t)((n/maxIter)*255);
				color.B = (uint8_t)((n/maxIter)*255);
				break;
			}
			

			//uint8_t r = (uint8_t)((n/maxIter) * 255);
			//uint8_t g = (uint8_t)((n/maxIter) * 255);
			//uint8_t b = (uint8_t)((n/maxIter) * 255);

			PIXELS[i][j] = color;
			lastMandel[i][j] = n;
		}
		//if(i == SCREENHEIGHT / 2) break;
	}
}

int main() {
	srand(time(0));

	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		// honestly no idea why you would even get here
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	SDL_CreateWindowAndRenderer(SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_RESIZABLE, &WINDOW, &RENDERER);
	if (WINDOW == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}
	SURFACE = SDL_GetWindowSurface(WINDOW);
	
	SDL_SetWindowTitle(WINDOW,"Mandelbrot");
	//SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
	//SDL_RenderClear(RENDERER);

	//SDL_SetSurfaceRLE(SURFACE,1);

	SDL_Event event;
	Needed pixelRenderer {};
	clock_t timeNew = clock();
	clock_t timeOld = clock();
	double deltaTime = 0;
	uint32_t mouseX = 0, mouseY = 0;
	bool QUIT = false;
	bool newCalc = true;
	bool doEasy = false;

	/*
	SDL_SetRenderDrawColor(RENDERER,R,B,G,A);
	SDL_RenderDrawPoint(RENDERER,X,Y); // maybe i dont have to use this
	SDL_RenderPresent(R_RENDERER); // or this
	SDL_RenderClear(R_RENDERER);
	*/
	
	while (!QUIT) {
		//SDL_RenderClear(RENDERER);
		timeNew = clock();
		deltaTime = timeNew - timeOld;

		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			QUIT = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				QUIT = true;
				break;

			case SDLK_KP_1: // rainbow colored
				colorMethod = 0;
				newCalc = true;
				doEasy = true;
				break;
			case SDLK_KP_2: // grayscale
				colorMethod = 1;
				newCalc = true;
				doEasy = true;
				break;

			case SDLK_LEFT:
				screenCenter[0] -= .1 * currentZoomFactor;
				newCalc = true;
				break;
			case SDLK_RIGHT:
				screenCenter[0] += .1 * currentZoomFactor;
				newCalc = true;
				break;
			case SDLK_UP:
				screenCenter[1] -= .1 * currentZoomFactor;
				newCalc = true;
				break;
			case SDLK_DOWN:
				screenCenter[1] += .1 * currentZoomFactor;
				newCalc = true;
				break;

			case SDLK_KP_PERIOD:
				newCalc = true;
				doEasy = true;
				break;
			case SDLK_KP_0:
				newCalc = true;
				break;

			case SDLK_KP_MULTIPLY:
				maxIter *= 1.5;
				newCalc = true;
				break;
			case SDLK_KP_DIVIDE:
				maxIter /= 1.5;
				newCalc = true;
				break;
			}

		//default: break;

		case SDL_MOUSEBUTTONDOWN: break;
			if (event.button.button == SDL_BUTTON_LEFT) {
				screenCenter[0] = event.motion.x;
				screenCenter[1] = event.motion.y;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0) {
				newCalc = true;
				currentZoomFactor *= .8;
				maxIter += 1;
			} else {
				newCalc = true;
				currentZoomFactor /= .8;
				maxIter -= 1;
			}
			break;
			std::cout 
				<< '[' << mouseX << ',' << mouseY << ']'
				<< ": value=" << lastMandel[mouseY][mouseX]
				<<'\n';
			break;
		}
		
		if (SDL_MUSTLOCK(SURFACE)) SDL_LockSurface(SURFACE);
		//pixelRenderer.putPixelBlock(SURFACE, PIXELS);
		//pixelRenderer.putPixel(SURFACE, 100, 100, Colors::red);

		if (newCalc) {
			std::cout
				<< "\nCALCULATING WITH PARAMETERS: "
				<< "maxN = " << maxIter
				//<< ", colorMethod = " << colorMethod
				//<< ", fastRenderVal = " << fastRenderVal
				<< ", center = [" << screenCenter[0] << "," << screenCenter[1] << "]"
				<< ", zoom = " << currentZoomFactor
				<< "\n\n";
			
			std::thread threads[THREAD_AMOUNT] {};

			for (uint32_t i = 0; i < THREAD_AMOUNT; ++i) {
				threads[i] = std::thread(
					doMandel, 
					0, i*(SCREENHEIGHT/THREAD_AMOUNT), 
					SCREENWIDTH, (i+1)*(SCREENHEIGHT/THREAD_AMOUNT),
					false
				);
			}

			for (uint16_t i = 0; i < THREAD_AMOUNT; ++i) {
				threads[i].join();
			}

			//doMandel(0, 0, SCREENWIDTH, SCREENHEIGHT, doEasy);
			
			newCalc = false;
			doEasy = false;
		}

		pixelRenderer.putPixelBlock(SURFACE, PIXELS);
		
		if (SDL_MUSTLOCK(SURFACE)) SDL_UnlockSurface(SURFACE);
		
		//SDL_RenderPresent(RENDERER);
		SDL_UpdateWindowSurface(WINDOW);
		timeOld = timeNew;
	}
	
	SDL_DestroyRenderer(RENDERER);
	SDL_DestroyWindow(WINDOW);
	SDL_Quit();
	
	return 0;
}