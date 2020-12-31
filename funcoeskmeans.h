#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#define LINHA 768
//#define COLUNA 9
//#define LINHA 150
//#define COLUNA 4
#include <stdbool.h>
#include "lista.h"


int LINHA=768;
int COLUNA=9;

float **alocamatriz(){


      float **matriz;  /* ponteiro para a matriz */
      int  i;    /* variavel auxiliar      */

      /* aloca as linhas da matriz */
      matriz = (float **) malloc (LINHA * sizeof(float *));
      if (matriz == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
      }

      /* aloca as colunas da matriz */
      for ( i = 0; i < LINHA; i++ ) {
          matriz[i] = (float*) malloc (COLUNA * sizeof(float));	/* m vetores de n floats */
          if (matriz[i] == NULL) {
             printf ("** Erro: Memoria Insuficiente **");
             return (NULL);
          }

       }



      return (matriz); /* retorna o ponteiro para a matriz */

}



int **alocaClusters(){

        int **ptr = (int **) malloc(LINHA * sizeof(int *));

        return ptr;
}


int menu(){

        int op;
        printf("\033[1;31m----------K—MEANS----------\n \033[0m\n");
        printf("[1]-Carregar banco de dados\n");
        printf("[2]-Aplicar a clusterizacao\n");
        printf("[3]-Sair\n");
        printf("Opção:");
        scanf("%d",&op);

        return op;
}

float **carregaBancoDados(FILE *data,int op_base){

        int i,j,k;
        float **matrizaux;

        if(op_base==1){

            data = fopen("base_diabetes.txt","r");

        }else if(op_base==2){

            data = fopen("base_iris.txt","r");
            LINHA=150;
            COLUNA=5;

        }

        matrizaux=alocamatriz();

        for(i=0;i<LINHA;i++){

            for(j=0;j<COLUNA;j++){

                fscanf(data,"%f",&matrizaux[i][j]);

            }

        }


        return (matrizaux);
}


void MinMax(float **matriz,int qtd,int op){


        float min=0.0;
        float max=0.0;
        float vetormimax[16];
        int col=0,j=0,i,valor;

        for(col=0;col<COLUNA;col++){
            i=0;
            min=matriz[i][col];
            max=matriz[i][col];

            for(i=0;i<LINHA;i++){

                    if(matriz[i][col] < min)
                            min=matriz[i][col];

                    if(matriz[i][col] > max)
                            max=matriz[i][col];

            }

            vetormimax[j]=min;j++;
            vetormimax[j]=max;j++;

        }


        normalizaMatriz(vetormimax,matriz,qtd,op);


}
float calculaMinMax(float MinMax[] ,float valoratual,int atributo){
        //

        float menor;
        float maior;
        float numerador;

        switch(atributo){

                case 0:

                menor=MinMax[0];
                maior=MinMax[1];

                break;

                case 1:

                menor=MinMax[2];
                maior=MinMax[3];

                break;

                case 2:

                menor=MinMax[4];
                maior=MinMax[5];

                break;

                case 3:

                menor=MinMax[6];
                maior=MinMax[7];

                break;

                case 4:

                menor=MinMax[8];
                maior=MinMax[9];

                break;

                case 5:

                menor=MinMax[10];
                maior=MinMax[11];

                break;

                case 6:

                menor=MinMax[12];
                maior=MinMax[13];

                break;

                case 7:

                menor=MinMax[14];
                maior=MinMax[15];

                break;


        }

        float denominador=(maior - menor);
        numerador=(valoratual -menor);
        float result=numerador/denominador;

        if(denominador==0)
            result=0;


        return result;

}

float calculaDistanciaEuclidiana(int posponto,int poscluster,float **matriznormalizada){

        float soma=0;


        for(int i=0;i<COLUNA;i++){

                soma+=pow(matriznormalizada[posponto][i]-matriznormalizada[poscluster][i],2);

        }

        return sqrt(soma);

}

void normalizaMatriz(float vetminmax[],float **matriz,int qtd,int op){

        int atributo;
        float **matriznormalizada;
        matriznormalizada=alocamatriz();

        if(COLUNA==9)
               COLUNA=COLUNA-1;

        if(COLUNA==5)
                COLUNA=COLUNA-1;

        for(int i=0;i<LINHA;i++){
            atributo=0;

            for(int j=0;j<COLUNA;j++){


                    matriznormalizada[i][j]=calculaMinMax(vetminmax,matriz[i][j],atributo);
                    atributo++;


            }

        }



       clusterizacao(matriznormalizada,qtd,matriz,op);
       // printf("Distancia:%lf\n",calculaDistanciaEuclidiana(1,0,matriznormalizada));

}

float menorDistancia(int i,int num_clusters,float **matriznormalizada){

        float dist,cluster;
        Lista *li=cria();

        for(int c=0;c<num_clusters;c++){

            dist=calculaDistanciaEuclidiana(i,c,matriznormalizada);
            inserefim(li,dist,c);

        }

        cluster=encontraCluster(li);
        deletaLista(li);

        return cluster;

}



void atualizaCentroide(float **matriznormalizada,int **clusters,int num_clusters){

        float media,media2;
        int tam=0,tam2=0;
        int pos=0;


            for(int j=0;j<COLUNA;j++){

                for(int k=0;k<num_clusters;k++){
                    media=0;
                    tam=0;
                    for(int c=num_clusters;c<LINHA;c++){

                        if(clusters[c]==k){
                            media+=matriznormalizada[c][j];
                            tam++;
                        }

                    }

                    media=media/tam;
                    matriznormalizada[k][j]=media;

                }


              }


}

float calculaSSE(float **matriznormalizada,int num_cluster,int tamanho,int **clusters){


            float soma=0,somaerros=0;

            for(int k=0;k<num_cluster;k++){
                soma=0;
                for(int c=num_cluster;c<tamanho;c++){//tamanho=numero de instancias


                        if(clusters[c]==k){

                            soma+=pow(calculaDistanciaEuclidiana(c,k,matriznormalizada),2);


                        }
                }

                somaerros+=soma;

            }

            return somaerros;


}

bool verificaSSE(float SSE,float SSEaux){

      float erro;

      //erro=SSE-(0.05 * SSE);


      if(SSE == SSEaux)
        return true;



      return false;
}


void salvaCluster(int clusters[],int num_cluster,float **matrinormalizada){


        FILE *arq;

        arq=fopen("cluster0.txt","w");

        for(int i=num_cluster;i<LINHA;i++){

                if(clusters[i]==3){

                 fprintf(arq,"%d ",i);

                }

        }

        fclose(arq);


}

float calculaLog(float pcomdiabetes,float psem_diabetes,int base,float tam_cluster) {



    pcomdiabetes=pcomdiabetes/tam_cluster;
    psem_diabetes=psem_diabetes/tam_cluster;

    pcomdiabetes=(pcomdiabetes * ( log(pcomdiabetes) / log(base))) * -1;
    psem_diabetes=(psem_diabetes * ( log(psem_diabetes) / log(base)));

     if(isnan(pcomdiabetes))
        pcomdiabetes=0;

      if(isnan(psem_diabetes))
        psem_diabetes=0;

    return pcomdiabetes+psem_diabetes;
}

float calculaLogIris(float Iris_setosa,float Iris_versicolor,float Iris_virginica,int base,float tam_cluster) {



    Iris_setosa=Iris_setosa/tam_cluster;
    Iris_versicolor=Iris_versicolor/tam_cluster;
    Iris_virginica=Iris_virginica/tam_cluster;




    Iris_setosa=(Iris_setosa * ( log(Iris_setosa) / log(base))) * -1;
    Iris_versicolor=(Iris_versicolor * ( log(Iris_versicolor) / log(base)));
    Iris_virginica=(Iris_virginica * ( log(Iris_virginica) / log(base)));

    if(isnan(Iris_setosa))
        Iris_setosa=0;

    if(isnan(Iris_versicolor))
        Iris_versicolor=0;

    if(isnan(Iris_virginica))
        Iris_virginica=0;


    float teste=Iris_setosa+Iris_versicolor+Iris_virginica;

    return Iris_setosa+Iris_versicolor+Iris_virginica;
}


void calculaEntropia(int **clusters,int num_clusters,float **matriz){

        float psem_diabetes=0;
        float pcom_diabetes=0;
        float resultprob;
        float classes=2;
        float tam;

        for(int c=0;c<num_clusters;c++){
            tam=0;
            resultprob=0;
            pcom_diabetes=0;
            psem_diabetes=0;
            for(int i=num_clusters;i<LINHA;i++){


                            if(clusters[i]==c){

                                 if(matriz[i][COLUNA]==0)
                                    psem_diabetes++;

                                 if(matriz[i][COLUNA]==1)
                                    pcom_diabetes++;

                                tam++;
                            }

            }

            resultprob=calculaLog(pcom_diabetes,psem_diabetes,2,tam);
            printf("\nCluster %d:",c);
            printf("\n%.1f pessoas com diabetes\n",pcom_diabetes);
            printf("%.1f pessoas sem diabetes\n",psem_diabetes);
            printf("Entropia do cluster: %.3f\n",resultprob);

      }

}

void calculaEntropiaIris(int **clusters,int num_clusters,float **matriz){

        float Iris_setosa=0;
        float Iris_versicolor=0;
        float Iris_virginica=0;
        float resultprob;
        float tam;

        for(int c=0;c<num_clusters;c++){
            tam=0;
            resultprob=0;
            Iris_setosa=0;
            Iris_versicolor=0;
            Iris_virginica=0;
            for(int i=num_clusters;i<LINHA;i++){


                            if(clusters[i]==c){

                                 if(matriz[i][COLUNA]==0)
                                    Iris_setosa++;

                                 if(matriz[i][COLUNA]==1)
                                    Iris_versicolor++;

                                 if(matriz[i][COLUNA]==2)
                                    Iris_virginica++;

                                tam++;
                            }

            }

            resultprob=calculaLogIris(Iris_setosa,Iris_versicolor,Iris_virginica,2,tam);
            printf("\nCluster %d:",c);
            printf("\n%.1f Iris-setosa\n",Iris_setosa);
            printf("\n%.1f Iris_versicolor\n",Iris_versicolor);
            printf("\n%.1f Iris_virginica\n",Iris_virginica);
            printf("Entropia da classe:%.3f\n",resultprob);


      }

}


void clusterizacao(float **matriznormalizada,int num_clusters,float **matriz,int op){

        float dist1,dist2,menordist,qtdcluster=0;
        int tamanho=2,cluster;
        float SSE;
        float SSEaux=0;
        int interacoes=0;
        int cont=0;
        int **clusters=alocaClusters();

        while(1){


                for(int i=num_clusters;i<LINHA;i++){

                    cluster=menorDistancia(i,num_clusters,matriznormalizada);
                    clusters[i]=cluster;

                }

                 SSE=calculaSSE(matriznormalizada,num_clusters,LINHA,clusters);
                 atualizaCentroide(matriznormalizada,clusters,num_clusters);


                 if(!verificaSSE(SSE,SSEaux)){

                        SSEaux=SSE;

                    }else{

                        break;
                    }
                interacoes++;

            }
            system("clear");
            printf("\033[1;32m----------RESULTADOS----------\033[0m\n");
            printf("Foram realizadas %d iteracoes\n",interacoes);
            printf("SSE para %d clustes:%.2f\n",num_clusters,SSE);

            if(op==1){
                calculaEntropia(clusters,num_clusters,matriz);

            }else if(op==2){

                calculaEntropiaIris(clusters,num_clusters,matriz);
            }

            //salvaCluster(clusters,num_clusters,matriznormalizada);

}




