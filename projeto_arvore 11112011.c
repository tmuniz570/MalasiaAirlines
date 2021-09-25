/*
        ########## Trabalho de P2 ############
        _______________________________________
       |,---"-----------------------------"---,|
       ||___    16 bit....................    ||
       ||====\ :HHHHHHHHHHHHHHHHHHHHHHHHHHH   ||
       ||=====):H                         H   ||
       ||====/ :H     MALASIA AIRLINES    H   ||
       ||"""   :H           P2            H   ||
       ||      :H                         H   ||
       ||      :HHHHHHHHHHHHHHHHHHHHHHHHHHH   ||
       ||_____,_________________________,_____||
       |)_____)-----.| /I\ UFAL |.------(_____(|
     //"""""""|_____|=----------=|______|"""""""\
    // _| _| _| _| _| _| _| _| _| _| _| _| _| _| \
   // ___| _| _| _| _| _| _| _| _| _| _| _|  |  | \
  |/ ___| _| _| _| _| _| _| _| _| _| _| _| ______| \
  / __| _| _| _| _| _| _| _| _| _| _| _| _| _| ___| \
 / _| _| _| _| ________________________| _| _| _| _| \
|------"--------------------------------------"-------|
`-----------------------------------------------------'

*/



/*--------------------------------- BIBLIOTECAS ----------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define OS 1//0-LINUX; 1-WINDOWS

/*---------------------------------- ESTRUTURAS ----------------------------------*/

typedef struct reserva{
    char nome[51];
    int num_voo;
    struct reserva *prox, *ant;
}RESERVA;


typedef struct aviao{
    int num;
    RESERVA *reservas;
    int poltronas_total, poltronas_livre;
    int alte, altd;
    struct aviao *esq, *dir, *pai;
}AVIAO;

/*------------------------------------ FUNÇÕES ------------------------------------*/

void freebuffer(){
    if(OS)
        fflush(stdin);
    else
        __fpurge(stdin);
}

void pause(){
    freebuffer();
    getchar();
}


RESERVA *new_reserva(int num){
    RESERVA *r;
    r = (RESERVA*)malloc(sizeof(struct reserva));
    if(r != NULL){
        r->prox = NULL;
        r->ant = NULL;
        r->num_voo = num;
    }
    else{
        printf("NAO FOI POSSIVEL CRIAR UMA NOVA RESERVA!");
    }
    return(r);
}

AVIAO *new_plane(){
    AVIAO *p;
    p = (AVIAO*) malloc(sizeof(AVIAO));
    if(p != NULL){
        p->esq = NULL;
        p->dir = NULL;
        p->reservas = NULL;
    }
    else{
        printf("NAO FOI POSSIVEL CRIAR UM NOVO AVIAO!");
    }
    return(p);
}

int altura(AVIAO *plane) {
    int x, y;
    if (plane == NULL) return -1;
    x = altura(plane->esq);
    y = altura(plane->dir);
    plane->alte = x + 1;
    plane->altd = y + 1;
    if (x > y){
        return (x + 1);
    }
    else{
        return (y + 1);
    }
}

AVIAO *rotacao_esquerda(AVIAO* aux){
	AVIAO *aux1, *aux2;
	aux1 = aux->dir;
	aux2 = aux1->esq;
	aux1->esq=aux;
	aux->dir = aux2;
	altura(aux1);
	return aux1;
}

AVIAO *rotacao_direita(AVIAO* aux){
	AVIAO *aux1, *aux2;
	aux1 = aux->esq;
	aux2 = aux1->dir;
	aux1->dir = aux;
	aux->esq = aux2;
	altura(aux1);
	return aux1;
}

void pai (AVIAO *plane, AVIAO *aux){
    if (plane != NULL){
    plane->pai = aux;
    pai(plane->esq, plane);
    pai(plane->dir, plane);
    }
}

AVIAO *balanceamento(AVIAO *plane){
	int dif, dif_filho;
	dif = plane->altd - plane->alte;
	if (dif == 2){
		dif_filho = plane->dir->altd - plane->dir->alte;
		if (dif_filho >= 0){
			plane = rotacao_esquerda(plane);
		}
		else{
			plane->dir = rotacao_direita(plane->dir);
			plane = rotacao_esquerda(plane);
		}
	}
	else if (dif == -2){
	    dif_filho = plane->esq->altd - plane->esq->alte;
		if (dif_filho <= 0){
			plane = rotacao_direita(plane);
		}
		else{
			plane->esq = rotacao_esquerda(plane->esq);
			plane = rotacao_direita(plane);
		}
	}
	pai(plane, NULL);
	return plane;
}

void troca_reserva(RESERVA *dir,RESERVA *esq){
	char nome[51];
	int num_voo = dir->num_voo;
    strcpy(nome, dir->nome);

	dir->num_voo = esq->num_voo;
	strcpy(dir->nome, esq->nome);

	esq->num_voo = num_voo;
	strcpy(esq->nome, nome);
}

void quick_reserva(RESERVA* esq,RESERVA* dir){
	char pivo[51];//para a comparação
	strcpy(pivo, esq->nome);
	RESERVA* inicio = esq;
	RESERVA* fim = dir;
	int	cruzou = 0;
	do{
		while (cruzou == 0 && strcmp(esq->nome, pivo) <= 0){
			if(esq == dir)
				cruzou = 1;
			esq = esq->prox;
		}
		while (strcmp(dir->nome, pivo) > 0){
			if(esq == dir)
				cruzou=1;
			dir = dir->ant;
		}
		if (cruzou == 0){
			troca_reserva(esq, dir);
		}
	} while (cruzou == 0);
	// troca pivô
	if(inicio != dir)
		troca_reserva(inicio, dir);
	//chama pros lados do pivô(dir)
	if(inicio != dir)
		quick_reserva(inicio, dir->ant);
	if(dir != fim)
		quick_reserva(dir->prox, fim);
}

RESERVA *dir_list_reserva(RESERVA *res){//localizar o endereço do ultimo elemento a direita da lista reservas do aviao
    while (res->prox != NULL && res != NULL){
        res = res->prox;
    }
    return (res);
}

AVIAO *add_plane(AVIAO *aviao, int num, int poltronas){
    AVIAO *aviao_aux = NULL;
    if(aviao == NULL){
        aviao = new_plane();
        aviao->num = num;
        aviao->poltronas_total = poltronas;
        aviao->poltronas_livre = poltronas;
    }
    else{
        aviao_aux = aviao;
        while ((num > aviao_aux->num && aviao_aux->dir != NULL) || (num < aviao_aux->num && aviao_aux->esq != NULL)){
            if (num > aviao_aux->num){
                aviao_aux = aviao_aux->dir;
            }
            else{
                aviao_aux = aviao_aux->esq;
            }
        }
        if (num > aviao_aux->num){
            aviao_aux->dir = new_plane();
            aviao_aux->dir->num = num;
            aviao_aux->dir->poltronas_total = poltronas;
            aviao_aux->dir->poltronas_livre = poltronas;
        }
        else{
            aviao_aux->esq = new_plane();
            aviao_aux->esq->num = num;
            aviao_aux->esq->poltronas_total = poltronas;
            aviao_aux->esq->poltronas_livre = poltronas;
        }
    }
    altura(aviao);
    aviao = balanceamento(aviao);
    altura(aviao);
    pai(aviao, NULL);
    return (aviao);
}

AVIAO *search_aviao(AVIAO* aviao, int num){
    AVIAO *aviao_aux = NULL;
	if (aviao != NULL){
		if (aviao->num == num)
			aviao_aux = aviao;
		else if ( num < aviao->num )
			aviao_aux = search_aviao(aviao->esq, num);
		else
			aviao_aux = search_aviao(aviao->dir, num);
	}
	return aviao_aux;
}

/*----------------------------------------------------------*/

void add_reserva(AVIAO *aviao, int num, char nome[]){
    AVIAO *aviao_aux;
    aviao_aux = search_aviao(aviao, num);
    aviao_aux->poltronas_livre--;
    if(aviao_aux->reservas == NULL){
        aviao_aux->reservas = new_reserva(num);
        strcpy(aviao_aux->reservas->nome, nome);
        }
    else{
        RESERVA *reserva_aux;
        reserva_aux = new_reserva(num);
        strcpy (reserva_aux->nome, nome);
        reserva_aux->prox = aviao_aux->reservas;
        aviao_aux->reservas->ant = reserva_aux;
        aviao_aux->reservas = reserva_aux;
        }
}

void show_reservas(AVIAO *aviao, int num){
    RESERVA *reserva_aux;
    int i = 0;
    reserva_aux = search_aviao(aviao, num)->reservas;
        if(reserva_aux != NULL){
            quick_reserva(search_aviao(aviao, num)->reservas, dir_list_reserva(search_aviao(aviao, num)->reservas));
            do{
                if(reserva_aux == NULL){
                    break;
                }
                printf("\n%s", reserva_aux->nome);
                reserva_aux = reserva_aux->prox;
                i++;
            }while(reserva_aux != NULL);
            printf("\n\nTOTAL DE RESERVAS: %d\nPOLTRONAS LIVRE: %d\n\n", i, search_aviao(aviao, num)->poltronas_livre);
        }
        else{
            printf("\nNAO EXISTEM RESERVAS NESTE AVIAO!");
        }
}

void show_planes(AVIAO *plane){
    if (plane != NULL){
        if (plane->pai == NULL){
            printf("                      -----------------------------------\n                     |   AVIAO   |  TOTAL DE POLTRONAS   |\n                      -----------------------------------\n");
        }
        show_planes (plane->esq);
        printf("                     |%7.4d%5c%13.2d%11c\n                      -----------------------------------\n", plane->num, '|', plane->poltronas_total, '|');
        show_planes (plane->dir);
    }
}

int search_passageiro (AVIAO *paviao, char name[], int achou){
    RESERVA *preserva;
    if (paviao != NULL){
        achou = search_passageiro (paviao->esq, name, achou);
        preserva = paviao->reservas;
        while (preserva != NULL){
            if (!strcmp(name, preserva->nome)){
                if (achou == 0){
                    printf ("\n O Pasageiro: %s esta,\n\n", name);
                }
                printf ("NO AVIAO: %d\n", preserva->num_voo);
                achou = 1;
            }
            preserva = preserva->prox;
        }
        achou = search_passageiro (paviao->dir, name, achou);
        if (achou == 0 && paviao->pai == NULL){
            printf ("\nPASSAGEIRO NAO ENCONTRADO!\n");
        }
    }
    return (achou);
}

AVIAO *limpa(AVIAO *plane){
    RESERVA *clean_res;
	if (plane!=NULL){
	    while(plane->reservas != NULL){
            clean_res = plane->reservas;
            plane->reservas = plane->reservas->prox;
            free(clean_res);
        }
		plane->esq=limpa(plane->esq);
		plane->dir=limpa(plane->esq);
		free (plane);
	}
	return NULL;
}

AVIAO *ler_arquivo (){
    AVIAO plane, *inicio = NULL;
    RESERVA res;
    FILE *arq1, *arq2;
    arq1 = fopen ("voos", "a+b");
    arq2 = fopen ("reservas", "a+b");
    while(fread(&plane, sizeof(AVIAO), 1, arq1) != 0){
        inicio = add_plane(inicio, plane.num, plane.poltronas_total);
    }
    while(fread(&res, sizeof(RESERVA), 1, arq2) != 0){
        add_reserva(inicio, res.num_voo, res.nome);
    }
    fclose(arq1);
    fclose(arq2);
    return(inicio);
}

void gravar_arquivo (AVIAO *paviao, int limpa){
    FILE *arq1, *arq2;
    if (limpa == 1){
        arq1 = fopen ("voos", "wb");
        arq2 = fopen ("reservas", "wb");
        fclose(arq1);
        fclose(arq2);
        limpa = 0;
    }
    arq1 = fopen ("voos", "ab");
    arq2 = fopen ("reservas", "ab");
    if (paviao != NULL){
    gravar_arquivo(paviao->esq, limpa);
    fwrite(paviao, sizeof(AVIAO), 1, arq1);
    while(paviao->reservas != NULL){
        fwrite(paviao->reservas, sizeof(RESERVA), 1, arq2);
        paviao->reservas = paviao->reservas->prox;
    }
    gravar_arquivo(paviao->dir, limpa);
    }
    fclose(arq1);
    fclose(arq2);
}

void creditos (){
     system ("clear || cls");
     printf ("\n\n                       UFAL - Projeto de Programacao 2\n                          Prof. Alan Pedro da Silva\n\n                                  Kin Torres\n                                Thiago Brandao\n\n                                  11/11/2011\n\n\n");
     pause();
}

/*--------------------------------- FUNÇÃO PRINCIPAL ---------------------------------*/


int main()
{
    AVIAO *planes = NULL;
    char opcao;
    int i, j;
    char name[51];

    planes = ler_arquivo();


    do
    {
        printf("                    CENTRAL DE RESERVAS - MALASIA AIRLINES                    \n\n");
        printf("MENU PRINCIPAL:\n\n");
        printf("    1 - CADASTRAR AVIAO\n");
        printf("    2 - RESERVAR PASSAGEM\n");
        printf("    3 - CONSULTAR\n");
        printf("    0 - FINALIZAR\n");

        printf("\nOPCAO: ");
        freebuffer();
        opcao = getchar();
        system("clear || cls");

        switch(opcao)
        {
            case '1':                               //cadastra os aviões

                printf("DIGITE O NUMERO DO AVIAO: ");
                scanf("%d", &i);
                if (search_aviao(planes, i) != NULL){
                    printf("AVIAO PREVIAMENTE CADASTRADO!");
                    pause();
                    break;
                }
                do{
                printf("DIGITE O NUMERO TOTAL DE POLTRONAS DO AVIAO(1-60): ");
                scanf("%d", &j);
                if (j<1 || j>60){
                    printf ("O NUMERO PASSAGEIROS E DE 1 A 60!");
                    pause();
                    system("clear || cls");
                }
                }while (j<1 || j>60);

                planes = add_plane(planes,i,j);
                break;

            case '2':                               //reserva passagem

                printf("DIGITE O NUMERO DO AVIAO: ");
                scanf("%d", &i);

                if(search_aviao(planes, i) == NULL || search_aviao(planes, i)->poltronas_livre <= 0)
                {
                    printf("\nAVIAO NAO ENCONTRADO OU NAO HA MAIS VAGAS DISPONIVEIS!\n");
                    pause();
                    break;
                }

                printf("DIGITE O NOME DO PASSAGEIRO: ");
                freebuffer();
                gets(name);
                add_reserva(planes, i, name);
                break;

            case '3':                               //consulta no sistema

                printf("                    CENTRAL DE RESERVAS - MALASIA AIRLINES                    \n\n");
                printf("CONSULTAR:\n\n");
                printf("    1 - AVIOES CADASTRADOS\n");
                printf("    2 - DADOS DO AVIAO\n");
                printf("    3 - POR NOME DO PASSAGEIRO\n");
                printf("    4 - RETORNAR AO MENU PRINCIPAL\n");

                printf("\nOPCAO: ");
                freebuffer();
                opcao = getchar();

                system("clear || cls");
                printf("                    CENTRAL DE RESERVAS - MALASIA AIRLINES                    \n\n");

                switch(opcao)
                {
                    case '1':
                        if (planes != NULL){
                        //printf("                      -----------------------------------\n                     |   AVIAO   |  TOTAL DE POLTRONAS   |\n                      -----------------------------------\n");
                        show_planes(planes);
                        }
                        else{
                            printf("NAO EXISTEM AVIOES CADASTRADOS!\n");
                        }

                        pause();
                        break;

                    case '2':
                        printf("DIGITE O NUMERO DO AVIAO: ");
                        freebuffer();
                        scanf("%d", &i);

                        if(search_aviao(planes, i) == NULL){
                            printf("\nAVIAO NAO ENCONTRADO!\n");
                            pause();
                            break;
                        }

                        show_reservas(planes, i);

                        pause();
                        break;

                    case '3':
                        printf ("DIGITE O NOME DO PASSAGEIRO: ");
                        freebuffer();
                        gets(name);
                        search_passageiro(planes, name, 0);
                        pause();
                        break;

                    case '4':

                    break;

                    default:
                    printf ("\nOPCAO INVALIDA!\n");
                    pause();
                    break;

                }

                opcao = '3';

                break;

            case '0':                               //finaliza o programa

                break;

            default:                                //não faz nada

                printf("\n\nOPCAO INVALIDA!\n\n\n\n");
                pause();
                break;
        };




        system("clear || cls");

    }while(opcao != '0');


/*------------------------------------ CRÉDITOS ------------------------------------*/


creditos();

/*--------------------------------------- FIM ---------------------------------------*/
    gravar_arquivo(planes, 1);
    limpa(planes);
    return 0;

}
