#include "world.h"


struct World world = {};

void create_world() {
    // world.square = {
    //         Vertex{ { 0.5f, -0.5f, 0.0f} ,{ 0.0f,  0.0f,  1.0f} ,{1.0f,0.0f}},
    //         Vertex{ { 0.5f,  0.5f, 0.0f} ,{ 0.0f,  0.0f,  1.0f} ,{1.0f,1.0f}},
    //         Vertex{ {-0.5f,  0.5f, 0.0f} ,{ 0.0f,  0.0f,  1.0f} ,{0.0f,1.0f}},
    //         Vertex{ {-0.5f, -0.5f, 0.0f} ,{ 0.0f,  0.0f,  1.0f} ,{0.0f,0.0f}},
    // };
    world.square = {
            Vertex{ { 0.0f, 0.0f, 0.0f}},
            Vertex{ { 1.0f, 0.0f, 0.0f}},
            Vertex{ { 1.0f, 1.0f, 0.0f}},
            Vertex{ { 0.0f, 1.0f, 0.0f}},
    };
    world.squ_indices = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };
    world.tri= {
		Vertex{ { 0.5f, -0.5f, 0.0f}},
        Vertex{ {-0.5f, -0.5f, 0.0f}},
        Vertex{ { 0.0f,  0.5f, 0.0f}}
	};

}

void destroy_world() {

}