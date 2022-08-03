#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <iostream>
#include "SOM.h"

using namespace std;

Color** lattice;
Color* dataset;
int* map_width; 
int* map_height; 
int* max_iter;

int go = 0;
bool is_som_finished = false;
int iter = 0;
int level = 0;
const int max_level = 3;
const int num_color_type = 5;

double learning_rate = 0.1;
double radius = 100;
double n_learning_rate = 0.1;
double neighbor = 100;

Color** createMap(int width, int height);
Color** createNewMap(int level, int* width, int* height, Color** lattice);
Color* createInputDataset(int size);
int* create(int max_level);
int* createIter(int max_level);
void destroyMap(Color** lattice, int width);
void destroyInputDataset(Color* dataset);
bool isInNeighborhood(glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius);
void updateNode(Color** lattice, glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius, double learning_rate);
const Color& getInput(Color* dataset, int size);
double compute(int iter, double fun);
double computeSacle(double sigma, double dist);

void SOM_Create() {
    srand( time(NULL) );
    // 1. Create width, height, iter
    map_width = create(max_level);
    map_height = create(max_level);
    max_iter = createIter(max_level);
   
    // 2. Create lattice
    level = 0;
    lattice = createMap(map_width[level], map_height[level]);
    // 3. Create input dataset
    dataset = createInputDataset(num_color_type);
   
}

void SOM_IterateOnce() {
    // 1. Get one input from the dataset
    // 2. Find BMU
    // 3. Update BMU and the neighbors
    n_learning_rate = compute(iter, learning_rate);
    neighbor = compute(iter, radius);
    const Color& nowInput = getInput(dataset, num_color_type);
    double minDist = -1.0;
    double maxdist = 0.0;
    glm::ivec2 bmu;
    //compute winner point
    for(int i = 0; i < map_width[level]; i++){
        for(int j = 0; j < map_height[level]; j++){
            double tmp = 0.0;
            tmp = pow(lattice[i][j].r - nowInput.r,2) + pow(lattice[i][j].g - nowInput.g,2) + pow(lattice[i][j].b - nowInput.b,2);
            
            if(tmp > maxdist){
                maxdist = tmp;
            }

            if(minDist < 0.0){
                minDist = tmp;
            }else{
                if(minDist > tmp){
                    minDist = tmp;
                    bmu.x = i;
                    bmu.y = j;
                }
            }
        }
    }
    // renew winner point and neighnorhood

    for(int i = 0; i < map_width[level]; i++){
        for(int j = 0; j < map_height[level]; j++){
            glm::ivec2 node = glm::ivec2(i,j);
            glm::ivec2 diff = node - bmu;
            double squaredDist = static_cast<double>(diff.x * diff.x + diff.y * diff.y);

            if( isInNeighborhood(bmu, node, neighbor)){
                double n_radius = computeSacle(neighbor,squaredDist);
                updateNode(lattice, bmu, node, n_radius, n_learning_rate);
            }
        }
    }
    iter++;
    if(iter > max_iter[level]){
        // std::cout << iter << std::endl;
        // 1.new map (interpolation)
        lattice = createNewMap(level, map_width, map_height, lattice);
        // 2.mapwidth mapheight
        level++;
    }
    if(maxdist < 0.001){
        std::cout<< maxdist << std::endl;
    }
    // is_som_finished = (maxdist < 0.00001);
    is_som_finished = (iter > max_iter[max_level]);
}

void SOM_Destroy() {
    // 1. Destroy lattice
    destroyMap(lattice,map_width[level]);
    // 2. Destroy input dataset
    destroyInputDataset(dataset);
}
int* create(int max_level){
    int* map = (int*)malloc(sizeof(int) * max_level);
    map[0] = 60;
    map[1] = 180;
    map[2] = 540;
    return map;
}
int* createIter(int max_level){
    int* iterclass = (int*)malloc(sizeof(int) * max_level);
    iterclass[0] = 200;
    iterclass[1] = 1000;
    iterclass[2] = 5000;
    return iterclass;
}
Color** createMap(int width, int height) {
    Color** lattice = (Color**)malloc(sizeof(Color*) * width);
    for(int i = 0; i < width; i++){
        lattice[i] = (Color*)malloc(sizeof(Color) * height);
    } 
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            float r = (float)(rand()/(RAND_MAX + 1.0));
            float b = (float)(rand()/(RAND_MAX + 1.0));
            float g = (float)(rand()/(RAND_MAX + 1.0));
            lattice[i][j] = {r, g, b};
        }
    }
    return lattice;
}

Color* createInputDataset(int size) {
    Color* dataset = static_cast<Color*>(malloc(size * sizeof(Color)));
    
    for (int i = 0; i < size; i++) {
        float r = static_cast<float>(rand() / (RAND_MAX + 1.0));
        float g = static_cast<float>(rand() / (RAND_MAX + 1.0));
        float b = static_cast<float>(rand() / (RAND_MAX + 1.0));
        dataset[i] = {r, g, b};
        
    }
    return dataset;
}
Color** createNewMap(int level, int* width, int* height, Color** lattice){
    
    Color** new_lattice = createMap(width[level+1],height[level+1]);
    //interpolation
    int uplevelNum = 3;
    for(int i = 0; i < width[level]; i++){
        for(int j = 0; j < height[level]; j++){
            if(j == height[level]-1 || i == width[level]-1){
                for(int in = 0; in < uplevelNum; in++){
                    for(int jn = 0; jn < uplevelNum; jn++){
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].r = lattice[i][j].r;
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].g = lattice[i][j].g;
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].b = lattice[i][j].b;
                        
                    }
                }
            }else{

                for(int in = 0; in < uplevelNum; in++){
                    for(int jn = 0; jn < uplevelNum; jn++){
                        float dx = (static_cast<float>(in))/(static_cast<float>(uplevelNum));
                        float dy = (static_cast<float>(jn))/(static_cast<float>(uplevelNum));
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].r = 
                            (1-dx)*(1-dy)*lattice[i][j].r +
                            dx*(1-dy)*lattice[i+1][j].r +
                            dx*dy*lattice[i+1][j+1].r +
                            (1-dx)*dy*lattice[i][j+1].r;
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].g = 
                            (1-dx)*(1-dy)*lattice[i][j].g +
                            dx*(1-dy)*lattice[i+1][j].g +
                            dx*dy*lattice[i+1][j+1].g +
                            (1-dx)*dy*lattice[i][j+1].g;
                        new_lattice[i*uplevelNum + in][j*uplevelNum + jn].b = 
                            (1-dx)*(1-dy)*lattice[i][j].b +
                            dx*(1-dy)*lattice[i+1][j].b +
                            dx*dy*lattice[i+1][j+1].b +
                            (1-dx)*dy*lattice[i][j+1].b;
                    }
                }
            }
        }
    }
    destroyMap(lattice,map_width[level]);
    return new_lattice;
}
void destroyMap(Color** lattice,int width){
    for(int i = 0; i < width; i++){
        free(lattice[i]);
    }
}
void destroyInputDataset(Color* dataset){
    free(dataset);
}

bool isInNeighborhood(glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius) {
    glm::ivec2 diff = nodeIdx - bmuIdx;
    if ((diff.x * diff.x + diff.y * diff.y) <= (radius * radius)) {
        return true;
    }
    return false;
}

void updateNode(Color** lattice, glm::ivec2 bmuIdx, glm::ivec2 nodeIdx, double radius, double learning_rate) {
    const Color& bmu = lattice[bmuIdx.x][bmuIdx.y];
    Color& node = lattice[nodeIdx.x][nodeIdx.y];
    node.r = node.r + radius * learning_rate * (bmu.r - node.r);
    node.g = node.g + radius * learning_rate * (bmu.g - node.g);
    node.b = node.b + radius * learning_rate * (bmu.b - node.b);
}

const Color& getInput(Color* dataset, int size) {
    int num = rand() % size;
    return dataset[num];
}
double compute(int iter, double fun){
    double lamda = ((double)(max_iter[level]))/ log(fun);
    double sigma = fun*exp(-1* ((double)iter)/ lamda);
    return sigma;
}
double computeSacle(double sigma, double dist){
    double theta = exp((-1 *dist)/(2*pow(sigma,2)));

    return theta;
}
