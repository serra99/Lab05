#include <stdio.h>
#include <stdlib.h>
#define FILE_LOG "mat.txt"
#define BLACK 0
void free2d(int **m,int nr);

int **malloc2dR(FILE *fp, int nr, int nc);
void malloc2dP(FILE *fp, int ***m, int nr, int nc);
void separa(int **mat, int nr, int nc,int **black, int **white,int *dim_w, int *dim_b);
void print_v(int *v, int n);
void print_m(int **m, int nr, int nc);
int main()
{
    int **m1,**m2;
    FILE *f_in;
    int nr,nc;
    int dim_w, dim_b;
    int *black = NULL, *white = NULL;
    f_in = fopen(FILE_LOG,"r");
    if(f_in == NULL){
        printf("Errore nell'apertura del file %s\n",FILE_LOG);
        return -1;
    }
    printf("OK\n");

    fscanf(f_in,"%d %d",&nr,&nc);
    m1 = malloc2dR(f_in,nr,nc);
    printf("MATRICE 1\n");
    print_m(m1,nr,nc);

    fclose(f_in);
    f_in = fopen(FILE_LOG,"r");
    fscanf(f_in,"%d %d", &nr,&nc);

    malloc2dP(f_in,&m2,nr,nc);
    printf("MATRICE 2\n");
    print_m(m2,nr,nc);

    separa(m1,nr,nc,&black,&white,&dim_w,&dim_b);
    print_v(black,dim_b);
    print_v(white,dim_w);
    printf("FREE()...\n");
    free(white);
    free(black);
    free2d(m1,nr);
    free2d(m2,nr);
    printf("OK\n");
    return 1;
}
void print_v(int *v, int n){
    for(int i = 0; i < n; i++)
        printf("%d ",v[i]);
    printf("\n");

    return;
}
void free2d(int **m, int nr){
    int i;
    for(i = 0; i < nr; i++)
        free(m[i]);
    free(m);
}
void print_m(int **m, int nr, int nc){
    for(int i = 0; i < nr; i++)
        print_v(m[i],nc);
    return;
}
void malloc2dP(FILE *fp, int ***mp, int nr, int nc){
    int **m;
    int i;
    m = malloc(nr * sizeof(int *));
    for( i = 0; i < nr; i++){
        m[i] = malloc(nc * sizeof(int));
        for(int j = 0; j < nc; j++)
            fscanf(fp,"%d",&m[i][j]);
    }
    *mp = m;

}

int **malloc2dR(FILE *fp, int nr,int nc){
    int **m;
    int i,j;
    m = malloc(nr * sizeof(int*));
    for(i = 0; i < nr; i++){
        m[i] = malloc(nc * sizeof(int));
        for(j = 0; j < nc;j++){
            fscanf(fp,"%d",&m[i][j]);
        }
    }
    return m;
}

void separa(int **mat,int nr,int nc,int **black,int **white, int *dim_w, int *dim_b){
    printf("separa()...\n");
    int i, j;
    int count = 0;
    int c_white = 0, c_black = 0;
    *dim_w = 1;
    *dim_b = 1;
    int MAX_W = 1, MAX_B = 1;
    int *vw = malloc(sizeof(int) * (MAX_W));
    int *vb = malloc(sizeof(int)* (MAX_B));

    for(i = 0; i < nr; i++){
        for(j = 0; j < nc; j++){
            if(count % 2 == BLACK){
                if(c_black == MAX_B){
                    MAX_B =MAX_B * 2;
                    vb = realloc(vb,(MAX_B) * sizeof(int));
                }
                vb[c_black++] = mat[i][j];
                //printf("BLACK[%d] : %d\n",c_black,vb[c_black]);
            }
            else{
                if(c_white == MAX_W){
                    MAX_W *= 2;
                    vw = realloc(vw,(MAX_W) * sizeof(int));
                }
                vw[c_white++] = mat[i][j];
            }
            count++;
        }

    }
    printf("COUNT %d\n",count);
    printf("C_BLACK %d C_WHITE %d\n",c_black, c_white);
    *dim_b = c_black;
    *dim_w = c_white;
    *black = vb;
    *white = vw;
    printf("OK\n");
}
