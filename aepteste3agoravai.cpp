#include <stdio.h> //funcões principais
#include <stdlib.h> //funções principais
#include <locale.h> //deixar ortografia portuguesa correta
#include <conio.h> //função para usar teclas de setas
#include <windows.h> //função para usar som
#include <string.h> //para uso do strlen(comprimento da string)


//-------------------------------------------------------------------------------------------

struct cadastroPessoa { //struct para armazenar as informações do cadastro, todas serão do tipo char para melhor manipulação.
    char nome[30], email[100];
    char senha[30], cpf[16], senhacrip[30];
};

//----------------------------------- FUNÇÕES -----------------------------------------------

//função de criptografia e descriptografia
char* Cripto(char* s);
char* desCripto(char* j);

//função que define a cor do texto.
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

//função que mantem o menu ativo e só muda quando o usuário digitar ENTER.
void pausar() {
    printf("\nPara voltar ao menu principal, por gentileza pressione ENTER!");
    getchar(); 
    system("cls");
}

//função para coletar dados do usuário.
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

//função para salvar as informações do usuário em um arquivo txt.
void salvarInfos(struct cadastroPessoa *pessoa) {
    FILE *file = fopen("cadastro.txt", "a");
    if (file == NULL) {
        printf("ERRO! O ARQUIVO não existe!\n");
        return;
    } else {
        strcpy(pessoa->senha, desCripto(pessoa->senha));
        fprintf(file, "================================\n  NOME: %s\n   CPF: %s\nE-MAIL: %s\n SENHA: %s================================\n", pessoa->nome, pessoa->cpf, pessoa->email,pessoa->senha);
        fclose(file);
        printf("As informações do CADASTRO foram salvas com sucesso!!\n");
        strcpy(pessoa->senha, Cripto(pessoa->senha));
	}
    
    
}

//Função para mostrar os usuários cadastrados.
void listarUsuarios() {
    FILE *file = fopen("cadastro.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    char linha[150];
    setColor(14);
    printf("*                          Usuários cadastrados:                          *\n");
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

//função para editar usuário
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

    printf("Informe o nome do usuário que deseja editar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; 

    while (fscanf(file, "================================\n  NOME: %29[^\n]\n   CPF: %15[^\n]\nE-MAIL: %99[^\n]\n SENHA: %29[^\n]\n", p.nome, p.cpf, p.email, p.senha) == 4) {
        
        if (strcmp(p.nome, nomeBusca) == 0) { 
            encontrado = 1;
            printf("Usuário encontrado! Insira os novos dados:\n");
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
        printf("Usuário atualizado com sucesso!\n");
    } else {
        remove("temp.txt"); 
        printf("Usuário não encontrado.\n");
    }
}

//função para excluir usuário
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

    printf("Informe o nome do usuário que deseja excluir: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0; 

 
    while (fscanf(file, "================================\n  NOME: %29[^\n]\n   CPF: %15[^\n]\nE-MAIL: %99[^\n]\n SENHA: %29[^\n]\n", p.nome, p.cpf, p.email, p.senha) == 4) {
        
        if (strcmp(p.nome, nomeBusca) == 0) { 
            encontrado = 1;
            printf("Usuário '%s' excluído com sucesso!\n", nomeBusca);
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
        printf("Usuário não encontrado.\n");
    }
}

//função para exibir o menu com cores
void menuVisual() {
    struct cadastroPessoa p;
    int opcao = 0;
    int tecla;

    while (1) {
        system("cls"); 
        printf("*   Bem vindo ao sistema de cadastros, por gentileza escolha uma opção:   *\n");
        printf("===========================================================================\n");
        if (opcao == 0) setColor(10); else setColor(7);
        printf("=      Opção 1 - Cadastrar usuário.  - Por gentileza pressione ENTER      =\n");

        if (opcao == 1) setColor(10); else setColor(7);
        printf("=      Opção 2 - Verificar usuários. - Por gentileza pressione ENTER      =\n");

        if (opcao == 2) setColor(10); else setColor(7);
        printf("=      Opção 3 - Editar usuário.     - Por gentileza pressione ENTER      =\n");

        if (opcao == 3) setColor(10); else setColor(7);
        printf("=      Opção 4 - Excluir usuário.    - Por gentileza pressione ENTER      =\n");

        if (opcao == 4) setColor(10); else setColor(7);
        printf("=      Opção 5 - Sair do programa.   - Por gentileza pressione ENTER      =\n");    

        setColor(7); 
        printf("===========================================================================\n");
        printf("Feito por: Edson Jonas - Felipe Corrêa - Fernando Alexandre.");

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
                    printf("*               Você escolheu a opção de CADASTRAR USUÁRIO!               *\n");
                    printf("===========================================================================\n");
                    setColor(7);
                    coletaDados(&p);
                    salvarInfos(&p);
                    setColor(14);
                    printf("===========================================================================\n");
                    printf("*      O cadastro foi finalizado, segue as informações cadastradas:       *\n");
                    printf("                                                                           \n");
                    printf("O NOME cadastrado é:      %s\n", p.nome);
                    printf("O CPF cadastrado é:       %s\n", p.cpf);
                    printf("O E-MAIL cadastrado é:    %s\n", p.email);
                    printf("A SENHA cadastrada é:     %s\n", p.senha);
                    printf("===========================================================================\n");
                    setColor(7);
                    pausar();
                    break;

                case 1:
                    setColor(14);
                    printf("*                Você escolheu a opção de LISTAR USUÁRIOS!                *\n");
                    printf("                                                                           \n");
                    setColor(7);
                    listarUsuarios();
                    pausar();
                    break;

                case 2:
                    setColor(14);
                    printf("*                Você escolheu a opção de EDITAR USUÁRIO!                *\n");
                    printf("===========================================================================\n");
                    setColor(7);
                    editarUsuario();
                    pausar();
                    break;

                case 3:
                    setColor(14);
                    printf("*                Você escolheu a opção de EXCLUIR USUÁRIO!               *\n");
                    printf("===========================================================================\n");
                    setColor(7);
                    excluirUsuario();
                    pausar();
                    break;

                case 4:
                    setColor(12);
                    printf("O programa será ENCERRADO! Obrigado pela utilização! \n");
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
    tmp1[x] = (s[x] + 3); //soma a posição do caracter ao numero ascii
   }
   tmp1[x] = '\0';
   
   return (tmp1);
}
//-----------------------------------------
char* desCripto(char* j){
  char tmp[30];
  int x;

   for(x=0;x < strlen(j) ;x++){
    tmp[x] = (j[x] - 3); //soma a posição do caracter ao numero ascii
   }
   tmp[x] = '\0';
   
   return (tmp);
}

//main executando somente as funções já definidas anteriormente.
int main(void) {
    setlocale(LC_ALL, "Portuguese"); 
    menuVisual();
    return 0;
}

