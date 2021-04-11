#define THREAD_MAX 4 // changing this value breaks quick sort multithreaded immplementation!

// Rockets
typedef struct
{
  float x,y;
  float velocity[2];
  float **dna;
  int dna_length;
  int alive;
  float fitness_score;
} Rocket;

// Population
typedef struct
{
  Rocket **rockets;
  int size;
  int mutation_factor;
} Population;

// Obstacle
typedef struct
{
  int x0, y0, x1, y1;
} Obstacle;

// Board
typedef struct
{
  int width, height;
  int target[2];
  int target_radius;
  Obstacle **obstacles;
  int n_obstacles;
} Board;

// Rocket creation/destruction
Rocket *newRocket(int dna_length, int init_pos[2]);

Rocket *breed(Rocket *parent_a, Rocket *parent_b, int mutation_factor, int init_pos[2]);

void destroyRocket(Rocket *rocket);

// Population creation/destruction
Population *newPopulation(int size, int dna_length, int initial_position[2], int mutation_factor);

Population *nextGeneration(Population *population, int initial_position[2]);

void destroyPopulation(Population *population);

// Board creation/destruction
Board *newBoard(int width, int height, int target[2], int target_radius, int n_obstacles);

void destroyBoard(Board *board);

// Obstacle creation/destruction
Obstacle *newObstacle(int x0, int y0, int x1, int y1);

void destroyObstacle(Obstacle *obstacle);

// Fitness function
float fitness(Board *board, Rocket *rocket);

// Rocket update
void updateRocket(Board *board, Rocket *rocket, int frame_idx);

// Sort population by fitness value
void sortPopulation(Population *population);