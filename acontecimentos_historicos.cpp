/*
# * Equipe:
# * Carlos Vinicius Teixeira de Souza
# * Alexsandro Ferreira de Carvalho
# * Ana Julia Piva de Oliveira Gurita
# *
# * Tema: Acontecimentos historicos
# * Campos: id, nome, local, ano, paises_envolvidos, removido
# */

#include <fstream>
#include <iostream>
#include <string>

/*
 *  Falta:
 *  Menu principal
 *  Gravar alterações no CSV
 *  Impressão por trecho
 *  Segunda ordenação
 *  Integrar buscas e operações com entrada do usuário
 *
 *  Feito:
 *  Leitura do CSV
 *  Vetor dinâmico com redimensionamento
 *  Inserção e remoção lógica
 *  Busca por nome e por ano
 *  Ordenação por ano
 */

using namespace std;

const int FATOR_AUMENTO_VETOR = 5;
const int QUANTIDADE_INICIAL_VETOR = 40;

struct Acontecimento {
    int id;
    string nome;
    string local;
    int ano;
    string paises_envolvidos;
    bool removido = false;
};

// por cópia
Acontecimento cria_acontecimento(string nome, string local, int ano,
                                 string paises_envolvidos) {
    // Defini que um id -1 significa que o objeto foi criado, mas ainda não está
    // adicionado na base de dados
    return Acontecimento{-1, nome, local, ano, paises_envolvidos};
}

void redimensiona_vetor(Acontecimento*& vetor, int& capacidade,
                        int fator_aumento) {
    int capacidade_antiga = capacidade;
    Acontecimento* novo_vetor = new Acontecimento[capacidade + fator_aumento];
    // copia os elementos do vetor antigo para o vetor novo
    for (int i = 0; i < capacidade_antiga; i++) 
    {
        novo_vetor[i] = vetor[i];
    }
    // limpa o espaço de memória do vetor antigo
    delete[] vetor;

    capacidade += fator_aumento;
    vetor = novo_vetor;
}

void adiciona_acontecimento(Acontecimento*& vetor, int& tamanho_atual,
                            int& capacidade, Acontecimento ac, int& ultimo_id) {
    // aqui redimensiona o vetor somente em um, já que estamos adicionando
    if (tamanho_atual == capacidade) 
    {
        redimensiona_vetor(vetor, capacidade, 1);
    }
    // fazendo "manualmente" para evitar erros de ponteiros da string
    ac.id = ++ultimo_id;
    vetor[tamanho_atual].id = ac.id;
    vetor[tamanho_atual].nome = ac.nome;
    vetor[tamanho_atual].local = ac.local;
    vetor[tamanho_atual].ano = ac.ano;
    vetor[tamanho_atual].paises_envolvidos = ac.paises_envolvidos;
    tamanho_atual++;
}

void remove_acontecimento(Acontecimento*& vetor, int& tamanho_atual,
                          Acontecimento& ac, int& ultimo_id) {
    bool removeu = false;
    // loop pelos acontecimentos
    for (int i = 0; i < tamanho_atual; ++i)
    {
        // para saber qual é o acontecimento, basta checar o seu id
        if (vetor[i].id == ac.id)
        {
            // como a remoção pode ser feita de forma lógica
            // basta colocar a flag = true
            vetor[i].removido = true;
            removeu = true;
        }
    }
    if (!removeu)
    {
        cout << "ID: " << ac.id << "[" << ac.nome
             << "] Não existe na base de dados \n tem certeza que você o "
                "adicionou?\n";
    }
}

int string_para_int(string str) {
    int resposta = 0;
    for (int i = 0; i < (int)str.size(); i++)
    {
        // pensando no numero 123
        // resposta = 0 * 10 + 1 --> resposta  = 1
        // resposta = 1 * 10 + 2 --> resposta  = 12
        // resposta = 12 * 10 + 3 _-> resposta = 123
        resposta = resposta * 10 + (str[i] - '0');
    }
    return resposta;
}

void imprimeVetor(Acontecimento* vet,int meio){
        cout << "Registro #" << meio << '\n';
        cout << "ID: " << vet[meio].id << '\n';
        cout << "Nome: " << vet[meio].nome << '\n';
        cout << "Local: " << vet[meio].local << '\n';
        cout << "Ano: " << vet[meio].ano << '\n';
        cout << "Paises: " << vet[meio].paises_envolvidos << '\n';
        cout << "----------------------------------------\n";

}

void busca_por_nome(Acontecimento* vetor,int tamanho, string nome){
    bool achou = false;
    for (int i = 0; i < tamanho; i++)
    {
        if (vetor[i].nome == nome)
        {
            imprimeVetor(vetor, i);
            achou = true;
        }
    }
     if (!achou)
     {
        cout << "Nao achamos o acontecimento historico digitado!" << endl;
     }
}

//ordenaçao para busca binaria por ano
void ordena_por_ano(Acontecimento*& vetor, int tamanho){
    int j;
    for (int gap = tamanho/2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < tamanho; i++)
        {
          Acontecimento aux = vetor[i];

          j = i;
          while (j >= gap and vetor[j-gap].ano > aux.ano)
          {
            vetor[j] = vetor[j-gap];
            j -= gap;
          }
          vetor[j] = aux;
      }
    }
}

void buscaBinaria_por_data(Acontecimento* vet,int pInicial,int pFinal,int k){
    int meio = (pInicial + pFinal)/2;
    if (vet[meio].ano == k)
    {
        imprimeVetor(vet, meio);
                int olha_esqueda = meio -1;

        while (vet[olha_esqueda].ano == vet[meio].ano){
            imprimeVetor(vet,olha_esqueda);
            olha_esqueda--;
        }

        int olha_direita = meio+1;
        while (vet[olha_direita].ano == vet[meio].ano){
            imprimeVetor(vet,olha_direita);
            olha_direita++;
         }
    }

    if (pFinal > pInicial)
    {
        if (vet[meio].ano > k)
        {
            return buscaBinaria_por_data(vet, pInicial, meio - 1, k);
        }
        else if (vet[meio].ano < k)
        {
            return buscaBinaria_por_data(vet, meio + 1, pFinal, k);
        }
    }
    else
    {
        cout << "Nenhum 'Acontecimento Historico' encontrado na data escolhida!" << endl;
    }
}

void busca_por_intervalo(Acontecimento* vet, int inicio,int final,int tamanho) {

    for(int i=0;i <= tamanho;i++)
        if(vet[i].ano >= inicio and vet[i].ano <= final){
        imprimeVetor(vet,i);
    }
}

int main() {
    // capacidade inicial
    int capacidade = QUANTIDADE_INICIAL_VETOR;
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
    int ultimo_id = 0;
    int tamanho = 0;
    bool aspas = false;

    getline(entrada, linha);  // Pula o cabeçalho

    while (getline(entrada, linha)) {
        idx = 0;
        aspas = false;
        i = 0;

        if (tamanho == capacidade) {
            // redimensiona_vetor aumenta a capacidade por referencia
            redimensiona_vetor(acontecimentos, capacidade, FATOR_AUMENTO_VETOR);
        }

        while (linha[i] != '\0') {
            // checa se é uma aspas duplas
            if (linha[i] == '"') {
                // Precisa ser !aspas e nao aspas = true pois se tudo for
                // true ele sempre ignorará as aspas
                aspas = !aspas;
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
        acontecimentos[tamanho].paises_envolvidos = campo;
        campo = "";

        // preenche vetor
        acontecimentos[tamanho].id = string_para_int(campos[0]);
        acontecimentos[tamanho].nome = campos[1];
        acontecimentos[tamanho].local = campos[2];
        acontecimentos[tamanho].ano = string_para_int(campos[3]);
        acontecimentos[tamanho].paises_envolvidos = campos[4];
        // nao precisa marcar o removido como false pois a struct ja é
        // criada com valor padrão de false
        tamanho++;
    }
    ultimo_id = acontecimentos[tamanho - 1].id;
    // TODO a partir de agora todos os dados estão carregadas na struct

    // print de debug, remover depois
    Acontecimento teste =
        cria_acontecimento("teste1", "local de teste", 2026, "Brasil");
    adiciona_acontecimento(acontecimentos, tamanho, capacidade, teste,
                           ultimo_id);

    Acontecimento teste_remover = cria_acontecimento(
        "Removido??", "Remoção", 2026, "Brasil, México, Removido");
    adiciona_acontecimento(acontecimentos, tamanho, capacidade, teste_remover,
                           ultimo_id);
    remove_acontecimento(acontecimentos, tamanho, teste_remover, ultimo_id);

    // Testando remover um acontecimento sem ter colocado ele na base de dados
    Acontecimento teste2 = cria_acontecimento("Removi222do??", "Remoção2", 2026,
                                              "Brasil, 22México, Removido");

    //teste da ordenaçao por data                                      
    //ordena_por_ano(acontecimentos,tamanho); 

    cout << "===== DEBUG: ACONTECIMENTOS CARREGADOS =====\n";
    for (int i = 0; i < tamanho; i++) {
        cout << "Registro #" << i << '\n';
        cout << "ID: " << acontecimentos[i].id << '\n';
        cout << "Nome: " << acontecimentos[i].nome << '\n';
        cout << "Local: " << acontecimentos[i].local << '\n';
        cout << "Ano: " << acontecimentos[i].ano << '\n';
        cout << "Paises: " << acontecimentos[i].paises_envolvidos << '\n';
        cout << "Removido: " << acontecimentos[i].removido << '\n';
        cout << "----------------------------------------\n";
    }

    cout << "Quantidade carregada: " << tamanho << '\n';
    cout << "Capacidade do vetor: " << capacidade << '\n';

    remove_acontecimento(acontecimentos, tamanho, teste2, ultimo_id);

    cout <<"---------------------------------------------\n";
    cout <<"---------------------------------------------\n";
    cout <<"--------------- vetor ordenado---------------\n";
    ordena_por_ano(acontecimentos,tamanho);
    for (int i = 0; i < tamanho; i++) {
        cout << "Registro #" << i << '\n';
        cout << "ID: " << acontecimentos[i].id << '\n';
        cout << "Nome: " << acontecimentos[i].nome << '\n';
        cout << "Local: " << acontecimentos[i].local << '\n';
        cout << "Ano: " << acontecimentos[i].ano << '\n';
        cout << "Paises: " << acontecimentos[i].paises_envolvidos << '\n';
        cout << "----------------------------------------\n";
    }
    ordena_por_ano(acontecimentos,tamanho);

    /*busca linear por nome
    string procurado;
    getline(cin,procurado);
    busca_por_nome(acontecimentos,tamanho, procurado);
    */
    
        /* Busca por data
    int Ano;
    cin >> Ano;
    
    buscaBinaria_por_data(acontecimentos,0,tamanho-1,Ano);
    */

    /*Teste busca por intervalo 
    cout <<"---------------------------------------------\n";
    cout <<"------------ BUSCA POR INTERVALO---------------\n";
    cout <<"---------------------------------------------\n";
    int AnoI, AnoF;
    cin >> AnoI >> AnoF;
    busca_por_intervalo(acontecimentos,AnoI,AnoF,tamanho);*/
    // Finalizada toda a lógica, limpa o vetor
    delete[] acontecimentos;
    return 0;
}
