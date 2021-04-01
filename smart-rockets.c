#include <float.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

#include <smart-rockets.h>

typedef struct
{
  Population *population;
  unsigned short id_thread;
} SortPopulationArgs;

void* multithreadedMergeSort(void *args);
void mergeSort(Population *population, int low, int high);
void merge(Population *population, int low, int mid, int high);

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

// Sort population by fitness value
void sortPopulation(Population *population)
{
  pthread_t threads[THREAD_MAX];
  int max = population->size;

  SortPopulationArgs **args_array = malloc(max*sizeof(SortPopulationArgs*));
  for(int i = 0; i < THREAD_MAX; i++)
  {
    args_array[i] = malloc(sizeof(SortPopulationArgs*));
    args_array[i]->population = population;
    args_array[i]->id_thread = i+1;
  }
  // creating 4 threads
  for(int i = 0; i < THREAD_MAX; i++) 
    pthread_create(&threads[i], NULL, multithreadedMergeSort, (void*) args_array[i]);
  // joining all 4 threads
  for(int i = 0; i < THREAD_MAX; i++)
    pthread_join(threads[i], NULL);
  for(int i = 0; i < THREAD_MAX; i++)
    free(args_array[i]);
  
  // merging the final 4 parts
  merge(population, 0, (max/ 2 - 1) / 2, max / 2 - 1);
  merge(population, max / 2, max/2 + (max-1-max/2)/2, max - 1);
  merge(population, 0, (max - 1)/2, max - 1);
}

void* multithreadedMergeSort(void *args)
{
    // which part out of 4 parts
    unsigned short part = (unsigned short) ((SortPopulationArgs*) args)->id_thread;
    int size = ((SortPopulationArgs*) args)->population->size;
  
    // calculating low and high
    int low = part * (size / 4);
    int high = (part + 1) * (size / 4) - 1;
  
    // evaluating mid point
    int mid = low + (high - low) / 2;
    if (low < high) 
    {
      mergeSort(((SortPopulationArgs*) args)->population, low, mid);
      mergeSort(((SortPopulationArgs*) args)->population, mid + 1, high);
      merge(((SortPopulationArgs*) args)->population, low, mid, high);
    }
}

// merge sort function
void mergeSort(Population *population, int low, int high)
{
    // calculating mid point of array
    int mid = low + (high - low) / 2;
    if (low < high) {
  
        // calling first half
        mergeSort(population, low, mid);
  
        // calling second half
        mergeSort(population, mid + 1, high);
  
        // merging the two halves
        merge(population, low, mid, high);
    }
}

void merge(Population *population, int low, int mid, int high)
{
    Rocket** left = malloc((mid - low + 1)*sizeof(Rocket*));
    Rocket** right = malloc((high - mid)*sizeof(Rocket*));
  
    // left_size is size of left part and right_size is the size of right part
    int left_size = mid - low + 1;
    int right_size = high - mid;
    int i, j;
  
    // storing values in left part
    for (i = 0; i < left_size; i++)
      left[i] = population->rockets[i + low];
    // storing values in right part
    for (i = 0; i < right_size; i++)
      right[i] = population->rockets[i + mid + 1];
    
    // merge left and right in ascending order
    int k = low;
    i = j = 0;
    while (i < left_size && j < right_size) 
    {
      if (left[i] <= right[j])
        population->rockets[k++] = left[i++];
      else
        population->rockets[k++] = right[j++];
    }
  
    // insert remaining values from left
    while (i < left_size) 
      population->rockets[k++] = left[i++];
    // insert remaining values from right
    while (j < right_size)
      population->rockets[k++] = right[j++];
}