#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_LOG "corse.txt"
#define PARTENZA 1
#define MAX_RIGHE 1000
#define MAX_STRINGA 30
#define NUM_CAMPI 7
typedef enum {
    c_stampa,c_data,c_tratta, c_partenza, c_arrivo, c_ricerca,c_alloca, c_fine
}comando_e;
typedef enum {
    o_data,o_tratta,o_partenza, o_arrivo
}ordinamento;
typedef struct{
    char *codice_tratta;
    char *partenza;
    char *destinazione;
    char *data;
    char *ora_partenza;
    char *ora_arrivo;
    int ritardo;
}t_tratta;
void print_tratta(t_tratta t);
int binSearch(t_tratta **p_t, char *key,int l,int r);
comando_e leggiComando();
int ricercaLineare(t_tratta t[], char *key,int n);
int carica_struct(t_tratta t[]);
void menu(t_tratta t[],int n);
void ordinaCodice(t_tratta **pOrd, int n);
void ordinaStazPartenza(t_tratta** pOrd, int n);
void ordinaStazArrivo(t_tratta** pOrd, int n);
void ordinaData(t_tratta** pOrd, int n);
int dataCmp(char* data1, char* data2,char *ora1,char* ora2);
void strToLower(char s[MAX_STRINGA]);
void print_table(t_tratta t[MAX_RIGHE],int N);
void printByRef(t_tratta **pTratta, int n);
void ricercaDicotomica(t_tratta **p_t, char *key,int n);
int carica_da_file(t_tratta **p_t, char* filename);
void free_tratte(t_tratta *p_t, int n);
int main()
{
    int n;
    t_tratta *tratte;


    n = carica_da_file(&tratte,"corse1.txt");
    //n = carica_struct(t);
    if (n == -1){
        printf("Errore apertura del file %s\n",FILE_LOG);
        return -1;
    }
    //printByRef(tratte,n);
    print_table(tratte,n);
    /*ordinaCodice(pOrdTratta,n);
    printByRef(pOrdTratta,n);
    ordinaStazPartenza(pOrdStazPart,n);
    printByRef(pOrdStazPart,n);
    ordinaStazArrivo(pOrdStazArr,n);
    printByRef(pOrdStazArr,n);
    ordinaData(pOrdData,n);
    printByRef(pOrdData,n);
    */
    menu(tratte,n);
    return 0;
}
int carica_da_file(t_tratta **p_t, char* filename){
    char partenza[MAX_STRINGA], data[MAX_STRINGA], codice[MAX_STRINGA], arrivo[MAX_STRINGA], oraP[MAX_STRINGA],oraA[MAX_STRINGA];

    int n,i;
    t_tratta *tratte;
    FILE* f_in;
    int ritardo;
    f_in = fopen(filename,"r");
    if(f_in == NULL) return -1;
    printf("File aperto\n");
    fscanf(f_in, "%d" , &n);

    tratte = (t_tratta*) malloc(n * sizeof(t_tratta));

    for(i = 0; i < n; i++){

        fscanf(f_in,"%s %s %s %s %s %s %d",codice,partenza,arrivo,data,oraP,oraA,&ritardo);
        tratte[i].codice_tratta = strdup(codice);
        tratte[i].partenza = strdup(partenza);
        tratte[i].destinazione = strdup(arrivo);
        tratte[i].data = strdup(data);
        tratte[i].ora_partenza = strdup(oraP);
        tratte[i].ora_arrivo = strdup(oraA);
        tratte[i].ritardo = ritardo;
    }

    *p_t = tratte;
    return n;

}
void free_tratte(t_tratta *p_t, int n){
    int i;
    for(i = 0; i < n; i++){
        free(p_t[i].codice_tratta);
        free(p_t[i].partenza);
        free(p_t[i].destinazione);
        free(p_t[i].ora_partenza);
        free(p_t[i].ora_arrivo);
        free(p_t[i].data);
    }
    free(p_t);
}


void printByRef(t_tratta **pTratta,int n){
    for(int i = 0; i < n; i++)
        print_tratta(*pTratta[i]);
}
void ordinaCodice(t_tratta** pOrd, int n){
    int i,j;
    t_tratta* temp;
    printf("Ordine per codice...\n");
    for(i = 1; i < n; i++){
        temp = pOrd[i];
        j = i - 1;
        while(j >= 0 && strcmp(temp->codice_tratta,pOrd[j]->codice_tratta) < 0){
            pOrd[j+1] = pOrd[j];
            j--;
        }
        pOrd[j+1] = temp;
    }
    printf("OK\n");
    return;
}
void ordinaStazPartenza(t_tratta** pOrd, int n){
    int i,j;
    t_tratta* temp;
    printf("Ordine per stazione di partenza...\n");
    for(i = 1; i < n; i++){
        temp = pOrd[i];
        j = i - 1;
        while(j >= 0 && strcmp(temp->partenza,pOrd[j]->partenza) < 0){
            pOrd[j+1] = pOrd[j];
            j--;
        }
        pOrd[j+1] = temp;
    }
    printf("OK\n");
    return;
}
int dataCmp(char* data1, char* data2,char *ora1,char* ora2){
    //printf("Confronto : %s %s \n",data1, data2);
    if(strcmp(data1,data2) > 0){
        //printf("%s > %s\n",data1,data2);
        return 0;
    }if(strcmp(data1,data2) < 0){
        //printf("%s < %s\n",data1,data2);
        return 1;
    }
    //printf("%s = %s\n",data1,data2);
    if(strcmp(ora1,ora2) <= 0)
        return 1;
    return 1;

}
void ordinaStazArrivo(t_tratta** pOrd, int n){
    int i,j;
    t_tratta* temp;
    printf("Ordine per stazione di arrivo...\n");
    for(i = 1; i < n; i++){
        temp = pOrd[i];
        j = i - 1;
        while(j >= 0 && strcmp(temp->destinazione,pOrd[j]->destinazione) < 0){
            pOrd[j+1] = pOrd[j];
            j--;
        }
        pOrd[j+1] = temp;
    }
    printf("OK\n");
    return;
}
void ordinaData(t_tratta** pOrd, int n){
    int i,j;
    t_tratta* temp;
    printf("Ordine per data...\n");
    for(i = 1; i < n; i++){
        temp = pOrd[i];
        j = i - 1;
        while(j >= 0 &&
              (dataCmp(temp->data,pOrd[j]->data,temp->ora_partenza,pOrd[j]->ora_partenza) == 0)){
            pOrd[j+1] = pOrd[j];
            j--;
        }
        pOrd[j+1] = temp;
    }
    printf("OK\n");
    return;
}
void menu(t_tratta t[],int n){
    int ordinato = -1;
    int pos;
    t_tratta **pOrdData, **pOrdTratta, **pOrdStazPart,
        **pOrdStazArr;
    pOrdData =(t_tratta**) malloc(n * sizeof(t_tratta*));
    pOrdTratta = malloc(n * sizeof(t_tratta*));
    pOrdStazPart = malloc(n * sizeof(t_tratta*));
    pOrdStazArr = malloc(n * sizeof(t_tratta*));
    int i;
    char key[MAX_STRINGA],filename[MAX_STRINGA];
    for(i = 0; i < n; i++)
        pOrdData[i] = pOrdTratta[i] = pOrdStazPart[i] = pOrdStazArr[i] = &t[i];
    //printf("CODICE: %s\n",t[2].codice_tratta);
    comando_e codiceComando;
    //printf("MALLOC OK \n");
    int continua = 1;

    while(continua){
        codiceComando = leggiComando();
        //printf("%d",codiceComando);
        switch (codiceComando) {
        case c_stampa :

        break;
        case c_data:
            //printf("CASE DATA\n");
            ordinaData(pOrdData,n);
            printByRef(pOrdData,n);
            ordinato = o_data;
        break;
        case c_tratta:
            ordinaCodice(pOrdTratta,n);
            printByRef(pOrdTratta,n);
            ordinato = o_tratta;
        break;

        case c_partenza:
            ordinaStazPartenza(pOrdStazPart,n);
            printByRef(pOrdStazPart,n);
            ordinato = o_partenza;
        break;
        case  c_arrivo:
            ordinaStazArrivo(pOrdStazArr,n);
            printByRef(pOrdStazArr,n);
            ordinato = o_arrivo;
        break;
        case c_ricerca:
            printf("Inserisci la stazione di partenza (o parte di essa)\n");
            scanf("%s",key);

            if(ordinato == o_partenza){
                printf("ORDINATO per PARTENZA\n");
                ricercaDicotomica(pOrdStazPart,key,n);
            }
            else{
                pos = ricercaLineare(t,key,n);
                printf("Sono stati trovati %d elementi che corrispondono alla ricerca\n",pos);
            }
        break;
        case c_alloca:
            do{
                printf("Inserisci il nome del file: \n");
                scanf("%s",filename);
                free_tratte(t,n);
                n = carica_da_file(&t,filename);
            }while(n == -1);

        break;
        case c_fine:
            continua = 0;
        break;
        default:
            printf("Errore nell'inserimento del comando\n");
        break;
        }
    }
}
int ricercaLineare(t_tratta t[], char *key,int n){
    int trovato = 0;
    for(int i = 0; i < n; i++)
        if(strncmp(key,t[i].partenza,strlen(key)) == 0){
            print_tratta(t[i]);
            trovato++;
        }
    return trovato;

}
void ricercaDicotomica(t_tratta **p_t, char *key,int n){
    int m = 0;
    int i,len;
    m = binSearch(p_t, key,0,n);
    if(m < 0){
        printf("Nessun risultato trovato\n");
        return;
    }
    printf("TABLE...\n");
    len = strlen(key);
    for(i = m; i < n;i++){
        if(strncmp(p_t[i]->partenza,p_t[m]->partenza,len) == 0)
            print_tratta(*p_t[i]);
        else{
            break;
        }


    }
    for(i = m + 1; i > 0;i--){
        if(strncmp(p_t[m]->partenza,key,len) == 0)
            print_tratta(*p_t[i]);
        else
            break;

    }
    return;
}
int binSearch(t_tratta **p_t, char *key,int l,int r){

    int m;
    int len = strlen(key);
    while(l <= r){
        m = (l + r) / 2;
        if(strncmp(p_t[m]->partenza,key,len) == 0)
            return m;
        if(strncmp(p_t[m]->partenza,key,len) < 0)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}
void print_tratta(t_tratta t){
    printf("%s\t%s\t%s\t%s\t%s\t%s\t%d\n",
           t.codice_tratta,t.partenza,t.destinazione,t.data,t.ora_partenza,t.ora_arrivo,t.ritardo);
}
int carica_struct(t_tratta t[]){
    FILE *f_log = fopen(FILE_LOG, "r");
    if(f_log == NULL) return -1;
    int n, i;
    fscanf(f_log, "%d" , &n);
    for(i = 0; i < n; i++)
        fscanf(f_log, "%s %s %s %s %s %s %d",
               t[i].codice_tratta, t[i].partenza, t[i].destinazione, t[i].data, t[i].ora_partenza, t[i].ora_arrivo, &t[i].ritardo);

    return n;
}
void print_table(t_tratta t[MAX_RIGHE],int N){
    int i;
    for(i = 0; i < N; i++)
        print_tratta(t[i]);
}
void strToLower(char s[MAX_STRINGA]){
    int i;
    for(i = 0; i < strlen(s); i++){
        s[i] = tolower(s[i]);
    }
}
comando_e leggiComando(){
    comando_e c;
    char cmd[MAX_STRINGA];
    char tabella[c_fine+1][MAX_STRINGA] = {
        "stampa", "data","tratta","partenza", "arrivo","ricerca","alloca","fine"
    };
    printf("comando (data - tratta - partenza - arrivo - ricerca - alloca - fine )\n");
    scanf("%s",cmd);
    strToLower(cmd);
    //printf("comando: %s",cmd);
    c = 0;
    while (c < c_fine && strcmp(cmd,tabella[c]) != 0)
        c++;

    return c;
}
