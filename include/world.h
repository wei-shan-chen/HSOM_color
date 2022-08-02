#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <vector>

#include "Vertex.h"

struct World {
    std::vector<Vertex> square;
    std::vector<unsigned int> squ_indices;
    std::vector<Vertex> tri;
    //std::array<Attribute, 4> square; 
    //std::array<unsigned int, 6> squ_indices; 
    // std::array<float, 108> cube; 
    // std::array<float, 9> tri; 
    //std::array<unsigned int, 36> cube_ebo; 
};

void create_world();
void destroy_world();

extern struct World world;

#endif