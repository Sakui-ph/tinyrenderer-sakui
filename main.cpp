#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

float lerp(float num0, float num1, float t);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	image.set(99, 41, red);

	// line(1, 1, 99, 99, image, red);
	// line2(20, 1, 50, 50, image, white);

	line2(10, 20, 15, 40, image, red); 
	line2(15, 40, 10, 20, image, white);  // flipped line
	line2(15, 1, 20, 99, image, red); /// steep line

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
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


