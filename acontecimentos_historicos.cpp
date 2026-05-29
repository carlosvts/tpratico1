#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct Acontecimento {
    int id;
    string nome;
    string local;
    int ano;
    string paises_envolvidos;
};

Acontecimento* rediensiona_vetor(Acontecimento* vetor, int& tamanho_atual) {
    int tamanho_antigo = tamanho_atual;
    Acontecimento* novo_vetor = new Acontecimento[tamanho_atual + 5];
    // copia os elementos do vetor antigo para o vetor novo
    for (int i = 0; i < tamanho_antigo; i++) {
        novo_vetor[i] = vetor[i];
    }
    // limpa o espaço de memória do vetor antigo
    delete[] vetor;

    tamanho_atual += 5;
    return novo_vetor;
}

int string_para_int(string str) {
    int resposta = 0;
    for (int i = 0; i < str.size(); i++) {
        // pensando no numero 123
        // resposta = 0 * 10 + 1 --> resposta  = 1
        // resposta = 1 * 10 + 2 --> resposta  = 12
        // resposta = 12 * 10 + 3 _-> resposta = 123
        resposta = resposta * 10 + (str[i] - '0');
    }
    return resposta;
}

int main() {
    // capacidade inicial
    int capacidade = 40;
    Acontecimento* acontecimentos = new Acontecimento[capacidade];
    ifstream entrada("acontecimentos_historicos.csv");
    if (not(entrada)) {
        cout << "Erro: não foi possível abrir o arquivo" << endl;
    }
    string campos[5];
    string campo;
    string linha;
    int idx = 0;
    int i = 0;
    int indice_acontecimento = 0;
    bool aspas = false;

    getline(entrada, linha);  // Pula o cabeçalho

    while (getline(entrada, linha)) {
        idx = 0;
        aspas = false;
        i = 0;

        if (indice_acontecimento == capacidade) {
            // rediensiona_vetor aumenta a capacidade por referencia
            acontecimentos = rediensiona_vetor(acontecimentos, capacidade);
        }

        while (linha[i] != '\0') {
            // checa se é uma aspas duplas
            if (linha[i] == '"') {
                aspas = true;
            } else if (linha[i] == ',' and !aspas) {
                campos[idx] = campo;
                campo = "";
                idx++;
            } else {
                campo += linha[i];
            }
            i++;
        }

        // ultimo campo
        campos[idx] = campo;
        acontecimentos[indice_acontecimento].paises_envolvidos = campo;
        campo = "";

        acontecimentos[indice_acontecimento].id = string_para_int(campos[0]);
        acontecimentos[indice_acontecimento].nome = campos[1];
        acontecimentos[indice_acontecimento].local = campos[2];
        acontecimentos[indice_acontecimento].ano = string_para_int(campos[3]);
        acontecimentos[indice_acontecimento].paises_envolvidos = campos[4];

        indice_acontecimento++;
    }
}
