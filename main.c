/* PROJECTO PPP ÂNGELO LOUREIRO */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define LINHA printf("\n");
#define INPUT printf(">>>\t");

// estrutura para as notas

typedef struct banco * banc;
typedef struct banco
{
    int cinco,dez,vinte,cinquenta;
}caixa;

// lista ligada para os utilizadores

typedef struct user *list;
typedef struct user
{
    int userid,pin,montante,tipo,estado; // 0 se cliente 1 se funcionario ;0 se activo, 1 se bloqueado
    list *next;
}utilizadores;

typedef struct histo *list1;
typedef struct histo
{
    int operacao,opuserid,opdestino;
    list1 *next;
}historico;



list criaheader (void);
list1 criahistorico(void);
void cria_teste(list header, int nuserid, int npin, int nmontante, int ntipo, int nestado);
void hist_cria(list1 headerh,int noperacao, int nopuserid,int nopdestino);
void printhist(list header,list1 headerh,int login,int pin,caixa notas);
void elimina_cliente (list header,list1 headerh,int login, int pin,caixa notas);
void cria_cliente (list header,list1 headerh,int login,int pin,caixa notas);
void lista_clientes (list header,list1 headerh,int login,int pin,caixa notas);
int procura_clientes(list header,int chave); //retorna 1 caso encontre, 0 caso contrario
int desbloqueia_clientes(list header,list1 headerh,int login, int pin,caixa notas); // retorna 1 caso alterado, 0 caso contrario
void consulta_clientes(list header,list1 headerh,int login,int pin,caixa notas); // retorna 1 caso encontre, 0 caso contrario
caixa defaultnotas(int ncinco, int ndez, int nvinte, int ncinq);
void alterar_notas(list header,list1 headerh, int login, int pin, caixa notas);
int login(list header,list1 headerh, int testeid, int testepin,caixa notas);
void menu(list header,list1 headerh,int login,int pin,caixa notas);
void autoconsulta(list header,list1 headerh,int login, int pin,caixa notas);
int cliente_altera(list header,list1 headerh,int login,int pin,caixa notas); // retorna 1 caso alterado, 0 caso contrario
void cliente_deposita(list header, list1 headerh,int login, int pin, caixa notas);
void cliente_transfere(list header,list1 headerh,int login, int pin,caixa notas);
void cliente_levanta(list header,list1 headerh, int login, int pin, caixa notas);
void decuser(list header,list1 headerh,int login,int pin,caixa notas);
void decfunc(list header,list1 headerh,int login,int pin,caixa notas);
void menucliente(list header,list1 headerh,int login, int pin,caixa notas);
void escrever_clientes(list header);
void escrever_notas(caixa notas);
caixa ler_notas();
void ler_clientes(list header);
void ler_hist(list1 headerh);
void escrever_hist(list1 headerh);


int main(int argc, int *argv[])
{
    list header = criaheader();
    list l = header->next;
    list1 headerh = criahistorico();

    int loginid, pin, tentativas = 0;
    int teste;
    caixa notas = ler_notas();

    ler_clientes(header);
    ler_hist(headerh);


    system("date");

    printf("\t\t :::::::::::::::::::::::::::::::::::::::::\n");
    printf("\t\t :::                                   :::\n");
    printf("\t\t :::      Caixa Bancário Automatico    :::\n");
    printf("\t\t :::                                   :::\n");
    printf("\t\t :::                 PPP               :::\n");
    printf("\t\t :::                                   :::\n");
    printf("\t\t :::                                   :::\n");
    printf("\t\t :::::::::::::::::::::::::::::::::::::::::\n");
    printf("\n\n");
    printf("\t\t    ********EFECTUE O SEU LOGIN********\n");

    printf("Insira o ID utilizador:\n");
    INPUT
    scanf("%6d",&loginid);
    //itoa(loginid,converte,10);
    //printf("%s",converte);
    printf("Insira o pin:\n");
    INPUT
    scanf("%4d",&pin);

    if (login(header,headerh,loginid,pin,notas) == 1)
    {
        printf("Sessão iniciada para utilizador: %d\n\n",loginid);
        menu(header,headerh,loginid,pin,notas);
    }

    else if (login(header,headerh,loginid,pin,notas) == 2)
    {
        printf("Sessão iniciada para utilizador: %d\n\n",loginid);
        menucliente(header,headerh,loginid,pin,notas);
    }

    else if (login(header,headerh,loginid,pin,notas) == 3)
    {
        printf("Dados invalidos ou conta bloqueada! Tente novamente\n");
        while (tentativas<=1)
        {
            printf("Insira o pin:\n");
            INPUT
            scanf("%d",&pin);

            if (login(header,headerh,loginid,pin,notas) == 3)
            {
                tentativas+= 1;
                printf("Dados invalidos ou conta bloqueada! Tente novamente\a\n");
            }
            else if (login(header,headerh,loginid,pin,notas) == 1)
            {
                printf("Sessão iniciada para utilizador: %d\n\n",loginid);
                menu(header,headerh,loginid,pin,notas);
                break;
            }
            else if (login(header,headerh,loginid,pin,notas) == 2)
            {
                printf("Sessão iniciada para utilizador: %d\n\n",loginid);
                menucliente(header,headerh,loginid,pin,notas);
                break;
            }
            else
                printf("Dados invalidos ou conta bloqueada! Tente novamente\n");
        }
    }
    else printf("Dados invalidos ou conta bloqueada! Tente novamente!\n");
    if (tentativas==2)
    {
        l = header->next;
        while (l)
        {
            if ((l->userid == loginid) && (l->estado == 0))
            {
                //printf("%d\n",header->estado);
                l->estado = 1;
                printf("Bloqueou o cartão\n");
                escrever_clientes(header);
                hist_cria(headerh,12,loginid,1);
                escrever_hist(headerh);
            }
            l=l->next;
        }
    }
}



/*---------------------------------------------------------------------------*/
/*----------------------------------OPERAÇÕES--------------------------------*/
/*---------------------------------------------------------------------------*/

// função que cria o header da lista

list criaheader (void)
{
    list header;
    header = (list) malloc (sizeof (utilizadores));
    if (header != NULL)
    {
        header->next = NULL;
    }
    return header;
}

list1 criahistorico(void)
{
    list1 headerh;
    headerh = (list) malloc (sizeof(historico));
    if (headerh != NULL)
    {
        headerh->next = NULL;
    }
    return headerh;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

// função para criar utilizadores de teste

void cria_teste(list header, int nuserid, int npin, int nmontante, int ntipo, int nestado)
{
    list novo;

    novo = (list) malloc (sizeof (utilizadores));
    if (novo != NULL)
    {
        novo->userid = nuserid;
        novo->pin = npin;
        novo->montante = nmontante;
        novo->tipo = ntipo;
        novo->estado = nestado;
        novo->next = header->next;
        header->next = novo;
    }
}

void hist_cria(list1 headerh,int noperacao, int nopuserid,int nopdestino)
{
    list1 novo;
    novo = (list) malloc (sizeof(historico));
    if (novo!=NULL)
    {
        novo->operacao=noperacao;
        novo->opuserid=nopuserid;
        novo->opdestino=nopdestino;
        novo->next=headerh->next;
        headerh->next=novo;
    }
}

void printhist(list header,list1 headerh,int login,int pin,caixa notas)
{
    printf("\n");
    list1 h = headerh->next;
    printf("TIPO\t\tID\t\tOPERACAO\t\tDESTINO\n\n");
    while (h)
    {

        if (h->operacao == 1) printf("FUN\t\t%d\t\tUSERADD\t\t\t%d\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 2) printf("FUN\t\t%d\t\tUSERDELL\t\t%d\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 3) printf("FUN\t\t%d\t\tLISTUSER\t\t\n",h->opuserid);
        else if (h->operacao == 4) printf("FUN\t\t%d\t\tUSERCHECK\t\t%d\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 5) printf("FUN\t\t%d\t\tUSERUNLOCK\t\t%d\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 6) printf("FUN\t\t%d\t\tSTOCKCHANGE\t\t\n",h->opuserid);
        else if (h->operacao == 7) printf("CLI\t\t%d\t\tCONSULT\t\t\n",h->opuserid);
        else if (h->operacao == 8) printf("CLI\t\t%d\t\tDEPOSIT\t\t\t%d€\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 9) printf("CLI\t\t%d\t\tLEVANTA\t\t\t%d€\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 10) printf("CLI\t\t%d\t\tTRANSFERE\t\t%d€\t\t\n",h->opuserid,h->opdestino);
        else if (h->operacao == 11) printf("CLI\t\t%d\t\tALTERA\t\t\t%d\n",h->opuserid,h->opdestino);
        else if (h->operacao == 12) printf("CLI\t\t%d\t\tBLOQUEADO\t\t\n",h->opuserid);
        h=h->next;
    }
    decfunc(header,headerh,login,pin,notas);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void elimina_cliente (list header,list1 headerh,int login, int pin,caixa notas)
{
    list auxant;
    list auxprox;
    int chave;

    printf("Introduza o ID do cliente que pretende eliminar\n");
    INPUT
    scanf("%d",&chave);

    auxprox = header;

    while (auxprox->next != NULL)
    {
        auxant = auxprox;
        auxprox = auxprox->next;
        if ((auxprox->userid) == chave)
        {
            auxant->next = auxprox->next;
            free(auxprox);
            printf("Utilizador %d eliminado com sucesso\n",chave);
            hist_cria(headerh,2,login,chave);
            //printhist(header,headerh,login,pin,notas);
            decfunc(header,headerh,login,pin,notas); // função para voltar ao menu de funcionario
            return;
        }
    }
    printf("Utilizador não existe\n");
    decfunc(header,headerh,login,pin,notas);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void cria_cliente (list header,list1 headerh,int login,int pin,caixa notas)
{
    int nuserid,npin,nmontante,ntipo,conf,nestado;
    list novo;
comeco:
    do
    {
        printf("Insira o userid: \n");
        INPUT
        scanf("%d",&nuserid);
        if (procura_clientes(header,nuserid) == 1)
        {
            printf("\tERRO: Utilizador já existe, insira outro USERID\n");
            printf("\t-------------------------------------------------\n");
            goto comeco;
        }
        printf("Insira o pin (4 digitos): \n");
        INPUT
        scanf("%4d",&npin);
        printf("Insira o montante inicial: \n");
        INPUT
        scanf("%d",&nmontante);
        printf("Insira o tipo de utilizador: \n");
        printf("0  -  Cliente\t1  -  Funcionario\t");
        INPUT
        scanf("%d",&ntipo);
        printf("Insira o estado inicial do utilizador: \n");
        printf("0  -  Activo\t1  -  Bloqueado\t\t");
        INPUT
        scanf("%d",&nestado);
        printf("0 - Alterar\t\t\t1 - Confirmar\t");
        INPUT
        scanf("%d",&conf);
    }
    while (conf==0);

    novo = (list) malloc (sizeof (utilizadores));
    if (novo != NULL)
    {
        novo->userid = nuserid;
        novo->pin = npin;
        novo->montante = nmontante;
        novo->tipo = ntipo;
        novo->estado = nestado;
        novo->next = header->next;
        header->next = novo;
    }
    printf("%s\t" ,__TIME__);
    printf("CLIENTE CRIADO COM SUCESSO\n");
    hist_cria(headerh,1,login,nuserid);
    decfunc(header,headerh,login,pin,notas);
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void lista_clientes (list header,list1 headerh,int login,int pin,caixa notas)
{
    list l = header->next;
    list k = header->next;
    int op,op2,i,j,max,min;
    int n = 0;
    int MAXSIZE = 0;
    int array[MAXSIZE];
start:
    printf("Ordenamento por:\n 1 - Ordem crescente de saldo \t 2 - Ordem decrescente de saldo\n");
    INPUT scanf("%d",&op);

    if (op == 2)
    {
        while (l)  //descobre o numero de nós e faz um vector
        {
            MAXSIZE++;
            array[MAXSIZE-1] = l->montante;
            l=l->next;
        }
        for (i = 0; i < MAXSIZE; i++) //ordena o vector, selection sort
        {
            for (j = (i+1); j < MAXSIZE;j++)
            {
                max = i;
                if (array[j] > array[max]) max = j;

                if (i != max)
                {
                    int temp = array[i];
                    array[i] = array[max];
                    array[max] = temp;
                }
            }
        }
        printf("UTILIZADOR ID");
        printf("\t\tSaldo\n\n");
        while (k) // percorre a lista e verifica o utilizador correspondente ao montante de valor array[n]
        {
            if (array[n] == k->montante)
            {
                printf("%d \t\t\t(%d€)\n",k->userid, k->montante);
                n++;
            }
            k=k->next;
            if ((k == NULL) && (n<MAXSIZE))
                k = header->next;
        }
    }
    else if (op==1)
    {
        while (l)  //descobre o numero de nós e faz um vector
        {
            MAXSIZE++;
            array[MAXSIZE-1] = l->montante;
            l=l->next;
        }
        for (i = 0; i < MAXSIZE; i++) //ordena o vector, selection sort
        {
            for (j = (i+1); j < MAXSIZE;j++)
            {
                min = i;
                if (array[j] < array[min]) min = j;

                if (i != min)
                {
                    int temp = array[i];
                    array[i] = array[min];
                    array[min] = temp;
                }
            }
        }
        printf("UTILIZADOR ID");
        printf("\t\tSaldo\n\n");
        while (k) // percorre a lista e verifica o utilizador correspondente ao montante de valor array[n]
        {
            if (array[n] == k->montante)
            {
                printf("%d \t\t\t(%d€)\n",k->userid, k->montante);
                n++;
            }
            k=k->next;
            if ((k == NULL) && (n<MAXSIZE))
                k = header->next;
        }

    }
    else
    {
        printf("Escolha apenas uma das opções indicadas!\n");
        printf("1 - Tentar novamente\t0 - Menu\n");
        INPUT scanf("%d",&op2);
        if (op2==1) goto start;
        else if (op2==0) decfunc(header,headerh,login,pin,notas);
        else printf("Opção invalida, voltando ao menu!");
    }
    hist_cria(headerh,3,login,0);    //printhist(headerh);
    decfunc(header,headerh,login,pin,notas);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

int procura_clientes(list header,int chave) //retorna 1 caso encontre, 0 caso contrario
{
    list l = header->next; // Salta o header //

    while (l)
    {
        if (l->userid == chave)
            //printf("Encontrou\n");
            return 1;
        l=l->next;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

int desbloqueia_clientes(list header,list1 headerh,int login, int pin,caixa notas) // retorna 1 caso alterado, 0 caso contrario
{
    list l = header->next; // Salta o header //
    list j = header->next;
    int chave;

    printf("Indique o userID que pretende desbloquear\n");
    printf("\tBloqueados:\n");
    while (j)
    {
        if (j->estado == 1) printf("\t\t\t%d\n",j->userid);
        j=j->next;
    }
    INPUT
    scanf("%d",&chave);

    while (l)
    {
        if ((l->userid == chave) && (l->estado == 1))
        {
            l->estado = 0;
            printf("Cliente %d desbloqueado com sucesso\n",chave);
            hist_cria(headerh,5,login,chave);
            decfunc(header,headerh,login,pin,notas);
            return 1;
        }
        l=l->next;
    }
    printf("Dados invalidos\n");
    decfunc(header,headerh,login,pin,notas);
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

void consulta_clientes(list header,list1 headerh,int login,int pin,caixa notas) // retorna 1 caso encontre, 0 caso contrario
{
    list l = header->next; // Salta o header //
    int chave;

    printf("Indique o USERID a consultar");
    INPUT
    scanf("%d",&chave);

    while (l)
    {
        if (l->userid == chave)
        {
            printf("USERID %d\n",l->userid);
            printf("PIN: %d\n",l->pin);
            printf("MONTANTE: %d\n",l->montante);
            printf("TIPO: %d\n",l->tipo);
            printf("ESTADO: %d\n",l->estado);
            hist_cria(headerh,4,login,chave);
            decfunc(header,headerh,login,pin,notas);
            return;
        }
        l=l->next;
    }
    printf("Utilizador não existe!");
    decfunc(header,headerh,login,pin,notas);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

// função que inicializa a caixa com um default de notas introduzido pelo progamador

caixa defaultnotas(int ncinco, int ndez, int nvinte, int ncinq)
{
    caixa notas;
    notas.cinco = ncinco;
    notas.dez = ndez;
    notas.vinte = nvinte;
    notas.cinquenta = ncinq;

    return notas;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


void alterar_notas(list header,list1 headerh, int login, int pin, caixa notas)
{
    int n, op, op2,total = 0;

    printf("\t1 - Inserir notas\t0 - Remover notas\t");
    INPUT
    scanf("%d",&op);
    LINHA
    if (op==1)
    {
        printf("\tIndique o valor da nota que pretende inserir->\n ");

        LINHA
        printf("\t1  -  5€( %d )\t\t2  -  10€( %d )\t\n",notas.cinco,notas.dez);
        printf("\t3  -  20€( %d )\t\t4  -  50€( %d )\t\n",notas.vinte,notas.cinquenta);

        total = (notas.cinco*5)+(notas.dez*10)+(notas.vinte*20)+(notas.cinquenta*50);
        printf("\t\tTotal na caixa:\t %d€ \n",total);
        INPUT
        scanf("%d",&op2);
        printf("Indique quantas notas pretende inserir-> ");
        switch (op2)
        {
        case 1:
            INPUT
            scanf("%d",&n);
            notas.cinco += n;
            printf("notas de 5: %d\n",notas.cinco);
            break;
        case 2:
            INPUT
            scanf("%d",&n);
            notas.dez += n;
            printf("notas de 10: %d\n",notas.dez);
            break;
        case 3:
            INPUT
            scanf("%d",&n);
            notas.vinte += n;
            printf("notas de 20: %d\n",notas.vinte);
            break;
        case 4:
            INPUT
            scanf("%d",&n);
            notas.cinquenta += n;
            printf("notas de 50: %d\n",notas.cinquenta);
            break;
        default:
            printf("Escolha apenas entre as opções indicadas\n");
        }
    }
    else if (op==0)
    {
        printf("\tIndique o valor da nota que pretende remover->\n ");
        LINHA
        printf("\t1  -  5€( %d )\t\t2  -  10€( %d )\t\n",notas.cinco,notas.dez);
        printf("\t3  -  20€( %d )\t\t4  -  50€( %d )\t\n",notas.vinte,notas.cinquenta);
        INPUT
        scanf("%d",&op2);
        printf("Indique quantas notas pretende remover-> ");
        switch (op2)
        {
        case 1:
            INPUT
            scanf("%d",&n);
            notas.cinco -= n;
            printf("notas de 5: %d\n",notas.cinco);
            break;
        case 2:
            INPUT
            scanf("%d",&n);
            notas.dez -= n;
            printf("notas de 10: %d\n",notas.dez);
            break;
        case 3:
            INPUT
            scanf("%d",&n);
            notas.vinte -= n;
            printf("notas de 20: %d\n",notas.vinte);
            break;
        case 4:
            INPUT
            scanf("%d",&n);
            notas.cinquenta -= n;
            printf("notas de 50: %d\n",notas.cinquenta);
            break;
        default:
            printf("Escolha apenas das opções indicadas\n");
        }
    }

    else
        printf("Escolha apenas das opções indicadas\n");

    total = (notas.cinco*5)+(notas.dez*10)+(notas.vinte*20)+(notas.cinquenta*50);
    printf("Total na caixa:\t %d€ \n",total);
    escrever_notas(notas);
    hist_cria(headerh,6,login,0);
    escrever_hist(headerh);

    decfunc(header,headerh,login,pin,notas);

}
/*---------------------------------------------------------------------------*/
/*----------------------------------LOGIN------------------------------------*/
/*---------------------------------------------------------------------------*/
// função para o login, caso 1 login funcionario feito, caso 2 login cliente feito, caso 3 password errada

int login(list header,list1 headerh, int testeid, int testepin,caixa notas)
{
    list l = header->next;

    while (l)
    {
        if ((l->userid == testeid) && (l->pin == testepin) && (l->estado == 0) && (l->tipo == 1))
        {
            system("clear");
            return 1;
        }
        else if ((l->userid == testeid) && (l->pin == testepin) && (l->estado == 0) && (l->tipo == 0))
        {
            system("clear");
            return 2;
        }
        else if ((l->userid == testeid) && (l->pin != testepin) && (l->estado == 0))
        {
            system("clear");
            return 3;
        }
        l=l->next;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
/*----------------------------------MENU FUNC--------------------------------*/
/*---------------------------------------------------------------------------*/

void menu(list header,list1 headerh,int login,int pin,caixa notas)
{
    int op;
    printf("\t\t ::::::::::::::::::::::::::::::::::::::::::: \n");
    printf("\t\t ::                                       :: \n");
    printf("\t\t ::   Escolha uma das seguintes opções    :: \n");
    printf("\t\t ::                                       :: \n");
    printf("\t\t ::::::::::::::::::::::::::::::::::::::::::: \n\n");
    printf(" \t1    -    Criar cliente\t\t");
    printf(" 2    -    Eliminar cliente\n");
    printf(" \t3    -    Listar clientes\t");
    printf(" 4    -    Consultar clientes\n");
    printf(" \t5    -    Desbloquear cliente\t");
    printf(" 6    -    Alterar notas\n");
    printf(" \t7    -    Historico\n\n");
    printf(" 0    -    Sair\n");

    INPUT
    scanf("%d",&op);

    switch (op)
    {
    case 1:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::            Criar utilizador        :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        cria_cliente(header,headerh,login,pin,notas);
        break;
    case 2:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::          Eliminar cliente          :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        elimina_cliente(header,headerh,login,pin,notas);
        break;
    case 3:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::           Listar cliente           :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        lista_clientes(header,headerh,login,pin,notas);
        break;
    case 4:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::         Consultar cliente          :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        consulta_clientes(header,headerh,login,pin,notas);
        break;
    case 5:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::         Desbloquear cliente        :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        desbloqueia_clientes(header,headerh,login,pin,notas);
        break;
    case 6:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::            Alterar notas           :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        alterar_notas(header,headerh,login,pin,notas);
        break;
    case 7:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::              Historico             :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printhist(header,headerh,login,pin,notas);
        break;
    case 0:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::      Caixa Bancário Automatico    :::\n");
        printf("\t\t :::              2009 2010            :::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::::\n");
        printf("\n\n");
        escrever_clientes(header);
        escrever_notas(notas);
        escrever_hist(headerh);
        system("exit");
        break;
    default:
        printf("Escolha apenas das opções indicadas\n");
    }
}

//////////////////////////////////////////////////////////////////////////////////////

// para o cliente verificar a sua conta

void autoconsulta(list header,list1 headerh,int login, int pin,caixa notas)
{
    list l = header->next; // Salta o header //
    int decisao;

    while (l)
    {
        if (l->userid == login)
        {
            printf("\tUSERID %d\t\t\t\t",l->userid);
            printf("PIN: %d\n",l->pin);
            printf("\tMONTANTE: %d\t\t\t\t",l->montante);
            printf("TIPO: %d\n",l->tipo);
            printf("\tESTADO: %d\n\n",l->estado);

            decuser(header,headerh,login,pin,notas);

        }
        l=l->next;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

//alterar o pin do cliente
int cliente_altera(list header,list1 headerh,int login,int pin,caixa notas) // retorna 1 caso alterado, 0 caso contrario
{
    list l = header->next;
    int pinteste;
    int novopin;
    int decisao;

    printf("Introduza o PIN actual\n");
    INPUT
    scanf("%d",&pinteste);

    if (pin==pinteste)
    {
        while (l)
        {
            if (l->userid == login)
            {
                printf("Introduza o novo PIN ( 4 digitos )\n");
                INPUT
                scanf("%d",&novopin);

                l->pin = novopin;

                printf("PIN alterado com sucesso\n");
                hist_cria(headerh,11,login,pinteste);

                decuser(header,headerh,login,novopin,notas);
            }
            l=l->next;
        }
    }
    else
    {
        printf("Dados inválidos\n");
        decuser(header,headerh,login,pin,notas);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


void cliente_deposita(list header, list1 headerh,int login, int pin, caixa notas)
{
    int valor,op,total = 0;
    list l = header->next;
    printf("Indique o valor da nota mais baixa que pretende depositar\n");
    INPUT scanf("%d",&op);

    while (l)
    {
        if (l->userid == login)
        {
            switch (op)
            {
            case 5:
                printf("Quantas notas de 5€?\n");
                INPUT scanf("%d",&valor);
                total = valor*5;
                notas.cinco+=valor;
            case 10:
                printf("Quantas notas de 10€?\n");
                INPUT scanf("%d",&valor);
                total+=valor*10;
                notas.dez+=valor;
            case 20:
                printf("Quantas notas de 20€?\n");
                INPUT scanf("%d",&valor);
                total+=valor*20;
                notas.vinte+=valor;
            case 50:
                printf("Quantas notas de 50€?\n");
                INPUT scanf("%d",&valor);
                total+=valor*50;
                notas.cinquenta+=valor;
                printf("Deposito efectuado com sucesso\n");
                break;
            default:
                printf("Só pode inserir notas de 5€,10€,20€ ou 50€ escolha apenas dentro destas opções\n");
            }
            l->montante+=total;
            printf("O seu saldo é agora %d€\n",l->montante);
        }
        l=l->next;
    }
    hist_cria(headerh,8,login,total);
    escrever_notas(notas);
    escrever_hist(headerh);
    decuser(header,headerh,login,pin,notas);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


void cliente_transfere(list header,list1 headerh,int login, int pin,caixa notas)
{
    int uid,valor = 0;
    list l = header->next;
    list j = header->next;
comeco:
    printf("Indique o utilizador ID a transferir\n");
    INPUT
    scanf("%d",&uid);


    if ((procura_clientes(header,uid)==1) && (login != uid)) //printf("Encontrou\n");
    {
        while (l)
        {
            if (l->userid == login)
            {
                printf("Indique o valor que pretende transferir\n");
                INPUT
                scanf("%d",&valor);
                if (valor<=(l->montante))
                {
                    l->montante-=valor;
                    //printf("%d",l->montante);
                    while (j)
                    {
                        if (j->userid == uid)
                        {
                            j->montante+=valor;
                            printf("Transferidos %d€ para utilizador %d\n",valor,j->userid);
                            //printf("%d\n",l->montante);
                            hist_cria(headerh,10,login,uid);
                        }
                        j=j->next;
                    }
                }
                else
                    printf("Está a tentar transferir um valor maior do que o seu saldo\n");
            }
            l=l->next;
        }
    }
    else if (login == uid)
    {
        printf("Não pode fazer uma transferência para a própria conta\n\n");
        goto comeco;
    }

    else
        printf("Utilizador %d não existe\n",uid);
    LINHA
    decuser(header,headerh,login,pin,notas);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void cliente_levanta(list header,list1 headerh, int login, int pin, caixa notas)
{
    list l = header->next;
    int valor = 0;
    int cinco = 0;
    int dez = 0;
    int vinte = 0;
    int cinquenta = 0;
    do
    {
        printf("Indique o valor que pretende levantar\n");
        INPUT scanf("%d",&valor);
        if (valor>200) printf("O valor máximo a levantar é de 200€\n\n");
        else if (valor<5) printf("O valor minimo a levantar é de 5€\n");
        else if (valor%2!=0) printf("Não existem notas para levantar esse montante\n");
    }
    while (valor>200);

    int total = valor;

    while (l)
    {
        if (l->userid == login)
        {
            if (l->montante>=valor)
            {
                if ((valor%2==0) && (valor>=5) && (notas.cinco>0))
                {
                    valor-= 5;
                    notas.cinco--;
                    cinco = 1;
                }
                if ((valor>=10) && (notas.dez>0))
                {
                    valor-= 10;
                    notas.dez--;
                    dez = 1;
                }
                if ((valor>=20) && (notas.vinte>0))
                {
                    valor-= 20;
                    notas.vinte--;
                    vinte = 1;
                }
                if ((valor>=50) && (notas.cinquenta>0))
                {
                    valor-= 50;
                    notas.cinquenta--;
                    cinquenta = 1;
                }
                while ((valor>=50) && (notas.cinquenta>0))
                {
                    cinquenta++;
                    valor-=50;
                    notas.cinquenta--;
                }
                while ((valor>=20) && (notas.vinte>0))
                {
                    vinte++;
                    valor-=20;
                    notas.vinte--;
                }
                while ((valor>=10) && (notas.dez>0))
                {
                    dez++;
                    valor-=10;
                    notas.dez--;
                }
                while ((valor>=5) && (notas.cinco>0))
                {
                    cinco++;
                    valor-=5;
                    notas.cinco--;
                }
                if (valor!=0)
                {
                    printf("CBA sem notas suficientes!\n");
                    decuser(header,headerh,login,pin,notas);
                }
                else
                {
                    printf("Notas de 5€: %d\nNotas de 10€: %d\nNotas de 20€: %d\nNotas de 50€: %d\n",cinco,dez,vinte,cinquenta);
                    l->montante-=total;
                    printf("Tem agora %d€ na sua conta\n",l->montante);
                    hist_cria(headerh,9,login,total);
                    decuser(header,headerh,login,pin,notas);
                }
            }
        }
        l=l->next;
    }
    escrever_notas(notas);
    escrever_hist(headerh);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

void decuser(list header,list1 headerh,int login,int pin,caixa notas)
{
    int decisao;

    printf("\t0 - Voltar ao menu                1 - Sair\t");
    INPUT
    scanf("%d",&decisao);

    if (decisao == 0)
    {
        system("clear");
        menucliente(header,headerh,login,pin,notas);
    }
    else if (decisao == 1)
    {
        system("exit");
        escrever_clientes(header);
        escrever_notas(notas);
        escrever_hist(headerh);
        system("clear");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void decfunc(list header,list1 headerh,int login,int pin,caixa notas)
{
    int decisao;

    printf("\t0 - Voltar ao menu                1 - Sair\t");
    INPUT
    scanf("%d",&decisao);

    if (decisao == 0)
    {
        system("clear");
        menu(header,headerh,login,pin,notas);
    }
    else if (decisao == 1)
    {
        system("exit");
        escrever_clientes(header);
        escrever_hist(headerh);
        escrever_notas(notas);
        system("clear");
    }
}



/*---------------------------------------------------------------------------*/
/*----------------------------------MENU CLIENTE-----------------------------*/
/*---------------------------------------------------------------------------*/

void menucliente(list header,list1 headerh,int login, int pin,caixa notas)
{
    int op;
    printf("\t\t ::::::::::::::::::::::::::::::::::::::::::: \n");
    printf("\t\t ::                                       :: \n");
    printf("\t\t ::   Escolha uma das seguintes opções    :: \n");
    printf("\t\t ::                                       :: \n");
    printf("\t\t ::::::::::::::::::::::::::::::::::::::::::: \n\n");
    printf(" \t1    -    Consultar Conta Pessoal\t");
    printf(" 2    -    Depositar montante\n");
    printf(" \t3    -    Levantar montante\t\t");
    printf(" 4    -    Transferir montante\n");
    printf(" \t5    -    Alterar Conta Pessoal\t\t");
    printf(" 0    -    Sair\n");

    INPUT
    scanf("%d",&op);

    switch (op)
    {
    case 1:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::       Consultar Conta Pessoal      :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        autoconsulta(header,headerh,login,pin,notas);
        break;
    case 2:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::         Depositar montante         :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        cliente_deposita(header,headerh,login,pin,notas);
        break;
    case 3:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::          Levantantamentos          :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        cliente_levanta(header,headerh,login,pin,notas);
        break;
    case 4:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::           Transferencias           :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        cliente_transfere(header,headerh,login,pin,notas);
        break;
    case 5:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t ::            Alterar Conta           :: \n");
        printf("\t\t ::                                    :: \n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::: \n");
        cliente_altera(header,headerh,login,pin,notas);
        break;
    case 0:
        system("clear");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::      Caixa Bancário Automatico    :::\n");
        printf("\t\t :::              2009 2010            :::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::                                   :::\n");
        printf("\t\t :::::::::::::::::::::::::::::::::::::::::\n");
        printf("\n\n");
        escrever_clientes(header);
        escrever_hist(headerh);
        escrever_notas(notas);
        system("exit");
        break;
    default:
        printf("Escolha apenas das opções indicadas\n");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////

void escrever_clientes(list header)
{
    list l=header->next;
    FILE *f_clientes= fopen("clientes.txt", "w");

    //fprintf (f_clientes, "UserID\t\tPIN\t\tSaldo\t\tTipo\t\tEstado\n");
    while (l)
    {
        fprintf (f_clientes, "%d\t\t%d\t\t%d\t\t%d\t\t%d\n", l->userid,l->pin,l->montante,l->tipo,l->estado);
        //printf("%d %d %d %d %d\n",l->userid,l->pin,l->montante,l->tipo,l->estado);
        l=l->next;
    }
    fclose(f_clientes);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void escrever_notas(caixa notas)
{
    FILE *f_notas = fopen("notas.txt", "w");
    fprintf(f_notas,"%d %d %d %d",notas.cinco,notas.dez,notas.vinte,notas.cinquenta);
    fclose(f_notas);
}

//////////////////////////////////////////////////////////////////////////////////////////////

caixa ler_notas()
{
    int cinco,dez,vinte,cinquenta;
    int ncinco = 10;
    int ndez = 10;
    int nvinte = 10;
    int ncinquenta = 10;

    FILE *f_notas = fopen("notas.txt", "r");
    if (f_notas == NULL)
    {
        FILE *f_notas = fopen("notas.txt", "w");
        fscanf(f_notas,"%d %d %d %d",&ncinco,&ndez,&nvinte,ncinquenta);
        return defaultnotas(ncinco,ndez,nvinte,ncinquenta);
    }
    else
        fscanf(f_notas,"%d %d %d %d", &cinco,&dez,&vinte,&cinquenta);
    return defaultnotas(cinco,dez,vinte,cinquenta);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void ler_clientes(list header)
{
    int i,pin,montante,tipo,estado,userid;

    FILE *f_clientes= fopen("clientes.txt", "r");
//    int res = fscanf(f_clientes, "%d\t\t%d\t\t%d\t\t%d\t\t%d\n", userid,pin,montante,tipo,estado);
    int res = fscanf(f_clientes, "%d\t\t%d\t\t%d\t\t%d\t\t%d\n", &userid,&pin,&montante,&tipo,&estado);
    for (i = 0; res > 0; i++)
    {
        //printf("cria: %d\n", res);
        cria_teste(header,userid,pin,montante,tipo,estado);
        res = fscanf(f_clientes, "%d\t\t%d\t\t%d\t\t%d\t\t%d\n", &userid,&pin,&montante,&tipo,&estado);
    }
    fclose (f_clientes);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void ler_hist(list1 headerh)
{
    int nop,nuid,ndest,i;
    list1 l = headerh->next;

    FILE *f_hist = fopen("historico.txt", "r");
    if (f_hist == NULL)
    {
        FILE *f_hist = fopen("historico.txt", "w");
    }
    else
    {
        int res = fscanf(f_hist, "%d %d %d\n", &nop,&nuid,&ndest);
        for (i = 0; res > 0; i++)
        {
            //printf("cria: %d\n", res);
            hist_cria(headerh,nop,nuid,ndest);
            res = fscanf(f_hist, "%d %d %d\n", &nop,&nuid,&ndest);
        }
        fclose (f_hist);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////

void escrever_hist(list1 headerh)
{
    list1 l=headerh->next;
    FILE *f_hist= fopen("historico.txt", "w");

    //fprintf (f_clientes, "UserID\t\tPIN\t\tSaldo\t\tTipo\t\tEstado\n");
    while (l)
    {
        fprintf (f_hist,"%d %d %d\n",l->operacao,l->opuserid,l->opdestino);
        //printf("%d %d %d %d %d\n",l->userid,l->pin,l->montante,l->tipo,l->estado);
        l=l->next;
    }
    fclose(f_hist);
}





