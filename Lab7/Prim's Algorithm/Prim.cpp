
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define INF 1000000000
 
int g[6][6] = { {0,6,5,0,3,0},
				{6,0,1,5,0,0},
				{5,1,0,5,6,4},
				{0,5,5,0,0,2},
				{3,0,6,0,0,6},
				{0,0,4,2,6,0},
			  };
 //LAB7

int dist[6];
int marc[6];
int dest[6];
int orig[6];

int N = 6;

int prim();
int min(int a, int b);
int parent[6];
int ord[6];
int main(){
	printf("\nCusto total: %d\n\n", prim());
	for (int i = 1; i < 6; i++)
      printf("%d - %d    %d \n", parent[i]+1, i+1, g[parent[i]][i]);
	return 0;
}
int prim() {
    // Retorna o custo de uma MST do grafo representado por g.
    for (int i = 0; i < N; i++) dist[i] = INF;
    dist[0] = 0;
   
    int curr = 0;
    int custo = 0;

    while (curr != -1) {
        marc[curr]=1;

        custo += dist[curr];

        for (int i = 0; i < N; i++) {
            if (g[curr][i]) {
            	dist[i] = min(dist[i],g[curr][i]);
			}
        }

       	curr = -1;
        int min_dist = INF;
        for (int i = 0; i < N; i++) {
            if (!marc[i] && dist[i] < min_dist) {
                curr = i;
                min_dist = dist[i];
            }
        }

                 
    }
    return custo;
}

int min(int a, int b){
	if (a<b)	return a;
	else return b;
}
