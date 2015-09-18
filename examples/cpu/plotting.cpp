/*******************************************************
 * Copyright (c) 2015-2019, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <forge.h>
#include <CPUCopy.hpp>
#include <complex>
#include <cmath>
#include <vector>
#include <iostream>

const unsigned DIMX = 512;
const unsigned DIMY = 512;

const float FRANGE_START = 0.f;
const float FRANGE_END = 2.f * 3.1415926f;

using namespace std;
void map_range_to_vec_vbo(float range_start, float range_end, float dx, std::vector<float> &vec, float (*map) (float)){
    if(range_start > range_end && dx > 0) return;
    for(float i=range_start; i < range_end; i+=dx){
        vec.push_back(i);
        vec.push_back((*map)(i));
    }
}

int main(void){
    std::vector<float> function;
    map_range_to_vec_vbo(FRANGE_START, FRANGE_END, 0.1f, function, &sinf);

    /*
     * First Forge call should be a window creation call
     * so that necessary OpenGL context is created for any
     * other fg::* object to be created successfully
     */
    fg::Window wnd(DIMX, DIMY, "Fractal Demo");
    wnd.makeCurrent();
    /* create an font object and load necessary font
     * and later pass it on to window object so that
     * it can be used for rendering text */
    fg::Font fnt;
#ifdef OS_WIN
    fnt.loadSystemFont("Calibri", 32);
#else
    fnt.loadSystemFont("Vera", 32);
#endif
    wnd.setFont(&fnt);

    /* Create a plot object which creates the necessary
     * vertex buffer objects to hold the plot
     */
    fg::Plot plt(function.size()/2, fg::FG_FLOAT);                                //create a default plot
    //fg::Plot plt(function.size()/2, fg::FG_FLOAT, fg::FG_SCATTER, fg::POINT);   //or specify a specific plot type

    //plt.setColor(fg::GREEN);                                                    //use a forge predefined color
    plt.setColor((fg::Color) 0xABFF01FF);                                         //or any hex-valued color

    plt.setAxesLimits(FRANGE_END, FRANGE_START, 1.1f, -1.1f);
    copy(plt, &function[0]);

    /* copy your data into the pixel buffer object exposed by
     * fg::Plot class and then proceed to rendering.
     * To help the users with copying the data from compute
     * memory to display memory, Forge provides copy headers
     * along with the library to help with this task
     */

    do {
        wnd.draw(plt);
    } while(!wnd.close());

    return 0;
}

