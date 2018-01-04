/*
**    $VER: cudoku.c 1.0 (29.03.06)
**
**    simple Sudoku solver, uses backtracking search
**
**    (C) Copyright 2006-2017 by Rainer Koppler
**
**==============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*------------------------------------------------------------------------------
*  constants
*/
enum {
   cMinNum   = 1,
   cMaxNum   = 9,
   cNumCnt   = cMaxNum - cMinNum + 1,
   cHistSize = cNumCnt * cNumCnt
};

static const int csPos2Sqr[cNumCnt][cNumCnt] = {
   { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
   { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
   { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
   { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
   { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
   { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
   { 6, 6, 6, 7, 7, 7, 8, 8, 8 },
   { 6, 6, 6, 7, 7, 7, 8, 8, 8 },
   { 6, 6, 6, 7, 7, 7, 8, 8, 8 }
};

/*------------------------------------------------------------------------------
*  types
*/
#define true  1
#define false 0

typedef char     bool;
typedef int      Number;
typedef unsigned BSet;

typedef struct {    /* position history */
   int   mRow;      /* position row */
   int   mCol;      /* position column */
   BSet  mAlts;     /* alternatives */
   int   mAltIdx;   /* current alternative */
   int   mAltCnt;   /* total number of alternatives */
} History;

/*------------------------------------------------------------------------------
*  variables
*/
static BSet sRows[cNumCnt];   /* available numbers for each row */
static BSet sCols[cNumCnt];   /* available numbers for each column */
static BSet sSqrs[cNumCnt];   /* available numbers for each square */

static Number sMatrix[cNumCnt][cNumCnt];
static bool   sPreset[cNumCnt][cNumCnt];

static History sHistory[cHistSize];

static int sPresetCnt;
static int sTotalCnt;
static int sCnt;

/*------------------------------------------------------------------------------
*  macros & functions
*/
#define BSetAdd(bset, num)      ((bset) |= (1 << (num)))
#define BSetRemove(bset, num)   ((bset) &= ~(1 << (num)))
#define BSetIsEmpty(bset)       ((bset) == 0)

/******************************************************************************/
static int BSetGetLength(BSet bset) {

   int n = 0;
   while (bset != 0) {
      if ((bset & 1) != 0) ++n;
      bset >>= 1;
   }
   return n;
}

/******************************************************************************/
static int BSetGetNth(BSet bset, int idx) {

   int i;
   for (i=0; i<cNumCnt && idx>=0; ++i) {
      if ((bset & 1) != 0 && --idx < 0) return i;
      bset >>= 1;
   }
   return -1;
}

/******************************************************************************/
static bool GetFreePos(int *pRow, int *pCol) {

   static bool sFirst = true;

   if (sFirst) {
      sFirst = false;
      *pRow = 0;
      *pCol = 0;
      if (!sPreset[*pRow][*pCol]) return true;
   }

   do {
      ++*pCol;
      if (*pCol == cNumCnt) {
         *pCol = 0;
         ++*pRow;
         assert(*pRow != cNumCnt);
      }
   } while (sPreset[*pRow][*pCol]);

   return true;
}

/******************************************************************************/
static void ExitUnsolved(void) {

   puts("puzzle cannot be solved !");
   exit(EXIT_FAILURE);
}

/*------------------------------------------------------------------------------
*  main
*/
int main(int argc, char *argv[]) {

   FILE    *pFile;
   int      i, j, v, row, col, sqr, altCnt, backtrackCnt = 0;
   BSet     alts;
   bool     ok;
   History *p;

   if (argc < 2) {
      printf("Usage: %s <matrix file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   puts("### Brute-force Sudoku Solver ###\nCopyright 2006-2017 by Rainer Koppler\n");

   /*---------------------------------------------------------------------------
   *  read incomplete matrix
   */
   pFile = fopen(argv[1], "r");
   if (pFile == 0) {
      printf("Error: could not read \"%s\"\n", argv[1]);
      exit(EXIT_FAILURE);
   }

   for (i=0; i<cMaxNum; ++i) {

      fscanf(pFile, "%d %d %d %d %d %d %d %d %d\n",
         &sMatrix[i][0],
         &sMatrix[i][1],
         &sMatrix[i][2],
         &sMatrix[i][3],
         &sMatrix[i][4],
         &sMatrix[i][5],
         &sMatrix[i][6],
         &sMatrix[i][7],
         &sMatrix[i][8]);

      sRows[i] = sCols[i] = sSqrs[i] = (1 << cMaxNum) - 1;   /* start with ALL numbers */
      for (j=0; j<cMaxNum; ++j) {
         if (sMatrix[i][j] != 0) {
            sPreset[i][j] = true;
            BSetRemove(sRows[i], sMatrix[i][j] - 1);
            ++sPresetCnt;
         }
      }
   }

   fclose(pFile);

   for (j=0; j<cMaxNum; ++j) {
      for (i=0; i<cMaxNum; ++i) {
         v = sMatrix[i][j];
         if (v != 0) {
            BSetRemove(sCols[j], v - 1);
            BSetRemove(sSqrs[csPos2Sqr[i][j]], v - 1);
         }
      }
   }

   /*---------------------------------------------------------------------------
   *  compute matrix elements
   */
   sTotalCnt = cNumCnt * cNumCnt - sPresetCnt;
   sCnt = 0;

   while (sCnt != sTotalCnt) {

      if (!GetFreePos(&row, &col)) ExitUnsolved();
      sqr = csPos2Sqr[row][col];

      alts = sRows[row] & sCols[col] & sSqrs[sqr];
      altCnt = BSetGetLength(alts);
      i = 0;
      ok = !BSetIsEmpty(alts);

      while (!ok) {
         if (sCnt == 0) ExitUnsolved();
         ++backtrackCnt;

         --sCnt;
         p = &sHistory[sCnt];
         row = p->mRow;
         col = p->mCol;
         sqr = csPos2Sqr[row][col];

         v = sMatrix[row][col] - 1;
         sMatrix[row][col] = 0;
         BSetAdd(sRows[row], v);
         BSetAdd(sCols[col], v);
         BSetAdd(sSqrs[sqr], v);

         alts   = p->mAlts;
         altCnt = p->mAltCnt;
         i      = p->mAltIdx;
         ++i;
         ok = i < p->mAltCnt;
      }

      v = BSetGetNth(alts, i);
      sMatrix[row][col] = v + 1;
      BSetRemove(sRows[row], v);
      BSetRemove(sCols[col], v);
      BSetRemove(sSqrs[sqr], v);

      p = &sHistory[sCnt];
      p->mRow    = row;
      p->mCol    = col;
      p->mAlts   = alts;
      p->mAltCnt = altCnt;
      p->mAltIdx = i;
      ++sCnt;
   }

   /*---------------------------------------------------------------------------
   *  print complete matrix
   */
   for (i=0; i<cMaxNum; ++i) {
      printf("%d %d %d %d %d %d %d %d %d\n",
         sMatrix[i][0],
         sMatrix[i][1],
         sMatrix[i][2],
         sMatrix[i][3],
         sMatrix[i][4],
         sMatrix[i][5],
         sMatrix[i][6],
         sMatrix[i][7],
         sMatrix[i][8]);
   }

   exit(EXIT_SUCCESS);
}
