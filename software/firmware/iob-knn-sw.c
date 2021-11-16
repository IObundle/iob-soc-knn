#include <stdlib.h>

#include "system.h"
#include "periphs.h"
#include "iob-uart.h"
#include "iob_timer.h"
#include "printf.h"

#define N 10  // data set size
#define K 4   // number of neighbours (K)
#define C 4   // number data classes
#define M 4   // number samples to be classified

#define INFINITE ~0 //-1

//
// Data structures
//

// labeled dataset
struct datum {
  short x;
  short y;
  unsigned char label;
} data[N], x[M];

// neighbor info
struct neighbor {
  unsigned int idx; //index in dataset array
  unsigned int dist; //distance to test point
} neighbor[K];

//
// Functions
//

// square distance between 2 points a and b
unsigned int sq_dist( struct datum a, struct datum b) {
  short X = a.x-b.x;
  unsigned int X2=X*X;
  short Y = a.y-b.y;
  unsigned int Y2=Y*Y;
  return (X2 + Y2);
}

// insert element in ordered array of neighbours
void insert (struct neighbor element, unsigned int position) {
  for (int j=K-1; j>position; j--)
    neighbor[j] = neighbor[j-1];

  neighbor[position] = element;

}


///////////////////////////////////////////////////////////////////
void knn(void) {

  // int vote accumulator
  int votes_acc[C] = {0};

  // generate random seed
  //srand(timer_time_us());   // Initialization, should only be called once.

  // init dataset
  for (int i=0; i<N; i++) {

    // init coordinates
    data[i].x = (short) rand();
    data[i].y = (short) rand();

    // init label
    data[i].label = (unsigned char) (rand()%C);
  }

#ifdef DEBUG
  printf("\n\n\nDATASET\n");
  printf("Idx \tX \tY \tLabel\n");
  for (int i=0; i<N; i++)
    printf("%d \t%d \t%d \t%d\n", i, data[i].x,  data[i].y, data[i].label);
#endif
  
  // init test points
  for (int k=0; k<M; k++) {
    x[k].x  = (short) rand();
    x[k].y  = (short) rand();
    //x[k].label will be calculated by the algorithm
  }

#ifdef DEBUG
  printf("\n\nTEST POINTS\n");
  printf("Idx \tX \tY\n");
  for (int k=0; k<M; k++)
    printf("%d \t%d \t%d\n", k, x[k].x, x[k].y);
#endif
  
  //
  // PROCESS DATA
  //

  // start knn here
  printf("Start KNN...");

  // Start time
  unsigned int start_time = timer_time_us();
  
  for (int k=0; k<M; k++) { // for all test points
  // compute distances to dataset points

#ifdef DEBUG
    printf("\n\nProcessing x[%d]:\n", k);
#endif

    // init all k neighbors infinite distance
    for (int j=0; j<K; j++)
      neighbor[j].dist = INFINITE;

#ifdef DEBUG
    printf("Datum \tX \tY \tLabel \tDistance\n");
#endif
    for (int i=0; i<N; i++) { // for all dataset points
      // compute distance to x[k]
      unsigned int d = sq_dist(x[k], data[i]);

      //insert in ordered list
      for (int j=0; j<K; j++)
        if ( d < neighbor[j].dist ) {
          insert( (struct neighbor){i,d}, j);
          break;
        }

#ifdef DEBUG
      // dataset
      printf("%d \t%d \t%d \t%d \t%d\n", i, data[i].x, data[i].y, data[i].label, d);
#endif

    }

    
    // classify test point

    // clear all votes
    int votes[C] = {0};
    int best_votation = 0;
    int best_voted = 0;

    // make neighbours vote
    for (int j=0; j<K; j++) { // for all neighbors
      if ( (++votes[data[neighbor[j].idx].label]) > best_votation ) {
        best_voted = data[neighbor[j].idx].label;
        best_votation = votes[best_voted];
      }
    }

    x[k].label = best_voted;

    votes_acc[best_voted]++;
    
#ifdef DEBUG
    printf("\n\nNEIGHBORS of x[%d]=(%d, %d):\n", k, x[k].x, x[k].y);
    printf("K \tIdx \tX \tY \tDist \t\tLabel\n");
    for (int j=0; j<K; j++)
      printf("%d \t%d \t%d \t%d \t%d \t%d\n", j+1, neighbor[j].idx, data[neighbor[j].idx].x,  data[neighbor[j].idx].y, neighbor[j].dist,  data[neighbor[j].idx].label);
    
    printf("\n\nCLASSIFICATION of x[%d]:\n", k);
    printf("X \tY \tLabel\n");
    printf("%d \t%d \t%d\n\n\n", x[k].x, x[k].y, x[k].label);

#endif

  } // all test points classified

  // stop knn here
  // read current timer count, compute elapsed time
  unsigned int end_time = timer_time_us();
  unsigned int elapsedu = end_time - start_time;

  printf("done!\n");
  printf("\nExecution time: %dus @%dMHz\n\n", elapsedu, FREQ/1000000);

  
  //print classification distribution to check for statistical bias
  for (int l=0; l<C; l++)
    printf("%d ", votes_acc[l]);
  printf("\n");

  return;
}


