#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "algorithm.h"


int main()
{
  clock_t t;
  int bufferSize;
  int matrixWidth, matrixHeight;
  Matrix matrixs;
  int numSequences;
  int totaltoken;
  int buffers;
  int sequenceSize;
  sequence seqnums[numSequences];
  int totalscore =0;
  Buffer buffer;
 char answer;
 
  printf("Cyberpunk 2077 Breach Protocol\n");
  printf("\n\nPILIH MODE PERMAINAN :\n");
  printf("1. Inputan text.\n");
  printf("2. Inputan Pemain.\n");
  
  printf("Input: ");
  scanf("%c", &answer);
  while (answer != '1' && answer != '2')
  {
    printf("Hanya ada dua pilihan.\n");
    scanf("Input: %c",&answer);
  }

  if (answer == '1')
  {
 
    FILE *file = fopen("input.txt", "r");
      if (file == NULL) {
          printf("Gagal membuka berkas\n");
          return 1;
        }
    // Baca buffer size
    fscanf(file, "%d", &bufferSize);
    // Baca jumlah kolom dan baris
        
    fscanf(file, "%d %d", &matrixWidth, &matrixHeight);
    // Baca Matrix
    readMatrix(file, &matrixs, matrixWidth, matrixHeight);
    printf("Matrix :\n");
    displayMatrix(matrixs,matrixHeight,matrixWidth);

// baca sequence
    
    fscanf(file, "%d", &numSequences);
    

    for (int i = 0; i < numSequences; ++i) {
        int leng=0;
            readSequences(file, &seqnums[i] ,&leng);
            int reward;
            fscanf(file, "%d", &reward);
            seqnums[i].rewards=reward;
            printf("Sequences %d:\n", i + 1);
            for (int j = 0; j < leng; ++j) {
                printf("%X ",seqnums[i].seqs[j]);
            }
            printf("\nReward: %d\n", reward);
        }
        qsort(seqnums, numSequences, sizeof(sequence), compareRewards);
  buffer.lengths = 0;
  buffer.buffSize = bufferSize;
  t = clock();   // Temukan sekuens optimal
  solvePuzzle(matrixs,seqnums,numSequences,&buffer, &totalscore);

    // Cetak hasil optimal
   printf("\n\nSkor optimal: %d\n", totalscore);
        if (totalscore == 0) {
            printf("Tidak ada solusi\n\n");
        } else {
            printf("Buffer: "); displayBuffMat(buffer,matrixs);printf("\n");
            printf("Lokasi pada matriks:\n");displayBuff(buffer);printf("\n");
        }
    t = clock() - t;
    int totaltime = (int)(((double)(t) / CLOCKS_PER_SEC)*1000);
    printf("%d ms \n", totaltime);
    saveText( buffer,totaltime, totalscore, matrixs);
  }

  else{
     clock_t t;
  int matrixWidth, matrixHeight;
  Matrix matrixs;
  int numSequences;
  int totaltoken;
  int buffers;
  int sequenceSize;
  
  int totalscore =0;
  Buffer buffer;

    scanf("%d",&totaltoken);
    int token[totaltoken];
    for (int i =0;i<totaltoken;i++)
    { 
      scanf("%X",&token[i]);
    }
    scanf("%d",&buffers);
    scanf("%d %d",&matrixWidth,&matrixHeight);
    scanf("%d",&numSequences);
    sequence seqnums[numSequences];
    scanf("%d",&sequenceSize);
    buildMatrix(&matrixs,matrixHeight,matrixWidth,token,totaltoken);
    displayMatrix(matrixs,matrixHeight,matrixWidth);
    for (int i =0;i<numSequences;i++)
    {
      
      buildSequence(&seqnums[i],sequenceSize,token,totaltoken);
    }

    for (int i=0;i<numSequences;i++)
    { 
      printf("Sequences %d:\n", i + 1);
      for (int j =0;j<(seqnums[i].lengs);j++)
      {
        printf("%X ",seqnums[i].seqs[j]);
      }
      printf("\nReward: %d\n",seqnums[i].rewards);
    }
    qsort(seqnums, numSequences, sizeof(sequence), compareRewards);
    buffer.lengths = 0;
    buffer.buffSize = buffers;
    t = clock();   // Temukan sekuens optimal
    solvePuzzle(matrixs,seqnums,numSequences,&buffer, &totalscore);

      // Cetak hasil optimal
    printf("\n\nSkor optimal: %d\n", totalscore);
          if (totalscore == 0) {
              printf("Tidak ada solusi\n\n");
          } else {
              printf("Buffer: "); displayBuffMat(buffer,matrixs);printf("\n");
              printf("Lokasi pada matriks:\n");displayBuff(buffer);printf("\n");
        }
    t = clock() - t;
    int totaltime = (int)(((double)(t) / CLOCKS_PER_SEC)*1000);
    printf("%d ms \n", totaltime);
    saveText( buffer,totaltime, totalscore, matrixs);
  }


  
    return 0;
}