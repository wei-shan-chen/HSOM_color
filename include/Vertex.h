#ifndef VERTEX_H
#define VERTEX_H

struct Vertex{
    struct Position{
        float x, y, z;
    };

    // struct Normal {
    //     float xn, yn, zn;
    // };
    
    Position position;
    // Normal normal;
    //Texcoord texcoord;
};

#endif