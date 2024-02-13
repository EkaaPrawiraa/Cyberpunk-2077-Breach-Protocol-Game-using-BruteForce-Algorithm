#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "algorithm.h"
#include <Windows.h>


unsigned int generateSeed() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;


    if (!QueryPerformanceFrequency(&frequency)) {
        fprintf(stderr, "Error: Unable to get performance frequency\n");
        exit(EXIT_FAILURE);
    }


    if (!QueryPerformanceCounter(&counter)) {
        fprintf(stderr, "Error: Unable to get performance counter\n");
        exit(EXIT_FAILURE);
    }

    unsigned int seed = (unsigned int)((counter.QuadPart * 1000000000) / frequency.QuadPart);

    return seed;
}

void buildMatrix(Matrix *matrixs,int row, int col,int *token,int total)
{
    // Menggunakan waktu sebagai seed untuk nilai acak
    srand(generateSeed());

    // Mengisi matriks dengan nilai acak dari array
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            // Memilih nilai acak dari array
            int randomIndex = rand() % (total);
            matrixs->mem[i][j] = token[randomIndex];
            
        }
    }
    matrixs->width=col;
    matrixs->height=row;
}

void buildSequence(sequence *sequences, int maxSeq, int *token,int total)
{
  srand(generateSeed());
  sequences->lengs=rand() % (maxSeq-2+1)+2;
  // printf(" panjang %d \n",sequences->lengs);
  sequences->rewards=rand() % (100-1+1)+1;
  // printf(" rewards %d \n",sequences->rewards);
  for (int i=0;i<sequences->lengs;i++)
  {
    int randomIndex = rand() % (total);
    sequences->seqs[i]=token[randomIndex];
    // printf("%X \n",sequences->seqs[i]);
  }


}

int compareRewards(const void *a, const void *b) {
    return ((sequence *)b)->rewards - ((sequence *)a)->rewards;
}
void readMatrix(FILE *file, Matrix *matrix, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fscanf(file, "%X", &matrix->mem[i][j]);
        }
    }
    matrix->width=width;
    matrix->height=height;
}
void readSequences(FILE *file, sequence *sequences, int* leng) {
    int value=0;
    int i =0;
    char newline;
    while(fscanf(file, "%X%c", &value, &newline) == 2){
        sequences->seqs[i] = value;
        i++;
        if (newline == '\n')
        {
            break;
        }
    }
    *leng = i;
    sequences->lengs=i;
}


    void displayMatrix(Matrix matrix,int row, int col)
{
    int i, j;
    for (i = 0; i <= row-1; i++)
    {
        for (j = 0; j <= col-1; j++)
        {
            if (j == col-1)
            {
                if (i == row-1)
                {
                    printf("%X", ELMT(matrix, i, j));
                }
                else
                {
                    printf("%X\n", ELMT(matrix, i, j));
                }
            }
            else
            {
                printf("%X ", ELMT(matrix, i, j));
            }
        }

    
    }
    printf("\n");
}


bool sameCoordinateToken (int token, Matrix m, Coordinate p)
{
    
    return (token == m.mem[p.x][p.y]);
}

bool isValidBuffer (Buffer b, Matrix m, sequence s)

{
    int i, j;
    if (b.lengths >= s.lengs) {
        i = 0;
        while (i <= (b.lengths - s.lengs)) {
            if (s.seqs[0]==m.mem[(b).container[i].x][(b).container[i].y])
             {
                j = 1;
                while (j < s.lengs && s.seqs[j]==m.mem[(b).container[i+j].x][(b).container[i+j].y])
                 {
                    j++;
                }
                if (j == s.lengs) {
                    return true;
                }
            }
            i++;
        }
    }
    return false;
}
int bufferScore (Buffer b, Matrix m, sequence *l,int numSequences)
{
    int i;
    int n = 0;
    for (i = 0; i < numSequences; i++) {
        if (isValidBuffer(b,m,l[i])) {
            n += l[i].rewards;
        }
    }
    return n;
}


void createCoordinate (Coordinate *coord,int a, int b)
{
    coord->x = a;
    coord->y = b;
}

void push (Buffer *b, Coordinate p)
{
    b->container[b->lengths]=p;
    b->lengths++;
}

void pop (Buffer *b)
{
    b->lengths--;
}
bool isValidPoint (Buffer b, int x, int y)
{
    int i;
    for (i = 0; i < b.lengths; i++) {
        if (b.container[i].x==x && b.container[i].y==y)
     {
            return false;
        }
    }
    return true;
}

void solvePuzzle(Matrix matrix, sequence *sequences, int numSequences, Buffer *buffer, int *finalScore) {
    int index = 0;
    int counters[buffer->buffSize];
    Coordinate point;
    Buffer tempBuffer;
    int maxScore = 0;
    int tempScore;
    // printf("Sempet");
    if (buffer->buffSize > 0) {
    
        for (int i = 0; i < buffer->buffSize; i++) {
            counters[i] = 0;
        }

        for (int i = 0; i < numSequences; i++) {
            maxScore += sequences[i].rewards;
        }

        tempBuffer.lengths = 0;
        tempBuffer.buffSize = buffer->buffSize;

        while (counters[0] < matrix.width && *finalScore != maxScore) {
            if (index == 0) {
                createCoordinate(&point, 0, counters[index]);
                push(&tempBuffer, point);
                index++;
            } else if (index < buffer->buffSize - 1) {
                if (index % 2 != 0) {
                    if (counters[index] != matrix.height) {
                        if (isValidPoint(tempBuffer, counters[index], counters[index - 1])) {
                            createCoordinate(&point, counters[index], counters[index - 1]);
                            push(&tempBuffer, point);
                            tempScore = bufferScore(tempBuffer, matrix, sequences, numSequences);
                            if (tempScore > *finalScore) {
                                *finalScore = tempScore;
                                *buffer = tempBuffer;
                            }
                            index++;
                        } else {
                            counters[index]++;
                        }
                    } else {
                        pop(&tempBuffer);
                        counters[index] = 0;
                        index--;
                        counters[index]++;
                    }
                } else {
                    if (counters[index] != matrix.width) {
                        if (isValidPoint(tempBuffer, counters[index - 1], counters[index])) {
                            createCoordinate(&point, counters[index - 1], counters[index]);
                            push(&tempBuffer, point);
                            tempScore = bufferScore(tempBuffer, matrix, sequences, numSequences);
                            if (tempScore > *finalScore) {
                                *finalScore = tempScore;
                                *buffer = tempBuffer;
                            }
                            index++;
                        } else {
                            counters[index]++;
                        }
                    } else {
                        pop(&tempBuffer);
                        counters[index] = 0;
                        index--;
                        counters[index]++;
                    }
                }
            } else {
                if (index % 2 != 0) {
                    while (counters[index] < matrix.height && *finalScore != maxScore) {
                        if (isValidPoint(tempBuffer, counters[index], counters[index - 1])) {
                            createCoordinate(&point, counters[index], counters[index - 1]);
                            push(&tempBuffer, point);
                            tempScore = bufferScore(tempBuffer, matrix, sequences, numSequences);
                            if (tempScore > *finalScore) {
                                *finalScore = tempScore;
                                *buffer = tempBuffer;
                            }
                            counters[index]++;
                            pop(&tempBuffer);
                        } else {
                            counters[index]++;
                        }
                    }
                } else {
                    while (counters[index] < matrix.width && *finalScore != maxScore) {
                        if (isValidPoint(tempBuffer, counters[index - 1], counters[index])) {
                            createCoordinate(&point, counters[index - 1], counters[index]);
                            push(&tempBuffer, point);
                            tempScore = bufferScore(tempBuffer, matrix, sequences, numSequences);
                            if (tempScore > *finalScore) {
                                *finalScore = tempScore;
                                *buffer = tempBuffer;
                            }
                            counters[index]++;
                            pop(&tempBuffer);
                        } else {
                            counters[index]++;
                        }
                    }
                }
                counters[index] = 0;
                index--;
                counters[index]++;
                pop(&tempBuffer);
            }
        }
    } else {
        *finalScore = 0;
    }
}

void displayCoordinate (Coordinate coord)
{
    printf("%d, %d", coord.y+1, coord.x+1);
}

void displayBuff (Buffer b)
{
    int i;
    for (i = 0; i < b.lengths; i++) {
        displayCoordinate(b.container[i]);
        printf("\n");
    }
}

void displayBuffMat (Buffer b, Matrix m)
{
    int i;
    Coordinate current;
    for (i = 0; i < b.lengths; i++) {
        current = b.container[i];
        printf("%X ",m.mem[current.x][current.y]);
    
    }
}
void saveText(Buffer buffer,int t,int totalscore, Matrix matrixs)
{
  
  char response;
    printf("Apakah ingin menyimpan solusi? (y/n): ");
    scanf(" %c", &response);

    // Jika pengguna menyetujui, simpan ke dalam file .txt
    if (response == 'y' || response == 'Y')
    {
        
        char fileName[50];
        printf("Masukkan nama berkas (.txt): ");
        scanf("%s", fileName);

        // Buka atau buat berkas .txt
        FILE *f = fopen(fileName, "w");
        if (f == NULL)
            {
            perror("Error opening file");
            exit(EXIT_FAILURE);
            }
        if (totalscore==0)
            {
          fprintf(f,"No solutions\n");
            }
        else{
          fprintf(f, "%d\n", totalscore);
          for (int i =0;i<buffer.lengths;i++)
          {
            fprintf(f,"%X ",matrixs.mem[buffer.container[i].x][buffer.container[i].y]);
          }
           fprintf(f, "\n");
           for (int i =0;i<buffer.lengths;i++)
           {
            fprintf(f,"%d, %d\n",buffer.container[i].y +1,buffer.container[i].x +1);
           }
           fprintf(f,"\n %dms",t);
          fclose(f);
          printf("Hasil disimpan ke dalam berkas %s\n", fileName);
          }
    }
  
}

