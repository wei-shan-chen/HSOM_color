#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "color.h"

Color** interpolation(int level, int* width, int* height, Color** lattice);

#endif

/*
if(j == height[level]-1 || i == width[level]-1){
                for(int in = 0; in < uplevelNum; in++){
                    for(int jn = 0; jn < uplevelNum; jn++){
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].r = lattice[i][j].r;
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].g = lattice[i][j].g;
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].b = lattice[i][j].b;
                    }
                }
            }
        */