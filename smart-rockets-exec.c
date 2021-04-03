#include <stdio.h>
#include <stdlib.h>

#include <smart-rockets.h>

int debug(){
  // DEBUG
  int initial_position[2] = {100,100};
  int target[2] = {100,0};
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

  Board *board = newBoard(width, height, target, n_obstacles);
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
  Board *board_1 = newBoard(width, height, target, n_obstacles);
  printf("Fitness Bad: %.2f | Good: %.2f \n", fitness(board, rocket_bad), fitness(board, rocket_good));
  destroyRocket(rocket_bad);
  destroyRocket(rocket_good);
  destroyBoard(board_1);
  printf("____________________________________________________\n\n");
  
  Board *board_2 = newBoard(width, height, target, n_obstacles);
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
  Board *board_3 = newBoard(width, height, target, n_obstacles);
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

int main(){
  debug();
  
  return 0;
}
