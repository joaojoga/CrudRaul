#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "pessoas.dat"
#define MAX_NOME 100
#define MAX_EMAIL 100
#define TAM_CPF 12  

typedef struct {
    char nome[MAX_NOME];
    char cpf[TAM_CPF];
    int idade;
    char email[MAX_EMAIL];
    int ativo;  // 1 = ativo, 0 = removido (remoção lógica)
} Pessoa;

// Protótipos das funções
void menu();
void cadastrar();
void listar();
void buscar();
void atualizar();
void remover();
int cpfExiste(char *cpf);
void limparBuffer();
int validarCPF(char *cpf);

int main() {
    int opcao;
    
    printf("=== Sistema de Cadastro de Pessoas ===\n\n");
    
    do {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch(opcao) {
            case 1:
                cadastrar();
                break;
            case 2:
                listar();
                break;
            case 3:
                buscar();
                break;
            case 4:
                atualizar();
                break;
            case 5:
                remover();
                break;
            case 6:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n\n");
        }
    } while(opcao != 6);
    
    return 0;
}

void menu() {
    printf("\n========== MENU ==========\n");
    printf("1. Cadastrar nova pessoa\n");
    printf("2. Listar todas as pessoas\n");
    printf("3. Buscar pessoa por CPF\n");
    printf("4. Atualizar dados por CPF\n");
    printf("5. Remover pessoa por CPF\n");
    printf("6. Sair\n");
    printf("==========================\n");
}

void cadastrar() {
    Pessoa p;
    FILE *arquivo;
    
    printf("\n=== CADASTRAR NOVA PESSOA ===\n");
    
    printf("CPF (11 digitos): ");
    fgets(p.cpf, TAM_CPF, stdin);
    p.cpf[strcspn(p.cpf, "\n")] = '\0';
    
    if (!validarCPF(p.cpf)) {
        printf("ERRO: CPF deve conter exatamente 11 digitos!\n");
        return;
    }
    
    if (cpfExiste(p.cpf)) {
        printf("ERRO: CPF ja cadastrado!\n");
        return;
    }
    
    printf("Nome: ");
    fgets(p.nome, MAX_NOME, stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';
    
    printf("Idade: ");
    scanf("%d", &p.idade);
    limparBuffer();
    
    printf("E-mail: ");
    fgets(p.email, MAX_EMAIL, stdin);
    p.email[strcspn(p.email, "\n")] = '\0';
    
    p.ativo = 1;
    
    arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return;
    }
    
    fwrite(&p, sizeof(Pessoa), 1, arquivo);
    fclose(arquivo);
    
    printf("\nPessoa cadastrada com sucesso!\n");
}

void listar() {
    Pessoa p;
    FILE *arquivo;
    int contador = 0;
    
    printf("\n=== LISTA DE PESSOAS CADASTRADAS ===\n\n");
    
    arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("Nenhuma pessoa cadastrada ainda.\n");
        return;
    }
    
    while (fread(&p, sizeof(Pessoa), 1, arquivo)) {
        if (p.ativo) {
            printf("CPF: %s\n", p.cpf);
            printf("Nome: %s\n", p.nome);
            printf("Idade: %d anos\n", p.idade);
            printf("E-mail: %s\n", p.email);
            printf("------------------------------------\n");
            contador++;
        }
    }
    
    fclose(arquivo);
    
    if (contador == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
    } else {
        printf("\nTotal: %d pessoa(s) cadastrada(s)\n", contador);
    }
}

void buscar() {
    Pessoa p;
    FILE *arquivo;
    char cpf[TAM_CPF];
    int encontrado = 0;
    
    printf("\n=== BUSCAR PESSOA POR CPF ===\n");
    printf("CPF (11 digitos): ");
    fgets(cpf, TAM_CPF, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    
    if (!validarCPF(cpf)) {
        printf("ERRO: CPF deve conter exatamente 11 digitos!\n");
        return;
    }
    
    arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return;
    }
    
    while (fread(&p, sizeof(Pessoa), 1, arquivo)) {
        if (strcmp(p.cpf, cpf) == 0 && p.ativo) {
            printf("\n--- Pessoa Encontrada ---\n");
            printf("CPF: %s\n", p.cpf);
            printf("Nome: %s\n", p.nome);
            printf("Idade: %d anos\n", p.idade);
            printf("E-mail: %s\n", p.email);
            encontrado = 1;
            break;
        }
    }
    
    fclose(arquivo);
    
    if (!encontrado) {
        printf("\nPessoa nao encontrada!\n");
    }
}

void atualizar() {
    Pessoa p;
    FILE *arquivo;
    char cpf[TAM_CPF];
    int encontrado = 0;
    long pos;
    
    printf("\n=== ATUALIZAR DADOS POR CPF ===\n");
    printf("CPF (11 digitos): ");
    fgets(cpf, TAM_CPF, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    
    if (!validarCPF(cpf)) {
        printf("ERRO: CPF deve conter exatamente 11 digitos!\n");
        return;
    }
    
    arquivo = fopen(ARQUIVO, "r+b");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return;
    }
    
    while (fread(&p, sizeof(Pessoa), 1, arquivo)) {
        if (strcmp(p.cpf, cpf) == 0 && p.ativo) {
            encontrado = 1;
            pos = ftell(arquivo) - sizeof(Pessoa);
            
            printf("\n--- Dados Atuais ---\n");
            printf("Nome: %s\n", p.nome);
            printf("Idade: %d\n", p.idade);
            printf("E-mail: %s\n", p.email);
            
            printf("\n--- Novos Dados ---\n");
            printf("Novo nome: ");
            fgets(p.nome, MAX_NOME, stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0';
            
            printf("Nova idade: ");
            scanf("%d", &p.idade);
            limparBuffer();
            
            printf("Novo e-mail: ");
            fgets(p.email, MAX_EMAIL, stdin);
            p.email[strcspn(p.email, "\n")] = '\0';
            
            fseek(arquivo, pos, SEEK_SET);
            fwrite(&p, sizeof(Pessoa), 1, arquivo);
            
            printf("\nDados atualizados com sucesso!\n");
            break;
        }
    }
    
    fclose(arquivo);
    
    if (!encontrado) {
        printf("\nPessoa nao encontrada!\n");
    }
}

void remover() {
    Pessoa p;
    FILE *arquivo;
    char cpf[TAM_CPF];
    int encontrado = 0;
    long pos;
    char confirma;
    
    printf("\n=== REMOVER PESSOA POR CPF ===\n");
    printf("CPF (11 digitos): ");
    fgets(cpf, TAM_CPF, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    
    if (!validarCPF(cpf)) {
        printf("ERRO: CPF deve conter exatamente 11 digitos!\n");
        return;
    }
    
    arquivo = fopen(ARQUIVO, "r+b");
    if (arquivo == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo!\n");
        return;
    }
    
    while (fread(&p, sizeof(Pessoa), 1, arquivo)) {
        if (strcmp(p.cpf, cpf) == 0 && p.ativo) {
            encontrado = 1;
            pos = ftell(arquivo) - sizeof(Pessoa);
            
            printf("\n--- Pessoa Encontrada ---\n");
            printf("Nome: %s\n", p.nome);
            printf("CPF: %s\n", p.cpf);
            printf("Idade: %d\n", p.idade);
            printf("E-mail: %s\n", p.email);
            
            printf("\nTem certeza que deseja remover? (s/n): ");
            scanf("%c", &confirma);
            limparBuffer();
            
            if (confirma == 's' || confirma == 'S') {
                p.ativo = 0;  // Remoção lógica
                fseek(arquivo, pos, SEEK_SET);
                fwrite(&p, sizeof(Pessoa), 1, arquivo);
                printf("\nPessoa removida com sucesso!\n");
            } else {
                printf("\nRemocao cancelada.\n");
            }
            break;
        }
    }
    
    fclose(arquivo);
    
    if (!encontrado) {
        printf("\nPessoa nao encontrada!\n");
    }
}

int cpfExiste(char *cpf) {
    Pessoa p;
    FILE *arquivo;
    
    arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        return 0;
    }
    
    while (fread(&p, sizeof(Pessoa), 1, arquivo)) {
        if (strcmp(p.cpf, cpf) == 0 && p.ativo) {
            fclose(arquivo);
            return 1;
        }
    }
    
    fclose(arquivo);
    return 0;
}

int validarCPF(char *cpf) {
    if (strlen(cpf) != 11) {
        return 0;
    }
    
    for (int i = 0; i < 11; i++) {
        if (cpf[i] < '0' || cpf[i] > '9') {
            return 0;
        }
    }
    
    return 1;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}
