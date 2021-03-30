#include <smart-rockets.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Rocket creation
Rocket *newRocket(int dna_length, int initial_position[2])
{
  // Allocate rocket memory
  Rocket *rocket = malloc(sizeof(Rocket));
  
  // Set dna length
  rocket->dna_length = dna_length;
  
  // Set initial position
  rocket->x = initial_position[0];
  rocket->y = initial_position[1];
  
  // Set initial velocity
  rocket->velocity[0] = 0;
  rocket->velocity[1] = 0;
  
  // Set initial fitness score
  rocket->fitness_score = 0;
  
  // Allocate rocket's dna memory
  rocket->dna = malloc(sizeof(float*) * dna_length);
  
  // Set random Genes
  for(int idx = 0; idx<dna_length; idx++)
  {
    // Allocate Gene
    rocket->dna[idx] = malloc(sizeof(float) * 2);
    rocket->dna[idx][0] = (rand() % 10) - 5;
    rocket->dna[idx][1] = (rand() % 10) - 5;
  }

  return rocket;
}

Rocket *breed(Rocket *parent_a, Rocket *parent_b, int mutation_factor, int initial_position[2])
{
  // Allocate rocket memory
  Rocket *rocket =  malloc(sizeof(Rocket));
  
  // Set dna length
  rocket->dna_length = parent_a->dna_length;
  
  // Set initial position
  rocket->x = initial_position[0];
  rocket->y = initial_position[1];
  
  // Set initial velocity
  rocket->velocity[0] = 0;
  rocket->velocity[1] = 0;
  
  // Set initial fitness score
  rocket->fitness_score = 0;
  
  // Allocate rocket's dna memory
  rocket->dna = malloc(sizeof(float*) * parent_a->dna_length);
  
  // Set random Genes
  for(int idx = 0; idx<parent_a->dna_length; idx++)
  {
    // Allocate Gene
    rocket->dna[idx] = malloc(sizeof(float) * 2);
    
    // Sellect between parent's genes
    if(rand() % 100 < 50){
      // Parrent A
      rocket->dna[idx][0] = parent_a->dna[idx][0];
      rocket->dna[idx][1] = parent_a->dna[idx][1];
    }else{
      // Parrent B
      rocket->dna[idx][0] = parent_b->dna[idx][0];
      rocket->dna[idx][1] = parent_b->dna[idx][1];
    }
    
    // Mutate
    if(rand() % 100 < mutation_factor){
      rocket->dna[idx][0] = (rand() % 10) - 5;
      rocket->dna[idx][1] = (rand() % 10) - 5;
    }
  }

  return rocket;
}

void destroyRocket(Rocket *rocket)
{
  for(int idx = 0; idx<rocket->dna_length; idx++)
  {
    // Free Gene
    free(rocket->dna[idx]);
  }
  // Free dna
  free(rocket->dna);
  
  // Free rocket
  free(rocket);
}

// Population creation/destruction
Population *newPopulation(int size, int dna_length, int init_pos[2],  int mutation_factor)
{
  // Allocate population memory
  Population *population = malloc(sizeof(Population));
  
  // Set population size
  population->size = size;
  
  // Set mutation factor
  population->mutation_factor = mutation_factor;
  
  // Allocate population's rockets memory
  population->rockets = malloc(sizeof(Rocket) * size);
  
  // Set random Rockets
  for(int idx=0; idx<size; idx++)
  {
    // Allocate Rocket
    population->rockets[idx] = newRocket(dna_length, init_pos);
  }
  
  return population;
}

Population *nextGeneration(Population *population, int initial_position[2])
{
  // Allocate population memory
  Population *newPopulation = malloc(sizeof(Population));
  
  // Set population size
  newPopulation->size = population->size;
    
  // Set mutation factor
  newPopulation->mutation_factor = population->mutation_factor;
  
  // Allocate population's rockets memory
  newPopulation->rockets = malloc(sizeof(Rocket) * newPopulation->size);
  
  // TODO: Use multithread merge sort using rocket's fitness score
  // Breed 25 best rockets
  for(int idx=0; idx<population->size; idx++){
    Rocket *parent_a = population->rockets[rand() % 25];
    Rocket *parent_b = population->rockets[rand() % 25];
    newPopulation->rockets[idx] = breed(parent_a, parent_a, population->mutation_factor, initial_position);
  }
  
  return newPopulation;
}

void destroyPopulation(Population *population)
{
  for(int idx = 0; idx<population->size; idx++)
  {
    // Free Rocket
    destroyRocket(population->rockets[idx]);
  }
  
  // Free population
  free(population);
}

// Board creation
Board *newBoard(int width, int height, int target[2], int n_obstacles)
{
  // Allocate board memory
  Board *board = malloc(sizeof(Board));
  
  // Set width and height
  board->width = width;
  board->height = height;
  
  // Set target
  board->target[0] = target[0];
  board->target[1] = target[1];
  
  // Set number of obstacles
  board->n_obstacles = n_obstacles;
  
  // Allocate board's obstacles memory
  board->obstacles = malloc(sizeof(Board) * n_obstacles);
  
  for(int idx = 0; idx<n_obstacles; idx++)
  {
    // Free Rocket
    int x0 = rand() % width;
    int y0 = rand() % height;
    int x1 = rand() % width;
    int y1 = rand() % height;
    board->obstacles[idx] = newObstacle(x0, y0, x1, y1);
  }
  
  return board;
}

void destroyBoard(Board *board){
  // Free obstacles
  for(int idx = 0; idx<board->n_obstacles; idx++)
  {
    // Free Rocket
    destroyObstacle(board->obstacles[idx]);
  }
}

// Obstacle creation/destruction
Obstacle *newObstacle(int x0, int y0, int x1, int y1)
{
  // Alocate obstacle memory
  Obstacle *obstacle = malloc(sizeof(Obstacle));
  
  // Set obstacle position
  obstacle->x0 = x0;
  obstacle->y0 = y0;
  obstacle->x1 = x1;
  obstacle->x1 = x1;
  
  return obstacle;
}

void destroyObstacle(Obstacle *obstacle)
{
  // Free obstacle
  free(obstacle);
}

// Fitness function
float fitness(Board *board, Rocket *rocket)
{
  // Calculate rocket's distance from board's target
  float distance = sqrt(
    (rocket->x - board->target[0]) * (rocket->x - board->target[0]) +
    (rocket->y - board->target[1]) * (rocket->y - board->target[1])
  );
  
  // Return Maximum float if rocket is right on target else 1/distance
  return ((rocket->x - board->target[0]) == 0 && (rocket->y - board->target[1]) == 0) ?
    FLT_MAX :
    1/distance;
}

// Rocket update
void updateRocket(Board *board, Rocket *rocket, int frame_idx)
{
  // TODO: Check for max velocity, off-screen movement and obstacle collision(maybe multithread?)
  // Update rocket's velocity
  rocket->velocity[0] += rocket->dna[frame_idx][0];
  rocket->velocity[1] += rocket->dna[frame_idx][1];
  
  // Update rocket's position
  rocket->x += rocket->velocity[0];
  rocket->y += rocket->velocity[1];
  
  // Update rocket's fitness score
  rocket->fitness_score = fitness(board, rocket);
}
