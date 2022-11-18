#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

void cadastro();
void consulta();
void alteracao();
void exclusao_logica();
void exclusao_fisica();
void ordena_nome_crescente();
int busca_codigo();
int recuperacao_backup();

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
        printf("\n1_ Cadastro De Clube");
        printf("\n2_ Consultar Clubes");
        printf("\n3_ Alteracao");
        printf("\n4_ Exclusao Logica");
        printf("\n5_ Exclusao Fisica");
        printf("\n6_ Ordenacao");
        printf("\n7_ Busca Codigo");
        printf("\n9_ Sair\n");
        //scanf("%c",&op);
        op= getche();

        switch (op){
            case '1': cadastro();
                system("CLS");
            break;

            case '2': consulta();
            break;

            case '3': alteracao();
                system("CLS");
            break;

            case '4': exclusao_logica();
                system("CLS");
            break;

            case '5': exclusao_fisica();
                system("CLS");
            break;

            case '6': ordena_nome_crescente();
                system("CLS");
            break;

            case '7': busca_codigo();
            break;

            case '8': recuperacao_backup();
                system("CLS");
            break;

            case '9': printf("\nSaindo...");
            break;

            default : printf("\nOpcao %c invalida", op);

        }
    }while(op!= '9');


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
}

void exclusao_logica(){
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
                F.E = 1;
                fseek(fptr, ftell(fptr)-sizeof(F),0);
                fwrite(&F,sizeof(F), 1, fptr);
                fseek(fptr, 0, 2);
            }
        }
    }
}

void exclusao_fisica(){
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

void ordena_nome_crescente(){
    int i, j, n;
    struct clubes Fi, Fj;

    if((fptr = fopen("Clubes.Bi","rb+"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    fseek(fptr, 0, 2);
    n = ftell(fptr) / sizeof(F);

    for(i = 0; i<n; i++){
        for(j= i+1; j<n; j++){
            fseek(fptr, i*sizeof(Fi), 0);
            fread(&Fi, sizeof(Fi), 1, fptr);
            fseek(fptr, j*sizeof(F), 0);
            fread(&Fj, sizeof(F), 1, fptr);
            if(strcmp(Fi.nome, Fj.nome)>0){
                fseek(fptr, i*sizeof(F), 0);
                fwrite(&Fj, sizeof(F), 1, fptr);
                fseek(fptr, j*sizeof(F), 0);
                fwrite(&Fi,sizeof(F), 1, fptr);
            }
        }
    }
    fclose(fptr);
}

int busca_codigo(){
    int low, mid, high, n, elem;

    if((fptr = fopen("Clubes.Bi","rb"))==NULL){
        printf("\nERRO");
        exit(1);
    }
    printf("\nEntre com o elemento a ser encontrado: ");
    scanf("%d",&elem);
    fseek(fptr, 0, 2);
    n = ftell(fptr)/sizeof(F);

    low = 0;
    high = n-1;
    while(low < high){
        mid = (low + high)/2;
        fseek(fptr, mid*sizeof(F), 0);
        fread(&F, sizeof(F), 1, fptr);
        if(F.codigo > elem){
            high = mid-1;
        }else if(F.codigo < elem){
            low = mid+1;
        }else{
            fclose(fptr);
            return mid;
        }
    }
    fclose(fptr);
    return -1;
}

int recuperacao_backup(){
    printf("\nFuncao em desenvolvimento.");
    return 1;
}
