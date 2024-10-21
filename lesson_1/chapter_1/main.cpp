#include "tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

float lerp(float num0, float num1, float t);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineFast(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void lineFastest(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color); 

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	image.set(99, 41, red);


	lineFastest(1, 1, 99, 99, image, red);
	lineFastest(20, 1, 50, 50, image, white);

	lineFastest(10, 20, 15, 40, image, red); 
	lineFastest(15, 40, 10, 20, image, white);  // flipped line
	lineFastest(15, 1, 20, 99, image, red); /// steep line
	lineFastest(1, 14, 99, 19, image, white); /// steep line

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	TGAImage other_image(100, 100, TGAImage::RGB);
	other_image.set(99, 41, red);
	lineFast(1, 1, 99, 99, other_image, red);
	lineFast(20, 1, 50, 50, other_image, white);
	lineFast(10, 20, 15, 40, other_image, red); 
	lineFast(15, 40, 10, 20, other_image, white);  // flipped line
	lineFast(15, 1, 20, 99, other_image, red); /// steep line
	lineFast(1, 14, 99, 19, image, white); /// steep line
	other_image.flip_vertically();
	other_image.write_tga_file("output_2.tga");

	return 0;
}


float lerp(float num0, float num1, float t)
{
	return num0 + (num1 - num0) * t;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	for (float i = 0.; i < 1.; i+=0.01)
	{
		image.set(lerp(x0, x1, i), lerp(y0, y1, i), color);
	}
}

void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	bool steep = false;
	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++)
	{
		float t = (float)(x - x0) / (float)(x1 - x0);
		int y = lerp(y0, y1, t);
		
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);
	}
}

void lineFast(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) // this doesn't use lerp anymore
{
	bool steep = false;
	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}


	int dx = x1 - x0;
	int dy = y1 - y0;
	float slope = std::abs(dy/float(dx));
	float slope_error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		slope_error += slope;
		
		if (slope_error > .5) // only update y if the slope_error becomes greater than the difference between the x coordinates
		{
			y += (y1>y0?1:-1); // if go up or down on y depending on if y1 is greater than it or not
			slope_error -= 1;
		}
	}	
}

void lineFastest(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) // removes all the floating point values and division
{
	bool steep = false;
	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}


	int dx = x1 - x0;
	int dy = y1 - y0;
	float slope = std::abs(dy*2);
	float slope_error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		slope_error += slope;
		
		if (slope_error > dx) // only update y if the slope_error becomes greater than the difference between the x coordinates
		{
			y += (y1>y0?1:-1); // if go up or down on y depending on if y1 is greater than it or not
			slope_error -= dx*2;
		}
	}	
}

