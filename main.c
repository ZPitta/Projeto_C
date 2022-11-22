#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

void cadastro();
void consulta();
void consultaDespesa();
void alteracao();
void exclusao_logica();
void exclusao_fisica();
void ordenaCodigoDecrescente();
void buscaNome();
void recuperacao_backup();

struct clubes{
    int codigo;
    char nome[25];
    int totalFuncionarios;
    float despesas;
    int E;
}F;

FILE * fptr;

int main(){
    char op;

    do{
        system("CLS");
        printf("\n1_ Cadastro De Clube");
        printf("\n2_ Consultar Clubes");
        printf("\n3_ Consultar Clubes com Despesas maior que 1 milhao");
        printf("\n4_ Alteracao de despesas");
        printf("\n5_ Exclusao Logica");
        printf("\n6_ Exclusao Fisica");
        printf("\n7_ Ordenacao Pelo Codigo em Ordem Decrescente");
        printf("\n8_ Busca Pelo Nome");
        printf("\n9_ Recuperar Backup");
        printf("\n0_ Sair\n");
        //scanf("%c",&op);
        op= getche();

        switch (op){
            case '1': cadastro();
            break;

            case '2': consulta();
            break;

            case '3': consultaDespesa();
            break;

            case '4': alteracao();
            break;

            case '5': exclusao_logica();
            break;

            case '6': exclusao_fisica();
            break;

            case '7': ordenaCodigoDecrescente();
            break;

            case '8': buscaNome();
            break;

            case '9': recuperacao_backup();
            break;

            case '0': printf("\nSaindo...");
            break;

            default : printf("\nOpcao %c invalida", op);

        }
    }while(op!= '0');


return 0;
}

void cadastro(){
    if((fptr = fopen("Clubes.Bi","rb+")) == NULL)
    {
        if((fptr = fopen("Clubes.Bi","wb"))==NULL){
            printf("\nErro");
            exit(1);
        }
    }
    printf("\nCodigo: ");
    scanf("%d",&F.codigo);

    printf("\nNome do Clube: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]", &F.nome);

    printf("\nNumero de Funcionarios: ");
    scanf("%d",&F.totalFuncionarios);

    printf("\nDespesas: ");
    scanf("%f",&F.despesas);

    F.E = 0;

    fseek(fptr, 0, 2);
    fwrite(&F, sizeof(F), 1, fptr);
    fclose(fptr);

}

void consulta(){
    if((fptr = fopen("Clubes.Bi","rb"))==NULL){
        printf("\nErro");
        exit(1);
    }
    fseek(fptr, 0, 0);
    while(fread(&F, sizeof(F), 1, fptr)){
        if(F.E == 0){
            printf("\nCodigo: %d", F.codigo);
            printf("\nNome do Clube: %s",F.nome);
            printf("\nNumero de Funcionarios: %d", F.totalFuncionarios);
            printf("\nDespesas: %.2f", F.despesas);
        }

    }
    getch();
fclose(fptr);
}

void consultaDespesa(){
    if((fptr = fopen("Clubes.Bi","rb"))==NULL){
        printf("\nErro");
        exit(1);
    }
    fseek(fptr, 0, 0);
    while(fread(&F, sizeof(F), 1, fptr)){
        if(F.E == 0){
            if(F.despesas > 1000000){
                printf("\nCodigo: %d", F.codigo);
                printf("\nNome do Clube: %s", F.nome);
                printf("\nNumero de Funcionarios: %d", F.totalFuncionarios);
                printf("\nDespesas: %.2f", F.despesas);
            }
        }
    }
    getch();
    fclose(fptr);
}

void alteracao(){
    char aux[25];
    if((fptr = fopen("Clubes.Bi","rb+"))==NULL){
        printf("\nErro");
        exit(1);
    }
    printf("\nEntre com o nome do clube a ser encontrado: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]",aux);
    fseek(fptr, 0, 0);
    while(fread(&F, sizeof(F), 1, fptr)){
        if(F.E == 0){
            if(strcmp(F.nome, aux) == 0){
                printf("\nEntre com a nova despesa: ");
                scanf("%f",&F.despesas);
                fseek(fptr, ftell(fptr)-sizeof(F),0);
                fwrite(&F,sizeof(F), 1, fptr);
                fseek(fptr, 0, 2);
            }
        }
    }
    fclose(fptr);
}

void exclusao_logica(){
    char aux[25];
    if((fptr = fopen("Clubes.Bi","rb+"))==NULL){
        printf("\nErro");
        exit(1);
    }else{
        printf("\nEntre com o nome do clube a ser encontrado: ");
        setbuf(stdin, NULL);
        scanf("%[^\n]",aux);
        fseek(fptr, 0, 0);
        while(fread(&F, sizeof(F), 1, fptr)){
            if(F.E == 0){
                if(strcmp(F.nome, aux) == 0){
                    F.E = 1;
                    fseek(fptr, ftell(fptr)-sizeof(F),0);
                    fwrite(&F,sizeof(F), 1, fptr);
                    fseek(fptr, 0, 2);
                }
            }
        }
    }
    fclose(fptr);
}

void exclusao_fisica(){ //nn sei se ta funcionando
    FILE * fptraux, * fptrback;

    //1
    if((fptr = fopen("Clubes.Bi","rb"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    if((fptraux = fopen("auxiliar.Bi","wb"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    if((fptrback = fopen("back.Bi","rb+"))==NULL){
        if((fptrback = fopen("back.Bi","wb"))==NULL){
            printf("\nERRO");
            exit(1);
        }
    }
    fseek(fptr, 0, 0);

    //2
    while(fread(&F, sizeof(F), 1, fptr)){
        if(F.E==0){
            fseek(fptraux, 0, 2);
            fwrite(&F, sizeof(F), 1,fptraux);
        }else{
            fseek(fptrback, 0, 2);
            fwrite(&F, sizeof(F), 1, fptrback);
        }
    }
    //3
    fclose(fptr);
    fclose(fptraux);
    fclose(fptrback);

    //4
    remove("Clubes.Bi");

    //5
    rename("auxiliar.Bi","Clubes.Bi");
}

void ordenaCodigoDecrescente(){
    int i, j, n;
    struct clubes Fi, Fj;

    if((fptr = fopen("Clubes.Bi","rb+"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    fseek(fptr, 0, 2);
    n = ftell(fptr) / sizeof(F);

    for(i = 0; i < n; i++){
        for(j = i + 1; j < n; j++){
            fseek(fptr, i*sizeof(Fi), 0);
            fread(&Fi, sizeof(Fi), 1, fptr);
            fseek(fptr, j*sizeof(F), 0);
            fread(&Fj, sizeof(F), 1, fptr);
            if(Fi.codigo < Fj.codigo){
                fseek(fptr, i*sizeof(F), 0);
                fwrite(&Fj, sizeof(F), 1, fptr);
                fseek(fptr, j*sizeof(F), 0);
                fwrite(&Fi,sizeof(F), 1, fptr);
            }
        }
    }
    fclose(fptr);
}

void buscaNome(){    //nn esta dando certo
    char aux[25];

    if((fptr = fopen("Clubes.Bi","rb"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    printf("\nEntre com o nome a ser encontrado: ");
    scanf("%[^\n]",aux);
    fseek(fptr, 0, 0);

    while(fread(&F, sizeof(F), 1, fptr)){
        if(F.nome == aux){
            printf("\nCodigo: %d", F.codigo);
            printf("\nNome do Clube: %s", F.nome);
            printf("\nNumero de Funcionarios: %d", F.totalFuncionarios);
            printf("\nDespesas: %.2f", F.despesas);
        }
    }

    getch();
    fclose(fptr);
}

void recuperacao_backup(){ //nn esta dando certo
    FILE * fptraux, * fptrback;

    //1
    if((fptr = fopen("Clubes.Bi","rb"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    if((fptraux = fopen("auxiliar.Bi","wb"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    if((fptrback = fopen("back.Bi","rb+"))==NULL){
        if((fptrback = fopen("back.Bi","wb"))==NULL){
            printf("\nERRO");
            exit(1);
        }
    }
    fseek(fptr, 0, 0);

    //2
    while(fread(&F, sizeof(F), 1, fptr)){
        if(F.E == 1){
            fseek(fptrback, 0, 2);
            fwrite(&F, sizeof(F), 0,fptrback);
        }else{
            fseek(fptrback, 0, 2);
            fwrite(&F, sizeof(F), 0, fptraux);
        }
    }
    //3
    fclose(fptr);
    fclose(fptraux);
    fclose(fptrback);

    //4
    remove("Clubes.Bi");
    //5
    rename("auxiliar.Bi","Clubes.Bi");
    //printf("\nFuncao em desenvolvimento.");
}
