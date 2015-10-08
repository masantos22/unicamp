#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 30
#define MAXK 30

int backTracking(int n, int S, int s[], double v[], int c[], int sol[]){
	int vWith, vWithout;

	if(n == -1)
		return 0;
	if(S < 0)
		return -1;
	n--;

	vWith = backTracking(n, (S - s[n]), s, v, c, sol);
	vWithout = backTracking(n, S, s, v, c, sol);

	if(vWith != -1)
		sol[n] = 1;
		return v[n] + vWith;

	sol[n] = 0;
	return vWithout; 
}

void resolve(int n, int K, int S, int s[], double v[], int c[], int sol[]) {
	backTracking(n,S,s,v,c,sol);
}

int main(int argc, char *argv[]) {

  int n;         /* numero de itens    */
  int K;         /* max num de classes */
  int S;         /* max tam da solucao */
  int s[MAXN];   /* tamanhos dos itens */
  double v[MAXN];/* valores dos itens  */
  int c[MAXN];   /* classes dos itens  */

  int sol[MAXN]; /* solucao obtida     */

  int i;          // contador
  double vsol;    // valor da solucao
  int ssol;       // tamanho da solucao
  int csol[MAXK]; // bitset das classes usadas na solucao
  int ncsol;      // num classes usadas na solucao

  if (argc >= 2) {
    freopen(argv[1], "r", stdin);
  }

  scanf("%d%d%d", &n, &K, &S);
  for(i=0; i<n; i++) {
    scanf("%d%lf%d", &s[i], &v[i], &c[i]);
  }

  resolve(n, K, S, s, v, c, sol);

  vsol = 0;
  ssol = 0;
  ncsol = 0;
  memset(csol, 0, sizeof(csol));

  printf("Solucao obtida:");
  for(i=0; i<n; i++) {
    printf(" %d", sol[i]);
    if (sol[i]) {
      vsol += v[i];
      ssol += s[i];
      if (csol[c[i]]++ == 0) {
        ncsol ++;
      }
    }
  }
  printf("\n");

  printf("Valor da solucao:\t\t%lf\n", vsol);
  printf("Tamanho da solucao:\t\t%d%s\n", ssol, (ssol>S)?"\t(inviavel)":"");
  printf("Numero de classes:\t\t%d%s\n", ncsol, (ncsol>K)?"\t(inviavel)":"");

  return 0;
}
