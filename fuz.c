#include<stdio.h>
#include<stdlib.h>

#define MAXVAL 3
typedef enum {N,Z,P} levels_t;
const char *levels_string[]={"N","Z","P"};

void printLine(int length) {
  int i;
  for(i=0;i<length;i++)
    printf("-");
  printf("\n");
}

void printRuleBase(levels_t ruleBase[3][3]) {
  int i,j,max;
  printf("\nThe following is the rule base\n\n");
  printf("%4s   "," ");
  for(i=0;i<3;i++) {
    printf("%2s ",levels_string[i]);
    if(i!=2)
      printf("| ");
  }
  printf("\n");
  printLine(22);
  for(i=0;i<3;i++) {
    printf("%4s | ",levels_string[i]);
    for(j=0;j<3;j++) {
      printf("%2s ",levels_string[ruleBase[i][j]]);
      if(j!=2)
	printf("| ");
    }
    printf("\n");
    printLine(22);
  }
    printf("\n\n");
}
void printMembershipTable(float membershipVal[3][2*MAXVAL+1]) {
  int max,i,j;
  printf("The membership values associated with different levels are \n\n");
  max=2*MAXVAL+1;
  printf("%4s   "," ");
  for(i=0;i<max;i++) {
    printf("%3d   ",i-MAXVAL);
    if(i!=max-1)
      printf("| ");
  }
  printf("\n");
  printLine(65);
  for(i=0;i<3;i++) {
    printf("%4s | ",levels_string[i]);
    for(j=0;j<max;j++) {
      printf("%5.2f ",membershipVal[i][j]);
      if(j!=max-1)
	printf("| ");
    }
    printf("\n");
    printLine(65);
  }
  printf("\n");
}

void printDuTable(float duTable[9][2*MAXVAL+1]) {
  int max,i,j;
  printf("The table containing du values from various rules is  \n\n");
  max=2*MAXVAL+1;
  printf("%6s   "," ");
  for(i=0;i<max;i++) {
    printf("%3d   ",i-MAXVAL);
    if(i!=max-1)
      printf("| ");
  }
  printf("\n");
  printLine(65);

  for(i=0;i<9;i++) {
    printf("Rule %d | ",i+1);
    for(j=0;j<max;j++) {
      printf("%5.2f ",duTable[i][j]);
      if(j!=max-1)
	printf("| ");
    }
    printf("\n");
    printLine(65);
  }
  printf("\n");
}

float max(float a, float b) {
  if(a>b)
    return a;
  return b;
}

float min(float a, float b) {
  if(a<b)
    return a;
  return b;
}

void calcResultMem(float duTable[9][2*MAXVAL+1],float resultMem[2*MAXVAL+1]) {
  int i,j;
  float maxMem;
  int maxv=2*MAXVAL+1;
  for(i=0;i<maxv;i++) {
    maxMem=0;
    for(j=0;j<9;j++) {
      maxMem=max(maxMem,duTable[j][i]);
    }
    resultMem[i]=maxMem;
  }
}


void printChangeInput(float changeInput[2*MAXVAL+1][2*MAXVAL+1]) {
  int maxv=2*MAXVAL+1;
  int i,j;
  printf("The following should the change in input for different values of e and ce\n\n");

  printf("e\\ce | ");
  for(i=0;i<maxv;i++) {
    printf("%5d     ",i-MAXVAL);
    if(i!=maxv-1)
      printf("| ");
  }
  printf("\n");
  printLine(90);
  for(i=0;i<maxv;i++) {
    printf("%4d | ",i-MAXVAL);
    for(j=0;j<maxv;j++) {
      printf("%9f ",changeInput[i][j]);
      if(j!=maxv-1)
	printf("| ");
    }
    printf("\n");
    printLine(90);
  }
  printf("\n");
}


int main() {
  int i,j,k,p,q;
  float minMem,du,totalMem;
  float membershipVal[3][2*MAXVAL+1] = {1,0.7,0.3,0,0,0,0,0,0.3,0.7,1,0.7,0.3,0,0,0,0,0,0.3,0.7,1};
  float duMat[9][2*MAXVAL+1]={0};
  levels_t ruleBase[3][3]={N,N,Z,N,Z,P,Z,P,P};
  float resultMemVal[2*MAXVAL+1],changeInput[2*MAXVAL+1][2*MAXVAL+1];

  //Set up the initial display
  printf("\nProgram to calculate change in input based on error and change in error\n");
  printLine(79);
  printMembershipTable(membershipVal);
  printRuleBase(ruleBase);
  int e, ce;

  for(p=0;p<2*MAXVAL+1;p++) {
    for(q=0;q<2*MAXVAL+1;q++) {
      e=p-MAXVAL;
      ce=q-MAXVAL;
      for(i=0;i<3;i++) {
	for(j=0;j<3;j++) {
	  //      printf("i = %d and j = %d => Rule = %s\n",i,j,levels_string[ruleBase[i][j]]);
	  minMem=min(membershipVal[i][e+MAXVAL], membershipVal[j][ce+MAXVAL]);
	  for(k=0;k<2*MAXVAL+1;k++) {
	    duMat[(i*3)+j][k]=min(minMem,membershipVal[ruleBase[i][j]][k]);
	  }
	}
      }
  
      //printDuTable(duMat);
      calcResultMem(duMat,resultMemVal);
      /*printf("The result vector is \n");
      for(i=0;i<2*MAXVAL+1;i++) {
	printf("%f ",resultMemVal[i]);
      }
      printf("\n");*/

      du=0,totalMem=0;
      for(i=0;i<2*MAXVAL+1;i++) {
	du=du+(i-MAXVAL)*resultMemVal[i];
	totalMem+=resultMemVal[i];
      }
      changeInput[p][q]=du/totalMem;
    }
  }
  printChangeInput(changeInput);
  return 0;
}
