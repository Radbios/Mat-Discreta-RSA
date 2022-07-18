#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
//Calcular módulo de dois números  >>>>>   a % b
long long int mod(long long int a, long long int b)
{
    long long int r = a % b;
    if ((r < 0) && (b > 0)){
        return (b + r);
    }
    if ((r > 0) && (b < 0)){
        return (b + r);
    }
    return (r);
}
//Algoritmo de euclides estendido para calcular o inverso de um número >>>>>>  
//                 >>>>mdc(primo1, primo2) = s . primo1 + t . primo 2
unsigned long long int algoritmo_euclides(unsigned long long int a, unsigned long long int b, unsigned long long int c)
{
    unsigned long long int r = mod(b, a);
    if (r == 0){
        return (mod((c / a), (b / a)));
    }
    return ((algoritmo_euclides(r, a, -c) * b + c) / (mod(a, b)));
}
//Calcular o módulo do tipo : (N ^ K mod P)
unsigned long long int exponenciacao_modular(unsigned long long int base, unsigned long long int expoente, unsigned long long int divisao, unsigned long long int resultado, unsigned long long int mod){
    if(expoente>0){
        if (expoente % 2 == 1){
            resultado = (resultado * mod) % divisao;
        }
        mod = (mod * mod) % divisao;
        exponenciacao_modular(base, expoente/2, divisao, resultado, mod);
    }
    else return resultado;
}
void encriptar(){
    char texto[255], k;
    unsigned long long int numero[255], n, e;
    FILE *mensagem_criptografada;

    //Caracteres e tamanho da array para criptografar
    printf("Digite a mensagem a ser encriptada:\n");
    scanf(" %[^\n]", texto);
    unsigned long long int  i = strlen(texto);

    //Chave pública
    printf("Digite a chave pública (N e E) recebida pelo arquivo:\n");
    scanf("%llu%llu", &n, &e);
    //Transformar caractere em inteiros e abrindo arquivo
    mensagem_criptografada = fopen("encrypted_message.txt", "w"); 
    for (int j=0; j<i; j++){
        
        k = tolower(texto[j]);
        numero[j] = k - 95;
        if (texto[j] == ' '){
            numero[j] = 28;
        }

        //Criptografar caracteres
        unsigned long long int u, texto_criptografado[255];
        texto_criptografado[j] = exponenciacao_modular(numero[j], e, n, 1, numero[j]%n);

        //Salvando os caracteres criptografado no arquivo
        if(j<i-1) fprintf(mensagem_criptografada, "%llu\n" ,texto_criptografado[j]);
        else fprintf(mensagem_criptografada, "%llu" ,texto_criptografado[j]);
    }
    printf("Sua mensagem encriptada foi salva no arquivo txt!\n");
    fclose(mensagem_criptografada);
}

void desencriptar()
{

    unsigned long long int p, q, e, i=0, x;

    printf("insira novamente os numeros primos digitados na geração da chave publica\n");
    scanf("%llu%llu", &p, &q);
    printf("agora, insira novamente o expoente digitado na geração da chave publica\n");
    scanf("%llu", &e);

    long long int d;
    unsigned long long int n, n2;
    n = p * q;
    n2 =(p-1)*(q-1);
    //Achar o inverso de e mod n2
    d = algoritmo_euclides(e, n2, 1);

    unsigned long long int texto_criptografado;
    FILE *mensagem_criptografada, *mensagem_descriptografada;

    //Abrir o arquivo da mensagem criptografada (PARA LER)
    mensagem_criptografada = fopen("encrypted_message.txt", "r");

    //Abrir o arquivo da mensagem descriptografada (PARA ESCREVER)
    mensagem_descriptografada = fopen("desencrypted_message.txt", "w");
    while (!feof(mensagem_criptografada))
        {
                fscanf(mensagem_criptografada,"%llu", &texto_criptografado);
                int h = exponenciacao_modular(texto_criptografado, d, n, 1, texto_criptografado%n);
                if(h != 28){
                    h = h + 95;
                }
                else h = h +4;
                fprintf(mensagem_descriptografada,"%c", h);

        }
    fclose(mensagem_descriptografada);
    fclose(mensagem_criptografada);
    


   
}
// Tratamento de erro (número primo)
int Primo(int q) {
    int c = 2;
    int ehprimo = 1;

    if (c <= 1) {ehprimo = 0;}
        
    while (ehprimo == 1 && c <= q / 2)
    {
        if (q % c == 0)
        {ehprimo = 0;}
        c++;
    }

    if (ehprimo == 0) return 0;
    else return 1;
}

void gerar_chave(){
    FILE *chave_publica;
    unsigned long long int p, q, n, e, d, z;

    //Inserir P, Q e E

    printf("Você escolheu gerar uma chave pública...\n");
    printf("Por favor, digite um par de números primos:\n");
    scanf("%llu%llu", &p, &q);
    // Verificar se p e q são primos, caso não sejam digitar novamente
    while ((Primo(p) == 0) || (Primo(q)) == 0)
    {
        if ((Primo(p) == 0) && Primo(q) == 0)
        {
            printf("%llu e %llu não são primos\nPor favor, digite um par de números primos: ", p, q);
            scanf("%llu%llu", &p, &q);
        }
        if (Primo(p) == 0)
        {
            printf("%llu não é um número primo\nPor favor, digite um número primo: ", p);
            scanf("%llu", &p);
        }
        if (Primo(q) == 0)
        {
            printf("%llu não é um número primo\nPor favor, digite um número primo: ", q);
            scanf("%llu", &q);
        }
    }
    n = p * q;
    z = (p-1)*(q-1);
    printf("Agora digite um expoente relativamente primo a %llu:\n", z);
    scanf("%llu", &e);

    // Abrir/Criar arquivo txt

    chave_publica = fopen("public_key.txt", "w"); 
    fprintf(chave_publica, "Sua chave pública: (%llu, %llu)", n, e);
    fclose(chave_publica);
}

int chamarMenu() {
    printf("\nEscolha uma das opções seguintes:\n1 - Gerar chave pública\n2 - Encriptar\n3 - Desencriptar\n4 - Fechar Programa\n");
    
    int opcao;
    scanf("%d", &opcao);

    while(opcao != 1 && opcao !=2 && opcao !=3 && opcao != 4){
        printf("Escolha inválida, tente novamente!\n");
        scanf("%d", &opcao);
    }

    if(opcao == 1){
        gerar_chave();
        chamarMenu();
    }
    if(opcao == 2){
        encriptar();
        chamarMenu();}
    if(opcao == 3)
    {
        desencriptar();
        return 0;
    }
}

int main(){
    printf("Bem vindo ao projeto de Criptografia RSA!\n");
    chamarMenu(); 
    
    return 0;
}