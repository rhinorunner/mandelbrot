#pragma once

#include <iostream>
#include <thread>
#include <complex>
#include <immintrin.h>

#include "specifications.hpp"

typedef std::complex<double> Complex;

uint16_t findMandelbrot(double cr, double ci, uint32_t max_iterations) {
	uint32_t i = 0;
	double zr = 0.0, zi = 0.0;
	while ((i < max_iterations) && ((zr*zr + zi*zi) < 4.0)) {
		double temp = zr*zr - zi*zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		++i;
	}
	return i;
}

double mapToReal(uint32_t x, uint32_t width, double minR, double maxR) {
	return x * ((maxR-minR) / width) + minR;
}

double mapToImaginary(uint32_t y, uint32_t height, double minI, double maxI) {
	return y * ((maxI-minI) / height) + minI;
}

RGBA_t HSVtoRGB(float H, float S, float V){
	if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
		return RGBA_t(0,0,0,255);
	}
	double s = S/100;
	double v = V/100;
	double C = s*v;
	double X = C*(1-abs(fmod(H/60.0, 2)-1));
	double m = v-C;
	double r,g,b;
	if(H >= 0 && H < 60){
		r = C,g = X,b = 0;
	}
	else if(H >= 60 && H < 120){
		r = X,g = C,b = 0;
	}
	else if(H >= 120 && H < 180){
		r = 0,g = C,b = X;
	}
	else if(H >= 180 && H < 240){
		r = 0,g = X,b = C;
	}
	else if(H >= 240 && H < 300){
		r = X,g = 0,b = C;
	}
	else{
		r = C,g = 0,b = X;
	}
	int R = (r+m)*255;
	int G = (g+m)*255;
	int B = (b+m)*255;

	return RGBA_t(R,G,B,255);
}