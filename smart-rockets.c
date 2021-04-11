#include <float.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <smart-rockets.h>

typedef struct
{
  Population *population;
  int id_thread;
} SortPopulationArgs;

typedef struct
{
  Obstacle *obstacle;
  Rocket *rocket;
  int *obstacle_hit;
} ObstacleCollisionArgs;

void* multithreadedMergeSort(void *args);
void mergeSort(Population *population, int low, int high);
void merge(Population *population, int low, int mid, int high);

// Rocket creation
Rocket *newRocket(int dna_length, int initial_position[2])
{
  // Allocate rocket memory
  Rocket *rocket = malloc(sizeof(Rocket));

  //Set rocket is alive
  rocket->alive = 1;
  
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
    rocket->dna[idx][0] = (rand() % 6) - 3;
    rocket->dna[idx][1] = (rand() % 6) - 3;
  }

  return rocket;
}

Rocket *breed(Rocket *parent_a, Rocket *parent_b, int mutation_factor, int initial_position[2])
{
  // Allocate rocket memory
  Rocket *rocket =  malloc(sizeof(Rocket));

  //Set rocket is alive
  rocket->alive = 1;
  
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
      rocket->dna[idx][0] = (rand() % 6) - 3;
      rocket->dna[idx][1] = (rand() % 6) - 3;
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
  
  sortPopulation(population);
  // Breed 1/4 best rockets
  for(int idx=0; idx<population->size; idx++){
    Rocket *parent_a = population->rockets[rand() % (int)(population->size / 4)];
    Rocket *parent_b = population->rockets[rand() % (int)(population->size / 4)];
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
Board *newBoard(int width, int height, int target[2],int target_radius, int n_obstacles)
{
  // Allocate board memory
  Board *board = malloc(sizeof(Board));
  
  // Set width and height
  board->width = width;
  board->height = height;
  
  // Set target
  board->target[0] = target[0];
  board->target[1] = target[1];
  board->target_radius = target_radius;
  
  // Set number of obstacles
  board->n_obstacles = n_obstacles;
  
  // Allocate board's obstacles memory
  board->obstacles = malloc(sizeof(Board) * n_obstacles);
  
  for(int idx = 0; idx<n_obstacles; idx++)
  {
    // Free Rocket
    int var_temp;
    int x0 = rand() % width;
    int y0 = rand() % height;
    int x1 = x0 + 30;
    int y1 = y0 + 10;
   
    if (x0 > x1){
      var_temp = x0;
      x0 = x1;
      x1 = var_temp;
    }
    if (y0 > y1){
      var_temp = y0;
      y0 = y1;
      y1 = var_temp;
    }
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
  obstacle->y1 = y1;
  
  return obstacle;
}

void destroyObstacle(Obstacle *obstacle)
{
  // Free obstacle
  free(obstacle);
}

//Distance utility function
float distance(float x0, float y0, float x1, float y1)
{
  return sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

// Fitness function
float fitness(Board *board, Rocket *rocket)
{
  // Calculate rocket's distance from board's target
  float targetDistance = distance(rocket->x, rocket->y, board->target[0], board->target[1]);
  
  // Return Maximum float if rocket is right on target else 1/distance
  return ((rocket->x - board->target[0]) == 0 && (rocket->y - board->target[1]) == 0) ?
    FLT_MAX :
    1000/targetDistance;
}

void *checkObstacleCollision(void *args)
{
  Rocket *rocket = ((ObstacleCollisionArgs *)args)->rocket;
  Obstacle *obstacle = ((ObstacleCollisionArgs *)args)->obstacle;
  int *obstacle_hit = ((ObstacleCollisionArgs *)args)->obstacle_hit;
  free(args);
  
  // Check obstacle collision
  if ((rocket->x >= fmin(obstacle->x0, obstacle->x1) && 
       rocket->x <= fmax(obstacle->x0, obstacle->x1)) ||
      (rocket->y >= fmin(obstacle->y0, obstacle->y1) && 
       rocket->y <= fmax(obstacle->y0, obstacle->y1)))
    *obstacle_hit = 1;
}

void checkCollisions(Board *board, Rocket *rocket)
{
  // Check wall collision
  if (rocket->x < 0 || rocket->x >= board->width || 
      rocket->y < 0 || rocket->y >= board->height)
    rocket->alive = 0;
  
  // Check target collision
  if(distance(rocket->x, rocket->y, board->target[0], board->target[1]) <= board->target_radius)
    rocket->alive = 0;
  
  // Check obstacles collision
  pthread_t threads[board->n_obstacles];
  int **obstacle_hit = malloc(sizeof(int)*board->n_obstacles);
  // Launch threads
  for(int idx = 0; idx < board->n_obstacles; idx++){
    ObstacleCollisionArgs *args = malloc(sizeof(ObstacleCollisionArgs));
    args->obstacle = board->obstacles[idx];
    args->rocket = rocket;
    obstacle_hit[idx] = malloc(sizeof(int));
    args->obstacle_hit = obstacle_hit[idx];
    pthread_create(&threads[idx], NULL, checkObstacleCollision, (void*) args);
  }

  // Join threads
  for(int idx = 0; idx < board->n_obstacles; idx++)
  {
    pthread_join(threads[idx], NULL);
  }
  
  // Check for hits
  for(int idx = 0; idx < board->n_obstacles; idx++)
  {
    if (rocket->alive > 1 && obstacle_hit[idx] > 0)
        rocket->alive = 0;
  }
}

// Rocket update
void updateRocket(Board *board, Rocket *rocket, int frame_idx)
{
  // TODO: Check for max velocity
  // Update rocket's velocity
  rocket->velocity[0] += rocket->dna[frame_idx][0];
  rocket->velocity[1] += rocket->dna[frame_idx][1];
  
  // Update rocket's position
  rocket->x += rocket->velocity[0];
  rocket->y += rocket->velocity[1];
  
  // Check collision against board's walls, target and obstacles
  checkCollisions(board, rocket);
  
  // Update rocket's fitness score
  rocket->fitness_score = fitness(board, rocket);
}

// Sort population by fitness value
void sortPopulation(Population *population)
{
  pthread_t threads[THREAD_MAX];
  int max = population->size;

  SortPopulationArgs **args_array = malloc(THREAD_MAX*sizeof(SortPopulationArgs*));
  for(int i = 0; i < THREAD_MAX; i++)
  {
    args_array[i] = malloc(sizeof(SortPopulationArgs*));
    args_array[i]->population = population;
    args_array[i]->id_thread = i;
  }
  // Creating threads
  for(int i = 0; i < THREAD_MAX; i++) 
    pthread_create(&threads[i], NULL, multithreadedMergeSort, (void*) args_array[i]);
  // Joining all threads
  for(int i = 0; i < THREAD_MAX; i++)
    pthread_join(threads[i], NULL);
  
  // Merging the final 4 parts
  merge(population, 0, (max/ 2 - 1) / 2, max / 2 - 1);
  merge(population, max / 2, max/2 + (max-1-max/2)/2, max - 1);
  merge(population, 0, (max - 1)/2, max - 1);

  for(int i = 0; i < THREAD_MAX; i++)
    free(args_array[i]);
}

// Each thread created sort using mergeSort
void* multithreadedMergeSort(void *args)
{
    int part = (int) ((SortPopulationArgs*) args)->id_thread;
    int size = ((SortPopulationArgs*) args)->population->size;
  
    // Calculating low and high
    int low = part * (size / THREAD_MAX);
    int high = (part + 1) * (size / THREAD_MAX) - 1;
  
    // Evaluating mid point
    int mid = low + (high - low) / 2;
    if (low < high) 
    {
      mergeSort(((SortPopulationArgs*) args)->population, low, mid);
      mergeSort(((SortPopulationArgs*) args)->population, mid + 1, high);
      merge(((SortPopulationArgs*) args)->population, low, mid, high);
    }
}

void mergeSort(Population *population, int low, int high)
{
    int mid = low + (high - low) / 2;
    if (low < high) {
      // Calling first half
      mergeSort(population, low, mid);
      // Calling second half
      mergeSort(population, mid + 1, high);
      // Merging the two halves
      merge(population, low, mid, high);
    }
}

void merge(Population *population, int low, int mid, int high)
{
    Rocket** left = malloc((mid - low + 1)*sizeof(Rocket*));
    Rocket** right = malloc((high - mid)*sizeof(Rocket*));
  
    int left_size = mid - low + 1;
    int right_size = high - mid;
    int i, j;
  
    // Storing values in left part
    for (i = 0; i < left_size; i++)
      left[i] = population->rockets[i + low];
    // Storing values in right part
    for (i = 0; i < right_size; i++)
      right[i] = population->rockets[i + mid + 1];
    
    // Merge left and right in ascending order
    int k = low;
    i = j = 0;
    while (i < left_size && j < right_size) 
    {
      if (left[i]->fitness_score >= right[j]->fitness_score)
        population->rockets[k++] = left[i++];
      else
        population->rockets[k++] = right[j++];
    }
  
    // Insert remaining values from left
    while (i < left_size) 
      population->rockets[k++] = left[i++];
    // Insert remaining values from right
    while (j < right_size)
      population->rockets[k++] = right[j++];

    free(left);
    free(right);
}