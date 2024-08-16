#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include<conio.h> //Biblioteca para usar getch()

#define QUANTIDADE_USUARIOS 50
//Estruturas para armazenas informações do usuário
typedef struct {
    char nome[50];
    char email[80];
    char senha[20];
    float saldo;
} Usuario;

//Estrutura para informações do banco
typedef struct {
    Usuario *usuarios[QUANTIDADE_USUARIOS];
    int totalUsuarios;
} Banco;

//Declarações das Funções 
void acessarMenuCadastrado(Banco *banco, int usuarioID); //Declaração da função menuUsuario
void menuNaoCadastrado(Banco *banco); //Declaração da função menuNaoCadastrado
void menuUsuario(Banco *banco, int usuarioID); //Declaração da função acessarMenuCadastrado

//Função para exibir mensagem de boas-vindas
void exibirBoasVindas(){
    printf(" =================================\n");
    printf(" Bem-vindo ao Banco Digital CDBank\n");
    printf(" =================================\n");
}

//Função para criar um novo banco
Banco* criarBanco(){
    Banco *banco = (Banco*)malloc(sizeof(Banco));
    if(!banco){
        printf("Erro ao alocar memória para o banco!\n");
        exit(1);
    }
    banco -> totalUsuarios = 0;
    return banco;
}

//Função para validar o email
int validarEmail(Banco *banco, const char *email, int indiceUsuarioAtual){
    
    //Verifica se o email contém '@'
    if(strchr(email, '@') == NULL){
        printf("Erro: Email inválido, deve conter '@'!\n");
        return 0;
    }

    //Verifica se o email já está cadastrado, ignorando o próprio email do usuário atual
    for (int i = 0; i < banco -> totalUsuarios; i++){
        if (i != indiceUsuarioAtual && strcmp (banco -> usuarios[i]->email, email) == 0){
            printf("Erro: Email já cadastrado!\n");
            return 0;
        }
    }
    return 1;
}
//Função para capturar senha com asteriscos
void capturarSenha(char *senha){
    char ch;
    int i = 0;
    while(1){
        ch = getch();
        if(ch == 13){ //Enter (fim da senha)
            senha[i] = '\0';
            break;
        } else if(ch == 8){ //Backspace
            if(i > 0){
                i--;
                printf("\b \b");
            }
        } else if(i < 19){
            senha[i++] = ch;
            printf("*");
        }
    }
    printf("\n");
}

//Função para cadastrar um novo usuário
void cadastrarUsuario(Banco *banco){
    if(banco -> totalUsuarios >= QUANTIDADE_USUARIOS){
        printf("Erro: Limite de usuários alcançado!\n");
        return;
    }
    Usuario *novoUsuario = (Usuario*)malloc(sizeof(Usuario));
    if(!novoUsuario){
        printf("Erro ao alocar memória para o novo usuário!\n");
        return;
    }
    novoUsuario->saldo = 0.0f;

    //Solicitar o nome do usuário
    printf("Digite seu nome: ");
    getchar(); //Limpar o buffer do teclado
    fgets(novoUsuario->nome, 50, stdin);
    novoUsuario->nome[strcspn(novoUsuario->nome, "\n")] = 0; //Remover o '\n' do final da string

    //Solicitar e validar o email
    do {
        printf("Digite seu email: ");
        scanf("%s", novoUsuario->email);
    } while (!validarEmail(banco, novoUsuario->email, -1)); //Uso do -1 para indicar que é um novo usuário

    //Solicitar a senha
    printf("Digite sua senha: ");
    capturarSenha(novoUsuario->senha);

    //Adicionar o novo usuário ao banco
    banco ->usuarios[banco->totalUsuarios] = novoUsuario;
    banco -> totalUsuarios++;
    if(!banco -> usuarios){
        printf("Erro ao alocar memória para os usuários!\n");
        exit(1);
    }
    banco -> usuarios[banco -> totalUsuarios - 1] = novoUsuario;
    printf("Usuário cadastrado com sucesso!\n\n");

    acessarMenuCadastrado(banco, banco -> totalUsuarios - 1);
}

//Função para listar todos os usuários cadastrados
void listarUsuarios(Banco *banco){
    printf("Usuários cadastrados:\n");
    for (int i = 0; i < banco -> totalUsuarios; i++){
        printf("%d. Nome: %s | Email: %s | Saldo: R$ %.2f\n", i + 1, banco -> usuarios[i]->nome, banco -> usuarios[i]->email, banco -> usuarios[i]->saldo);
    }
    printf("\n");
}

//Função para editar um usuário
void editarUsuario(Banco *banco){
    int indice;
    listarUsuarios(banco);
    printf("Selecione o número do usuário que deseja editar: ");
    scanf("%d", &indice);
    indice--; //Ajustar para índice baseado em zero
    if(indice >= 0 && indice < banco -> totalUsuarios){
        printf("Digite o novo nome: ");
        getchar(); 
        fgets(banco -> usuarios[indice]->nome, 50, stdin);
        banco -> usuarios[indice]->nome[strcspn(banco -> usuarios[indice]->nome, "\n")] = 0; 

        do{
            printf("Digite o novo email: ");
            scanf("%s", banco -> usuarios[indice]->email);
        } while (!validarEmail(banco, banco -> usuarios[indice]->email, indice));

        printf("Digite a nova senha: ");
        capturarSenha(banco -> usuarios[indice]->senha);
       
        printf("Usuário editado com sucesso!\n\n");
    } else {
        printf("Erro: Índice inválido!\n\n");
    }
}
    
//Função para excluir um usuário
void excluirUsuario(Banco *banco){
    int indice;
    listarUsuarios(banco);
    printf("Selecione o número do usuário que deseja excluir: ");
    scanf("%d", &indice);
    indice--; //Ajutar para o índice baseado em zero
    if(indice >= 0 && indice < banco -> totalUsuarios){
        free(banco -> usuarios[indice]);
        for(int i = indice; i < banco -> totalUsuarios - 1; i++){
            banco -> usuarios[i] = banco -> usuarios[i + 1];
        }
        banco -> totalUsuarios--;
        printf("Usuário excluído com sucesso!\n\n");
    } else {
        printf("Erro: Índice inválido!\n\n");
    }
}

//Função para sair do programa e liberar a memória alocada
void sair(Banco *banco){
    //Liberar a memória alocada para os usuários
    for(int i = 0; i < banco -> totalUsuarios; i++){
        free(banco -> usuarios);
    }
    //Liberar a memória alocada para o banco
    free(banco);
    printf("Obrigado por utilizar o Banco Digital CDBank. Até logo!\n");
    exit(0); //Encerra o programa
}

//Função para realizar login do usuário
int realizarLogin(Banco *banco) {
    char email[79];
    char senha[20];

    for (int tentativas = 3; tentativas > 0; tentativas--){
        printf("Digite seu email: ");
        scanf("%s", email);
        printf("Digite sua senha: ");
        capturarSenha(senha);
        
        for(int i = 0; i < banco -> totalUsuarios; i++){
            if(strcmp(banco -> usuarios[i]->email, email) == 0 && strcmp(banco -> usuarios[i]->senha, senha) == 0){
                printf("Login realizado com sucesso! Bem-vindo, %s!\n\n", banco -> usuarios[i]->nome);
                return i;
            }
        }
        printf("Email ou senha incorretos! Você tem %d tentativa(s) restante(s).\n\n", tentativas -1);
    }
    printf("Login falhou. Retornando ao menu principal.\n");
    return -1;
}

//Função para acessar o menu de um usuário cadastrado
void acessarMenuCadastrado(Banco *banco, int usuarioID){
    menuUsuario(banco, usuarioID);
}

//Função para verificar o saldo
void verificarSaldo(Banco *banco, int usuarioID){
    printf("Seu saldo é: R$ %.2f\n\n", banco->usuarios[usuarioID]->saldo);
}

//Função para realizar depósito
void realizarDeposito(Banco *banco, int usuarioID){
    float valor;
    int resultado;
    
    while(1){
        printf("Digite o valor do depósito: R$ ");
        resultado = scanf("%f", &valor);

        if(resultado == 1 && valor > 0){
            banco -> usuarios[usuarioID]->saldo += valor;
            printf("Depósito de R$ %.2f realizado com sucesso!\n\n", valor);
            break;
        } else{
            printf("Erro: Valor inválido! Insira um valor positivo.\n");
            while(getchar() != '\n');
        }
    }
}
//Função para realizar saque
void realizarSaque(Banco *banco, int usuarioID){
    float valor;
    int resultado;
    do{
    printf("Digite o valor do saque: R$ ");
    resultado = scanf("%f", &valor);
    if(resultado != 1 || valor <= 0){
        printf("Erro: Valor inválido! Insira um valor positivo.\n");
        while(getchar() != '\n');
    }
    } while(resultado != 1 || valor <= 0);
    if(valor <= banco -> usuarios[usuarioID]->saldo){
        banco -> usuarios[usuarioID]->saldo -= valor;
        printf("Saque de R$ %.2f realizado com sucesso!\n\n", valor);
    } else {
        printf("Saldo insuficiente!\n\n");
    }
}

//Função para alterar senha
void alterarSenha(Banco *banco, int usuarioID){
    printf("Digite sua nova senha: ");
    capturarSenha(banco -> usuarios[usuarioID]->senha);
    printf("Senha alterada com sucesso!\n\n");
}

//Função para transferir para outro usuário
void realizarTransferencia(Banco *banco, int usuarioID){
    int destinatarioID;
    float valor;
    int resultado;
    listarUsuarios(banco);
    printf("Selecione o número do usuário para quem deseja transferir: ");
    scanf("%d", &destinatarioID);
    destinatarioID--; 
    if(destinatarioID >= 0 && destinatarioID < banco -> totalUsuarios && destinatarioID != usuarioID){     
        do{
        printf("Digite o valor da transferência: ");
        resultado = scanf("%f", &valor);
            if(resultado != 1 || valor <= 0){
                printf("Erro: Valor inválido! Insira um valor positivo.\n");
                while(getchar() != '\n');
    }
    } while(resultado != 1 || valor <= 0);
    if(valor <= banco -> usuarios[usuarioID]->saldo){
        banco -> usuarios[usuarioID]->saldo -=valor;
        banco -> usuarios[destinatarioID]->saldo += valor;
        printf("Transferência de R$ %.2f realizada com sucesso para %s!\n\n", valor, banco -> usuarios[destinatarioID]->email);
    } else{
        printf("Saldo Insuficiente!\n\n");
        }       
    } else{
        printf("Erro: Usuário inválido!\n\n");
    }
}

//Menu para usuários não cadastrados
void menuNaoCadastrado(Banco *banco){
    int opcao;
    while(1){
        printf("Menu Principal\n");
        printf("1. Cadastrar usuário\n");
        printf("2. Listar usuários\n");
        printf("3.Editar usuário\n");
        printf("4. Excluir usuário\n");
        printf("5. Sair\n");
        printf("\nEscolha uma opção: ");

        while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 5){
            while(getchar() != '\n');
        }

        switch (opcao){     
            case 1: 
                cadastrarUsuario(banco);
                break;
            case 2: 
                listarUsuarios(banco);
                break;
            case 3:
                editarUsuario(banco);
                break;
            case 4: 
                excluirUsuario(banco);
                break;
            case 5: 
                // sair(banco);
                // break;
                return;
            default:
                printf("Opção inválida!\n");
        }
    } 
}


//Menu para usuários cadastrados
void menuUsuario(Banco *banco, int usuarioID){
    int opcao;
    while(1){
        printf("Menu do Usuário\n");
        printf("1. Verificar Saldo\n");
        printf("2. Realizar Depósito\n");
        printf("3. Realizar Saque\n");
        printf("4. Alterar Senha\n");
        printf("5. Realizar Transferências\n");
        printf("6. Sair\n");
        printf("\nEscolha uma opção: ");

        while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 6){
            while(getchar() != '\n');
        }

        switch (opcao){
            case 1:
                verificarSaldo(banco, usuarioID);
                break;
            case 2:
                realizarDeposito(banco, usuarioID);
                break;
            case 3:
                realizarSaque(banco, usuarioID);
                break;
            case 4:
                alterarSenha(banco, usuarioID);
                break;
            case 5:
                realizarTransferencia(banco, usuarioID);
                break;
            case 6:
                //sair(banco);
                return;

            default:
                printf("Opção inválida! Tente novamente.\n\n");
        }
    } 
}

//Função principal
int main(){
    setlocale(LC_ALL, "Portuguese");
    Banco *banco = criarBanco();
    int usuarioID;

    exibirBoasVindas();

    while (1){   
        printf("Você já tem cadastro? (1 - Sim, 2 - Não): ");
        int resposta;
        scanf("%d", &resposta);
        if(resposta == 1){
            usuarioID = realizarLogin(banco);
            if (usuarioID != -1){
                acessarMenuCadastrado(banco, usuarioID);
            } else {
                printf("login falhou. Tente novamente mais tarde.\n");
                // sair(banco);
            }
        } else if(resposta == 2){
            menuNaoCadastrado(banco);
        } else {
            printf("Opção inválida!\n");
            break;
        }
    }
        sair(banco);
    return 0;
}

