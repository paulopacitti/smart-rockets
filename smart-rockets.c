#include <smart-rockets.h>
#include <stdlib.h>

int libFunc()
{
  return 1;
}

// Rocket creation
Rocket *newRocket(int dna_length, int init_pos[2])
{
  // Allocate rocket memory
  Rocket *rocket = malloc(sizeof(Rocket));
  
  // Set dna length
  rocket->dna_length = dna_length;
  
  // Set initial position
  rocket->x = init_pos[0];
  rocket->y = init_pos[1];
  
  // Set initial velocity
  rocket->vel[0] = 0;
  rocket->vel[1] = 0;
  
  // Set initial fitness score
  rocket->fitness_score = 0;
  
  // Allocate rocket's dna memory
  rocket->dna = malloc(sizeof(float*) * dna_length);
  
  // Set random Genes
  for(int idx = 0; idx<dna_length; idx++)
  {
    // Allocate Gene
    rocket->dna[idx] = malloc(sizeof(float) * 2);
    rocket->dna[idx][0] = rand() % 10;
    rocket->dna[idx][1] = rand() % 10;
  }

  return rocket;
}

Rocket *breed(Rocket *parent_a, Rocket *parent_b)
{
  
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

Population *newPopulation(int size, int dna_length, int init_pos[2])
{
  // Allocate population memory
  Population *population = malloc(sizeof(Population));
  
  // Set population size
  population->size = size;
  
  // Allocate population's rockets memory
  population->rockets = malloc(sizeof(Rocket) * size);
  
  // Set random Rockets
  for(int idx = 0; idx<size; idx++)
  {
    // Allocate Rocket
    population->rockets[idx] = newRocket(dna_length, init_pos);
  }
  
  return population;
}

Population *nextGen(Population *population)
{
  
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
Board newBoard(int width, int height, int n_obstacles)
{
  
}

// Fitness function
int fitness(Board board, Rocket rocket)
{
  
}