#include "funcoeskmeans.h"

//-------------------------HEADER----------------------------------------------------|
// AUTOR             : Marlon Brendo <marlonbrendo2013@gmail.com>
// DATA-DE-CRIAÇÃO   : 2019
// PROGRAMA          : Kmeans
// LICENÇA           : MIT
// DESCRIÇÃO 	       : Algoritmo kmeans desenvolvido em C na base de dados pima-indians-diabetes e Iris
//

int main()
{


    FILE *data;
    float **matriz;
    int pos,qtd,op_base;
    int op;

      do{

        op=menu();

        switch(op){

                case 1:
                       printf("Informe a base a ser carregada:\n[1]-Base Diabetes\n[2]-Base Iris\n");
                       printf(">:");
                       scanf("%d",&op_base);
                       matriz=carregaBancoDados(data,op_base);

                       if(matriz!=NULL){

                            system("clear");
                            printf("\033[1;32mCarregado com sucesso! \033[0m\n");


                       }else{

                            system("clear");
                            printf("\033[1;31m[!]\033[0m Erro ao carregar! :(\n");

                       }

                break;

                case 2:

                    if(matriz==NULL){

                            system("clear");
                            printf("\033[1;31m[!]\033[0m Carregue a base de dados primeiro!\n");

                    }else{

                            printf("Informe a quandtidade clusters:>");
                            scanf("%d",&qtd);
                            MinMax(matriz,qtd,op_base);

                    }
                break;


                case 3:

                            system(0);

                break;

                default:
                            system("clear");
                            printf("\033[1;31m[!]\033[0m Opcao invalida!\n");
                break;

        }



       }while(op!=3);


    return 0;
}
