#include <vector>
#include <cmath>
#include <limits>
#include "tgaimage.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);
const int width  = 800;
const int height = 500;

void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t + .5;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

void rasterize(Vec2i v0, Vec2i v1, int ybuf[], TGAImage &image, TGAColor color) {
    // make sure the larger x is to the right (just my convention that i like)
    if (v0.x > v1.x)
        std::swap(v0, v1);
    for (int x = v0.x; x < v1.x; x++)
    {
        float t = (x - v0.x)/float(v1.x - v0.x);
        int y = v0.y + (v1.y - v0.y) * t;

        // now that we have x and y, we just need to draw the line but take into account the y buffer
        if (ybuf[x] < y)
        {
            ybuf[x] = y;
            image.set(x, 0, color);
        }
        
    }
}

int main(int argc, char** argv) {
    { // just dumping the 2d scene (yay we have enough dimensions!)
        TGAImage scene(width, height, TGAImage::RGB);

        // scene "2d mesh"
        line(Vec2i(20, 34),   Vec2i(744, 400), scene, red);
        line(Vec2i(120, 434), Vec2i(444, 400), scene, green);
        line(Vec2i(330, 463), Vec2i(594, 200), scene, blue);

        // screen line
        line(Vec2i(10, 10), Vec2i(790, 10), scene, white);

        scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        scene.write_tga_file("scene.tga");
    }

    {
        TGAImage render(width, 16, TGAImage::RGB);
        int ybuf[width] = {-9999};
        rasterize(Vec2i(20, 34),   Vec2i(744, 400), ybuf, render, red);
        rasterize(Vec2i(120, 434), Vec2i(444, 400), ybuf, render, green);
        rasterize(Vec2i(330, 463), Vec2i(594, 200), ybuf, render, blue);

        // 1-pixel wide image is bad for eyes, lets widen it
        for (int i=0; i<width; i++) {
            for (int j=1; j<16; j++) {
                render.set(i, j, render.get(i, 0));
            }
        }
        render.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        render.write_tga_file("render.tga");
    }
    return 0;
}

