#include <iostream>
#include "tgaimage.h"
#include "geometry.h"

void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color) // removes all the floating point values and division
{
	bool steep = false;
	if (std::abs(v0.x - v1.x) < std::abs(v1.y - v0.y))
	{
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
		steep = true;
	}

	if (v0.x > v1.x)
	{
		std::swap(v0, v1);
	}

	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;
	float slope = std::abs(dy*2);
	float slope_error = 0;
	int y = v0.y;
	for (int x = v0.x; x <= v1.x; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		slope_error += slope;
		
		if (slope_error > dx) // only update y if the slope_error becomes greater than the difference between the x coordinates
		{
			y += (v1.y>v0.y?1:-1); // if go up or down on y depending on if y1 is greater than it or not
			slope_error -= dx*2;
		}
	}	
}

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, TGAColor color)
{
    line(v0, v1, image, color);
    line(v1, v2, image, color);
    line(v2, v0, image, color);
}

void triangle(Vec2i t[], TGAImage &image, TGAColor color)
{
    line(t[0], t[1], image, color);
    line(t[1], t[2], image, color);
    line(t[2], t[0], image, color);
}

float lerp(float num0, float num1, float t)
{
	return num0 + (num1 - num0) * t;
}

void fill_triangle(Vec2i t[], TGAImage &image, TGAColor color)
{
    // main question: how do I get the points between the lines

    // sort triangle based on y
    for (int i = 0; i < 3; i++)
    {
        for (int j = i+1; j < 3; j++)
        {
            if (t[i].y < t[j].y)
            {
                std::swap(t[i], t[j]);
            }
        }
    }

    // get the side with the larger x value to keep the left side to t1
    if (t[1].x > t[2].x)
    {
        std::swap(t[1], t[2]);
    }

    // get the appropriate coordinates and fill it in with a line

    for (float i = 0.; i < 1.; i+=.01)
    {
        int x0 = lerp(t[0].x, t[1].x, i);
        int y0 = lerp(t[0].y, t[1].y, i);
        int x1 = lerp(t[0].x, t[2].x, i);
        int y1 = lerp(t[0].y, t[2].y, i);

        line(Vec2i(x0, y0), Vec2i(x1, y1), image, color);
    }
}


int main(int argc, char** argv)
{
    TGAColor white(255,255,255,255);
    TGAColor red(255,0,0,255);
    TGAColor green(0,255,0,255);
    TGAColor cyan(0,255,255,255);
    TGAImage image(200, 200, TGAImage::RGB);
    image.set(1,1,TGAColor(255,255,255,255));

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    fill_triangle(t0, image, cyan); 
    fill_triangle(t1, image, cyan);
    fill_triangle(t2, image, cyan); 

    triangle(t0, image, white);
    triangle(t1, image, red);
    triangle(t2, image, green);

    image.flip_vertically();
    image.write_tga_file("output2.tga");
    return 0;
}