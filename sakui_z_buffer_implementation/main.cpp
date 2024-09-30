#include <iostream>
#include <algorithm>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"


const int HEIGHT = 800;
const int WIDTH = 800;

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

void fill_triangle(Vec2i t[], TGAImage &image, TGAColor color) // my version
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

    for (float i = 0.; i < 0.1; i+=.01)
    {
        int x0 = lerp(t[0].x, t[1].x, i);
        int y0 = lerp(t[0].y, t[1].y, i);
        int x1 = lerp(t[0].x, t[2].x, i);
        int y1 = lerp(t[0].y, t[2].y, i);

        std::cout << "Line drawn at " << x0 << " " << y0 << " " << x1 << " " << y1 << "\n";
        line(Vec2i(x0, y0), Vec2i(x1, y1), image, TGAColor(255, x0*3, y0*5, 255));
        getchar();
    }
}

void fill_triangle_proper(Vec2i t[], TGAImage &image, TGAColor color)
{
    // Step 1: Sort in ascending order
    if (t[0].y < t[1].y) std::swap(t[0], t[1]);
    if (t[0].y < t[2].y) std::swap(t[0], t[2]);
    if (t[1].y < t[2].y) std::swap(t[1], t[2]);

    // Step 2: taking into account the fact that its split into 2 different parts between lines t[0] t[1] and t[2] t[1], draw the first half
    int total_height = t[0].y - t[2].y; 
    for (int y = t[1].y; y <= t[0].y; y++)
    {
        int segment_height = t[0].y - t[1].y; // take into account the height of the segment we're currently drawing
        
        // Get the value for t for both the segments (lerping)
        float total_lerp_value = (float)(y - t[2].y) / total_height; // subtract with the lowest point to get
        float segment_lerp_value = (float)(y - t[1].y) / segment_height;
        Vec2i point_A = t[2] + (t[0] - t[2]) * total_lerp_value;
        Vec2i point_B = t[1] + (t[0] - t[1]) * segment_lerp_value;

        // Swap if the x value of the reference point (starting point) is not the left-most pixel
        if (point_A.x > point_B.x) std::swap(point_A, point_B);

        // Draw each point on the same y axis, we don't use line here (i don't know why) 
        for (int j=point_A.x; j <= point_B.x; j++) { 
            image.set(j, y, color); 
        } 
    }

    // second half
    for (int y = t[2].y; y <= t[1].y; y++)
    {
        int segment_height = t[1].y - t[2].y; // take into account the height of the segment we're currently drawing
        
        // Get the value for t for both the segments (lerping)
        float total_lerp_value = (float)(y - t[2].y) / total_height; // subtract with the lowest point to get
        float segment_lerp_value = (float)(y - t[2].y) / segment_height;
        Vec2i point_A = t[2] + (t[0] - t[2]) * total_lerp_value;
        Vec2i point_B = t[2] + (t[1] - t[2]) * segment_lerp_value;

        // Swap if the x value of the reference point (starting point) is not the left-most pixel
        if (point_A.x > point_B.x) std::swap(point_A, point_B);

        // Draw each point on the same y axis, we don't use line here (i don't know why) 
        for (int j=point_A.x; j <= point_B.x; j++) { 
            image.set(j, y, color); 
        } 
    }
}

// btw, having it just be t[] is verbad hahaha
void fill_triangle_short(Vec2i t[], TGAImage &image, TGAColor color)
{
    // Sort
    if (t[0].y < t[1].y) std::swap(t[0], t[1]);
    if (t[0].y < t[2].y) std::swap(t[0], t[2]);
    if (t[1].y < t[2].y) std::swap(t[1], t[2]);

    // Fill in both halves
    int total_height = t[0].y - t[2].y; 
    for (int y = 0; y < total_height; y++)
    {
        bool isSecondHalf = y > t[1].y - t[2].y || t[1].y == t[2].y;
        int segment_height = isSecondHalf? t[0].y - t[1].y : t[1].y - t[2].y;

        float total_lerp_value = (float)y / total_height; 
        float segment_lerp_value = (float)(y - (isSecondHalf? t[1].y-t[2].y : 0)) / segment_height;

        Vec2i point_A = t[2] + (t[0] - t[2]) * total_lerp_value;
        Vec2i point_B = (isSecondHalf? t[1] + (t[0] - t[1]) * segment_lerp_value : t[2] + (t[1] - t[2]) * segment_lerp_value) ;

        if (point_A.x > point_B.x) std::swap(point_A, point_B);

        // Draw each point on the same y axis, we don't use line here (i don't know why) 
        for (int j=point_A.x; j <= point_B.x; j++) { 
            image.set(j, t[2].y + y, color); 
        } 
    }
}

Vec3f get_barycentric_coords(Vec2i t0, Vec2i t1, Vec2i t2, Vec2i p)
{
    Vec3f coords = Vec3f(t2.x - t0.x, t1.x - t0.x, t0.x - p.x)^Vec3f(t2.y - t0.y, t1.y - t0.y, t0.y - p.y);

    float w = 1.f - (coords.x + coords.y) / coords.z;
    float u = coords.x / coords.z;
    float v = coords.y / coords.z;
    return Vec3f(w, u, v);
}

void fill_triangle_barycentric(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{   
    if (t0.y==t1.y && t0.y==t2.y) return;
    // First, we want to get the bounding boxes of the triangle
    Vec2i bboxmax(0, 0);
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);
    Vec2i bboxmin = clamp;

    Vec2i points[3] = {t0, t1, t2}; 

    // Shrink the bounding boxes to find the smallest box that will fit the triangle
    for(int i = 0; i < 3; i++)
    {
        // from the current max (0),
        // make the smallest possible point in the triangle the end of the image (if it is outside the image, use clamp)
        // take the largest from the current value to the point
        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, points[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, points[i].y));

        bboxmin.x = std::max(0, std::min(bboxmin.x, points[i].x));
        bboxmin.y = std::max(0, std::min(bboxmin.y, points[i].y));
    }

    for (int y = bboxmin.y; y < bboxmax.y; y++)
    {
        for (int x = bboxmin.x; x < bboxmax.x; x++)
        {
            Vec3f barycrd = get_barycentric_coords(t0, t1, t2, Vec2i(x, y));
            if (barycrd.x < 0 || barycrd.y < 0 || barycrd.z < 0)
                continue;

            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv)
{
    TGAColor white(255,255,255,255);
    TGAColor red(255,0,0,255);
    TGAColor green(0,255,0,255);
    TGAColor cyan(0,255,255,255);

    Vec3f light_direction(0, 0, -1);

    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    Model* model = NULL;

    if (argc == 2)
    {
        model = new Model(argv[1]);
    }
    else
    {
        model = new Model("obj/african_head.obj");
    }


    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            screen_coords[j].x = (v.x+1.)*WIDTH/2.;
            screen_coords[j].y = (v.y+1.)*HEIGHT/2.;
            world_coords[j] = v;
        }

        Vec3f normal = (world_coords[2] - world_coords[0])^(world_coords[1] - world_coords[0]);
        normal.normalize();
        float intensity = normal*light_direction;
        
        TGAColor color = TGAColor(255*intensity, 255*intensity,255*intensity,255);

        if (intensity > 0)
            fill_triangle_barycentric(screen_coords[0], screen_coords[1], screen_coords[2], image, color);
            // fill_triangle_barycentric_z(screen_coords[0], screen_coords[1], screen_coords[2], image, color, zbuffer, world_coords);
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    
    return 0;
}