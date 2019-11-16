#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SONG    255
#define FILE_BRANI "brani.txt"
/*Utilizzo il principio di moltiplicazione in modo da poter esplorare tutte
le scelte.
n_scelte = produttoria della cardinalità di ogni insieme (num_di_canzoni)
*/
typedef struct {
    char **titoli;
    int amico;
}Canzone;
typedef struct {
    int *scelte;
    int num_scelte;
}Livello;
void stampa_canzoni(Canzone *canzoni, Livello *val, int n);
int princ_molt(int pos , Livello *val, int *sol, int n, int count,Canzone *c);
int elenca_playlist(int pos, Livello *val, int *sol, int n, int count);
int load_song(Canzone *canzoni,Livello **val, char *filename);
int carica_struct(char *filename,Canzone **c,Livello** l);
int main()
{

    Livello *val;
    int * sol, i,j,n,count;
    Canzone *canzoni;

    n = carica_struct(FILE_BRANI,&canzoni,&val);
    if(n == -1){
        printf("Errore nell'apertura del file %s\n",FILE_BRANI);
    }
    stampa_canzoni(canzoni,val,n);

    sol = (int *)malloc(n * sizeof(int));
    count = princ_molt(0,val,sol,n,0,canzoni);
    printf("COMBINAZIONI : %d\n",count);

    return 1;
}
void stampa_canzoni(Canzone *canzoni, Livello *val, int n){
    int i,j;
    for( i = 0; i < n; i++){
        printf("AMICO %d\n",canzoni[i].amico);
        for( j = 0; j < val[i].num_scelte; j++){
            printf("\t%d - SCELTE %d -%s\n",j,val[i].scelte[j],canzoni[i].titoli[j]);
        }
        printf("\n");
    }
}
int princ_molt(int pos , Livello *val, int *sol, int n, int count,Canzone *c){
    int i;

    if(pos >= n){
        for(i = 0; i < n; i++){

            printf("%s - ",c[i].titoli[sol[i]]);
        }
        printf("\n");
        return count+1;
    }
    for(i = 0; i < val[pos].num_scelte; i++){
        sol[pos] = val[pos].scelte[i];
        count = princ_molt(pos+1,val, sol,n, count,c);
    }
    return count;
}
int carica_struct(char *filename,Canzone **c,Livello** val){
    int n,n_song;
    int i,j;
    char song[MAX_SONG];
    Canzone *canzoni;
    Livello *l;
    FILE* f_in = fopen(filename,"r");

    if(f_in == NULL) return -1;

    fscanf(f_in,"%d",&n);

    canzoni = malloc(n * sizeof(*canzoni));
    l = malloc(n * sizeof(*l));

    for(i = 0; i < n; i++){

        fscanf(f_in, "%d", &n_song);
        canzoni[i].titoli = malloc(n_song * sizeof(char*));
        l[i].num_scelte = n_song;
        canzoni[i].amico = i;
        l[i].scelte = malloc(n_song * sizeof(int));

        for(j = 0; j < n_song;j++){
            fscanf(f_in,"%s",song);
            canzoni[i].titoli[j] = strdup(song);
            l[i].scelte[j] = j;
        }
    }
    *val = l;
    *c = canzoni;
    return n;

}
