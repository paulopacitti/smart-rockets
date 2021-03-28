#define GENE_LENGTH 10
#define POPULATION_SIZE 50
#define N_OBSTACLES 5

int libFunc();

// Rockets
typedef struct
{
  float x,y;
  float vel[2];
  float **dna;
  int dna_length;
  float fitness_score;
} Rocket;

// Population
typedef struct
{
  Rocket **rockets;
  int size;
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
  Obstacle *obstacles;
  int n_obstacles;
} Board;

// Rocket creation/destruction
Rocket *newRocket(int dna_length, int init_pos[2]);

Rocket *breed(Rocket *parent_a, Rocket *parent_b);

void destroyRocket(Rocket *rocket);

// Population creation
Population *newPopulation(int size, int dna_length, int init_pos[2]);

Population *nextGen(Population *population);

void destroyPopulation(Population *population);

// Board creation
Board newBoard(int width, int height, int n_obstacles);

// Fitness function
int fitness(Board board, Rocket rocket);
