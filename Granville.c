#include <stdlib.h> 
#include <stdio.h>
 
#define MAX_SIZE_SSET 1000000
 
int main(int argc, char* argv[]) { 
  int Sset[MAX_SIZE_SSET] ;
 
  int Ssetsize = 1; 
  Sset[0] = 1 ;
 
  for(int n = 2; n < MAX_SIZE_SSET; n++) { 
    int dsum = 0 ;
 
    for(int i = 0; i < Ssetsize; i++) { 
      if( n % Sset[i] == 0 && Sset[i] < n) dsum += Sset[i] ; 
      if (dsum > n || Sset[i] >= n) break ; 
    }
 
    if(dsum <= n) { 
      if(dsum == n) printf("%d\n", n) ; 
      Sset[Ssetsize++ ] = n ; 
    } 
  } 
}