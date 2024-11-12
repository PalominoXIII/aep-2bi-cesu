#include <stdio.h> //func�es principais
#include <stdlib.h> //fun��es principais
#include <locale.h> //deixar ortografia portuguesa correta
#include <conio.h> //fun��o para usar teclas de setas
#include <windows.h> //fun��o para usar som
#include <string.h> //para uso do strlen(comprimento da string)


//-------------------------------------------------------------------------------------------

struct cadastroPessoa { //struct para armazenar as informa��es do cadastro, todas ser�o do tipo char para melhor manipula��o.
    char nome[30], email[100];
    char senha[30], cpf[16], senhacrip[30];
};

//----------------------------------- FUN��ES -----------------------------------------------

//fun��o de criptografia e descriptografia
char* Cripto(char* s);
char* desCripto(char* j);

//fun��o que define a cor do texto.
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

//fun��o que mantem o menu ativo e s� muda quando o usu�rio digitar ENTER.
void pausar() {
    printf("\nPara voltar ao menu principal, por gentileza pressione ENTER!");
    getchar(); 
    system("cls");
}

//fun��o para coletar dados do usu�rio.
void coletaDados(struct cadastroPessoa *pessoa) {
    printf("* Por gentileza informe o seu NOME: \n");
    fgets(pessoa->nome, 30, stdin);
    printf("* Por gentileza informe o seu CPF: \n");
    fgets(pessoa->cpf, 16, stdin);
    printf("* Por gentileza informe o seu E-MAIL: \n");
    fgets(pessoa->email, 100, stdin);
    printf("* Por gentileza informe a sua SENHA: \n");
    fgets(pessoa->senha, 30, stdin);
    strcpy(pessoa->senhacrip, Cripto(pessoa->senha));
}

//fun��o para salvar as informa��es do usu�rio em um arquivo txt.
void salvarInfos(struct cadastroPessoa *pessoa) {
    FILE *file = fopen("cadastro.txt", "a");
    if (file == NULL) {
        printf("ERRO! O ARQUIVO n�o existe!\n");
        return;
    } else {
        strcpy(pessoa->senha, desCripto(pessoa->senha));
        fprintf(file, "================================\n  NOME: %s\n   CPF: %s\nE-MAIL: %s\n SENHA: %s================================\n", pessoa->nome, pessoa->cpf, pessoa->email,pessoa->senha);
        fclose(file);
        printf("As informa��es do CADASTRO foram salvas com sucesso!!\n");
        strcpy(pessoa->senha, Cripto(pessoa->senha));
	}
    
    
}

//Fun��o para mostrar os usu�rios cadastrados.
void listarUsuarios() {
    FILE *file = fopen("cadastro.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    char linha[150];
    setColor(14);
    printf("*                          Usu�rios cadastrados:                          *\n");
    printf("===========================================================================\n");
    printf("                                                                           \n");
    setColor(7);
    
    while (fgets(linha, sizeof(linha), file) != NULL) {
        printf("%s", linha);
    }
    
    setColor(14);
    printf("===========================================================================\n");
    fclose(file);
}

//fun��o para editar usu�rio
void editarUsuario() {
    FILE *file = fopen("cadastro.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct cadastroPessoa p;
    char nomeBusca[30];
    int encontrado = 0;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Informe o nome do usu�rio que deseja editar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; 

    while (fscanf(file, "================================\n  NOME: %29[^\n]\n   CPF: %15[^\n]\nE-MAIL: %99[^\n]\n SENHA: %29[^\n]\n", p.nome, p.cpf, p.email, p.senha) == 4) {
        
        if (strcmp(p.nome, nomeBusca) == 0) { 
            encontrado = 1;
            printf("Usu�rio encontrado! Insira os novos dados:\n");
            printf("Novo nome: ");
            fgets(p.nome, 30, stdin);
            p.nome[strcspn(p.nome, "\n")] = 0; 
            printf("Novo CPF: ");
            fgets(p.cpf, 16, stdin);
            p.cpf[strcspn(p.cpf, "\n")] = 0;
            printf("Novo e-mail: ");
            fgets(p.email, 100, stdin);
            p.email[strcspn(p.email, "\n")] = 0;
            printf("Nova senha: ");
            fgets(p.senha, 30, stdin);
            p.senha[strcspn(p.senha, "\n")] = 0;
        }

        fprintf(tempFile, "================================\n  NOME: %s\n   CPF: %s\nE-MAIL: %s\n SENHA: %s\n", p.nome, p.cpf, p.email, p.senha);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("cadastro.txt");  
        rename("temp.txt", "cadastro.txt");
        printf("Usu�rio atualizado com sucesso!\n");
    } else {
        remove("temp.txt"); 
        printf("Usu�rio n�o encontrado.\n");
    }
}

//fun��o para excluir usu�rio
void excluirUsuario() {
    FILE *file = fopen("cadastro.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct cadastroPessoa p;
    char nomeBusca[30];
    int encontrado = 0;

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Informe o nome do usu�rio que deseja excluir: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; 

 
    while (fscanf(file, "================================\n  NOME: %29[^\n]\n   CPF: %15[^\n]\nE-MAIL: %99[^\n]\n SENHA: %29[^\n]\n", p.nome, p.cpf, p.email, p.senha) == 4) {
        
        if (strcmp(p.nome, nomeBusca) == 0) { 
            encontrado = 1;
            printf("Usu�rio '%s' exclu�do com sucesso!\n", nomeBusca);
            continue; 
        }

     
        fprintf(tempFile, "================================\n  NOME: %s\n   CPF: %s\nE-MAIL: %s\n SENHA: %s\n", p.nome, p.cpf, p.email, p.senha);
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("cadastro.txt");        
        rename("temp.txt", "cadastro.txt"); 
    } else {
        remove("temp.txt"); 
        printf("Usu�rio n�o encontrado.\n");
    }
}

//fun��o para exibir o menu com cores
void menuVisual() {
    struct cadastroPessoa p;
    int opcao = 0;
    int tecla;

    while (1) {
        system("cls"); 
        printf("*   Bem vindo ao sistema de cadastros, por gentileza escolha uma op��o:   *\n");
        printf("===========================================================================\n");
        if (opcao == 0) setColor(10); else setColor(7);
        printf("=      Op��o 1 - Cadastrar usu�rio.  - Por gentileza pressione ENTER      =\n");

        if (opcao == 1) setColor(10); else setColor(7);
        printf("=      Op��o 2 - Verificar usu�rios. - Por gentileza pressione ENTER      =\n");

        if (opcao == 2) setColor(10); else setColor(7);
        printf("=      Op��o 3 - Editar usu�rio.     - Por gentileza pressione ENTER      =\n");

        if (opcao == 3) setColor(10); else setColor(7);
        printf("=      Op��o 4 - Excluir usu�rio.    - Por gentileza pressione ENTER      =\n");

        if (opcao == 4) setColor(10); else setColor(7);
        printf("=      Op��o 5 - Sair do programa.   - Por gentileza pressione ENTER      =\n");    

        setColor(7); 
        printf("===========================================================================\n");
        printf("Feito por: Edson Jonas - Felipe Corr�a - Fernando Alexandre.");

        tecla = getch();

        if (tecla == 224) { 
            tecla = getch();
            if (tecla == 72 && opcao > 0) { 
                opcao--;
            } else if (tecla == 80 && opcao < 4) {
                opcao++;
            }
        }

        if (tecla == 13) { 
            system("cls");
            switch (opcao) {
                case 0:
                    setColor(14);
                    printf("*               Voc� escolheu a op��o de CADASTRAR USU�RIO!               *\n");
                    printf("===========================================================================\n");
                    setColor(7);
                    coletaDados(&p);
                    salvarInfos(&p);
                    setColor(14);
                    printf("===========================================================================\n");
                    printf("*      O cadastro foi finalizado, segue as informa��es cadastradas:       *\n");
                    printf("                                                                           \n");
                    printf("O NOME cadastrado �:      %s\n", p.nome);
                    printf("O CPF cadastrado �:       %s\n", p.cpf);
                    printf("O E-MAIL cadastrado �:    %s\n", p.email);
                    printf("A SENHA cadastrada �:     %s\n", p.senha);
                    printf("===========================================================================\n");
                    setColor(7);
                    pausar();
                    break;

                case 1:
                    setColor(14);
                    printf("*                Voc� escolheu a op��o de LISTAR USU�RIOS!                *\n");
                    printf("                                                                           \n");
                    setColor(7);
                    listarUsuarios();
                    pausar();
                    break;

                case 2:
                    setColor(14);
                    printf("*                Voc� escolheu a op��o de EDITAR USU�RIO!                *\n");
                    printf("===========================================================================\n");
                    setColor(7);
                    editarUsuario();
                    pausar();
                    break;

                case 3:
                    setColor(14);
                    printf("*                Voc� escolheu a op��o de EXCLUIR USU�RIO!               *\n");
                    printf("===========================================================================\n");
                    setColor(7);
                    excluirUsuario();
                    pausar();
                    break;

                case 4:
                    setColor(12);
                    printf("O programa ser� ENCERRADO! Obrigado pela utiliza��o! \n");
                    setColor(7);
                    return;
            }
        }
    }
}
//----------------------------------------
char* Cripto(char* s){
  char tmp1[30];
  int x;

   for(x=0;x < strlen(s) ;x++){
    tmp1[x] = (s[x] + 3); //soma a posi��o do caracter ao numero ascii
   }
   tmp1[x] = '\0';
   
   return (tmp1);
}
//-----------------------------------------
char* desCripto(char* j){
  char tmp[30];
  int x;

   for(x=0;x < strlen(j) ;x++){
    tmp[x] = (j[x] - 3); //soma a posi��o do caracter ao numero ascii
   }
   tmp[x] = '\0';
   
   return (tmp);
}

//main executando somente as fun��es j� definidas anteriormente.
int main(void) {
    setlocale(LC_ALL, "Portuguese"); 
    menuVisual();
    return 0;
}

