#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include <smart-rockets.h>

int debug(){
  // DEBUG
  int initial_position[2] = {100,100};
  int target[2] = {100,0};
  int target_radius = 5;
  int dna_length = 50;
  int population_size = 20;
  int width = 500;
  int height = 500;
  int n_obstacles = 5;
  float mutation_factor = 5;
  
  Rocket *rocket = newRocket(dna_length, initial_position);
  
  printf("Rocket position: (%.2f,%.2f) \n", rocket->x, rocket->y);
  for(int idx = 0; idx<rocket->dna_length; idx++)
  {
    printf("Gene %d: (%.2f,%.2f) \n", idx, rocket->dna[idx][0], rocket->dna[idx][1]);
  }  
  destroyRocket(rocket);
  printf("____________________________________________________\n\n");

  Population *population = newPopulation(population_size, dna_length, initial_position, mutation_factor);
  printf("Population size: %d \n", population->size);
  for(int idx = 0; idx<population->size; idx++)
  {
    printf("Rocket %d position: (%.2f,%.2f) \n", idx, population->rockets[idx]->x, population->rockets[idx]->y);
  }
  destroyPopulation(population);
  printf("____________________________________________________\n\n");

  Board *board = newBoard(width, height, target, target_radius, n_obstacles);
  for(int idx = 0; idx<board->n_obstacles; idx++)
  {
    printf("Obstacle %d: (%d,%d) (%d,%d) \n", idx, board->obstacles[idx]->x0, board->obstacles[idx]->y0, 
           board->obstacles[idx]->x1, board->obstacles[idx]->y1);
  }
  destroyBoard(board);
  printf("____________________________________________________\n\n");

  Rocket *rocket_a = newRocket(dna_length, initial_position);
  Rocket *rocket_b = newRocket(dna_length, initial_position);
  
  Rocket *rocket_c = breed(rocket_a, rocket_b, mutation_factor, initial_position);
  
  for(int idx = 0; idx<rocket_a->dna_length; idx++)
  {
    printf("Gene %d: A:(%.2f,%.2f) | B:(%.2f,%.2f) | C:(%.2f,%.2f) \n", idx, rocket_a->dna[idx][0], rocket_a->dna[idx][1], 
           rocket_b->dna[idx][0], rocket_b->dna[idx][1], rocket_c->dna[idx][0], rocket_c->dna[idx][1]);
  }
  destroyRocket(rocket_a);
  destroyRocket(rocket_b);
  destroyRocket(rocket_c);
  printf("____________________________________________________\n\n");

  Rocket *rocket_bad = newRocket(dna_length, initial_position);
  Rocket *rocket_good = newRocket(dna_length, target);
  Board *board_1 = newBoard(width, height, target, target_radius, n_obstacles);
  printf("Fitness Bad: %.2f | Good: %.2f \n", fitness(board, rocket_bad), fitness(board, rocket_good));
  destroyRocket(rocket_bad);
  destroyRocket(rocket_good);
  destroyBoard(board_1);
  printf("____________________________________________________\n\n");
  
  Board *board_2 = newBoard(width, height, target, target_radius, n_obstacles);
  Population *population_1 = newPopulation(population_size, dna_length, initial_position, mutation_factor);
  printf("Population size: %d \n", population_1->size);
  for(int frame_idx=0; frame_idx<dna_length; frame_idx++)
  {
    for(int idx=0; idx<population_1->size; idx++)
    {
      updateRocket(board_2, population_1->rockets[idx], frame_idx);
    }
  } 
  
  population_1 = nextGeneration(population_1, initial_position);
  for(int idx=0; idx<population_1->size; idx++)
  {
    printf("Rocket %d position: (%.2f,%.2f) | Fitness: %.10f \n", idx, population_1->rockets[idx]->x, population_1->rockets[idx]->y, population_1->rockets[idx]->fitness_score);
  }

  destroyPopulation(population_1);
  destroyBoard(board_2);
  printf("____________________________________________________\n\n");

  printf("=> Test: should sort population: \n");
  Board *board_3 = newBoard(width, height, target, target_radius, n_obstacles);
  Population *population_2 = newPopulation(population_size, dna_length, initial_position, mutation_factor);
  printf("Population size: %d \n", population_2->size);
  for(int frame_idx=0; frame_idx<dna_length; frame_idx++)
    for(int idx=0; idx<population_2->size; idx++)
      updateRocket(board_3, population_2->rockets[idx], frame_idx);

  printf("Original order: \n");
  for (int idx=0; idx<population_2->size; idx++)
    printf("Rocket %d | Fitness: %.10f \n", idx, population_2->rockets[idx]->fitness_score);
  sortPopulation(population_2);
  printf("Sorted: \n");
  for (int idx=0; idx<population_2->size; idx++)
    printf("Rocket %d | Fitness: %.10f \n", idx, population_2->rockets[idx]->fitness_score);

  destroyPopulation(population_2);
  destroyBoard(board_3);
  printf("____________________________________________________\n\n");

  return 0;
}

struct Record
{
  float x,y;
};

struct Args
{
  int id;
  int generation;
  Rocket *rocket;
  Board *board;
  struct Record **record_buffer;
};

void drawGeneration(Board *board, struct Record **record_buffer, int buffer_height, int buffer_width){
  for(int frame_idx=0; frame_idx<buffer_width; frame_idx++)
  {
    printf("\e[1;1H\e[2J");
    for(int j = board->height-1; j>=0; j--)
    {
      if(j == 0 || j == board->height-1){
        for(int i = 0; i < board->width; i++)
        {
          printf("-");
        }
        printf("\n");
      }
      for(int i = 0; i < board->width; i++)
      {
        for(int id = 0; id < buffer_height; id++)
        {
          if(((int)record_buffer[id][frame_idx].x) == i && 
            ((int)record_buffer[id][frame_idx].y) == j)
          {
            printf("#");
            break;
          }
          else
          {
            
          }
        }
        printf(" ");
      }
      printf("\n");
    }
    printf("\n\n");
  }
}

void *lifecycle(void *args)
{
  int id = ((struct Args *) args)->id;
  int generation = ((struct Args *) args)->generation;
  Rocket *rocket = ((struct Args *) args)->rocket;
  Board *board = ((struct Args *) args)->board;
  struct Record **record_buffer = ((struct Args *) args)->record_buffer;
  record_buffer[id] = malloc(sizeof(struct Record)*(rocket->dna_length));
  free((struct Args *) args);
  int frame_idx = 0;
  while(frame_idx < rocket->dna_length)
  {
    if(rocket->alive == 1)
      updateRocket(board, rocket, frame_idx);
    
    record_buffer[id][frame_idx].x = rocket->x;
    record_buffer[id][frame_idx].y = rocket->y;
    frame_idx++;
    usleep(1000);
  }
}

int main()
{
  srand(time(0));
  int dna_length = 500;
  int population_size = 200;
  unsigned int generation = 1;

  int width = 1000;
  int height  = 1000;
  int initial_position[2] = {width/2, height/4};
  int target[2] = {width/2, 3*height/4};
  int target_radius = 50;
  int n_obstacles = 1;
  float mutation_factor = 1;

  Board *board = newBoard(width, height, target, target_radius, n_obstacles);
  board->obstacles[0]->x0 = 250;
  board->obstacles[0]->y0 = 400;
  board->obstacles[0]->x1 = 750;
  board->obstacles[0]->y1 = 600;
  Population *population = newPopulation(population_size, dna_length, initial_position, mutation_factor);
  
  pthread_t threads[population_size];
  while(1)
  {
    struct Record **records = malloc(sizeof(struct Record*)*population_size);
    for(int i = 0; i < population_size; i++)
    {
      struct Args* args = malloc(sizeof(struct Args));
      args->id = i;
      args->generation = generation;
      args->rocket = population->rockets[i];
      args->board = board;
      args->record_buffer = records;
      pthread_create(&threads[i], NULL, lifecycle, (void *) args);
    }
    for(int i = 0; i < population_size; i++)
      pthread_join(threads[i], NULL);
    
    float sum_fitness = 0;
    float sum_final_x = 0;
    float sum_final_y  = 0;
    for(int i = 0; i < population_size; i++)
    {
      sum_fitness += population->rockets[i]->fitness_score;
      struct Record record = records[i][dna_length-1];
      sum_final_x += record.x;
      sum_final_y += record.y;
    }
    printf("%d\n",generation);
    printf("Mean generation fitness : %.2f\n", sum_fitness/population_size);
    printf("Target position : (%d,%d) \n", target[0], target[1]);
    printf("Mean final position: (%.2f,%.2f) \n", sum_final_x/population_size, sum_final_y/population_size);
    printf("-----------------\n");
    //drawGeneration(board, records, population_size, dna_length);
    
    population = nextGeneration(population, initial_position);
    generation += 1;
  }
  return 0;
}
