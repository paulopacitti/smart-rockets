#include <stdio.h>
#include <stdlib.h>

#include <smart-rockets.h>

int debug(){
  // DEBUG
  int initial_pos[2] = {100,0};
  int dna_length = 50;
  int population_size = 50;
  
  Rocket *rocket = newRocket(dna_length, initial_pos);
  
  printf("Rocket position: (%.2f,%.2f) \n", rocket->x, rocket->y);
  for(int idx = 0; idx<rocket->dna_length; idx++)
  {
    printf("Gene %d: (%.2f,%.2f) \n", idx, rocket->dna[idx][0], rocket->dna[idx][1]);
  }
  destroyRocket(rocket);
  
  Population *population = newPopulation(population_size, dna_length, initial_pos);
  printf("Population size: %d \n", population->size);
  for(int idx = 0; idx<population->size; idx++)
  {
    printf("Rocket %d position: (%.2f,%.2f) \n", idx, population->rockets[idx]->x, population->rockets[idx]->y);
  }
  destroyPopulation(population);
  
  return 0;
}

int main(){
  debug();
  
  return 0;
}
