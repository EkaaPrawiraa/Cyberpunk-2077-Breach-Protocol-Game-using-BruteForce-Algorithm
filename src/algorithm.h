#ifndef PUZZLE_SOLVER_H
#define PUZZLE_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MAXS 100
#define ELMT(M, i, j) (M).mem[(i)][(j)]

typedef struct
{
    int seqs[MAXS];
    int lengs;
    int rewards;

} sequence;

typedef struct
{
    int x;
    int y;
} Coordinate;

typedef struct
{
    int mem[MAXS][MAXS];
    int height;
    int width;
} Matrix;

typedef struct
{
    int lengths;
    int buffSize;
    Coordinate container[MAXS];
} Buffer;

int compareRewards(const void *a, const void *b);

void readMatrix(FILE *file, Matrix *matrix, int width, int height);

void readSequences(FILE *file, sequence *sequences, int *leng);

void displayMatrix(Matrix matrix, int row, int col);

bool sameCoordinateToken(int token, Matrix m, Coordinate p);

bool isValidBuffer(Buffer b, Matrix m, sequence s);

int bufferScore(Buffer b, Matrix m, sequence *l, int numSequences);


void createCoordinate(Coordinate *coord, int a, int b);

void push(Buffer *b, Coordinate p);

void pop(Buffer *b);

bool isValidPoint(Buffer b, int x, int y);

void solvePuzzle(Matrix matrix, sequence *sequences, int numSequences, Buffer *buffer, int *finalScore);

void displayCoordinate(Coordinate coord);

void displayBuff(Buffer b);

void displayBuffMat(Buffer b, Matrix m);

void saveText(Buffer buffer, int t, int totalscore, Matrix matrixs);

unsigned int generateSeed();

// Function to build a random matrix
void buildMatrix(Matrix *matrixs, int row, int col, int *token, int total);

// Function to build a random sequence
void buildSequence(sequence *sequences, int maxSeq, int *token, int total);


#endif // PUZZLE_SOLVER_H
