#ifndef SOM_H
#define SOM_H

#include "color.h"

void SOM_Create();
void SOM_IterateOnce();
void SOM_Destroy();

extern Color** lattice;
extern Color* dataset;
extern bool is_som_finished;
extern int* map_width;
extern int* map_height;
extern int level;
extern int iter;
extern int go;
extern double n_learning_rate;
extern double neighbor;
extern const int num_color_type;
#endif