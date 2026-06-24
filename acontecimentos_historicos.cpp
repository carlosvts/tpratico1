/*
# * Equipe:
# * Carlos Vinicius Teixeira de Souza
# * Alexsandro Ferreira de Carvalho
# * Ana Julia Piva de Oliveira Gurita
# *
# * Tema: Acontecimentos historicos
# * Campos: id, nome, local, ano, paises_envolvidos, removido
# * Turma: 10A
# */

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

const int FATOR_AUMENTO_VETOR = 5;
const int QUANTIDADE_INICIAL_VETOR = 40;

struct Acontecimento
{
    int id;
    string nome;
    string local;
    int ano;
    string paises_envolvidos;
    bool removido;
};

// por cópia
Acontecimento cria_acontecimento(string nome, string local, int ano,
                                 string paises_envolvidos)
{
    // Defini que um id -1 significa que o objeto foi criado, mas ainda não está
    // adicionado na base de dados
    return Acontecimento{-1, nome, local, ano, paises_envolvidos, false};
}

void redimensiona_vetor(Acontecimento *&vetor, unsigned int &capacidade,
                        int fator_aumento)
{
    int capacidade_antiga = capacidade;
    Acontecimento *novo_vetor =
        new Acontecimento[capacidade + fator_aumento]();
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

void adiciona_acontecimento(Acontecimento *&vetor, unsigned int &tamanho_atual,
                            unsigned int &capacidade, Acontecimento ac,
                            int &ultimo_id)
{
    // aqui redimensiona o vetor para caber o novo acontecimento 
    if (tamanho_atual == capacidade)
    {
        redimensiona_vetor(vetor, capacidade, FATOR_AUMENTO_VETOR);
    }
    // fazendo "manualmente" para evitar erros de ponteiros da string
    ac.id = ++ultimo_id;
    vetor[tamanho_atual].id = ac.id;
    vetor[tamanho_atual].nome = ac.nome;
    vetor[tamanho_atual].local = ac.local;
    vetor[tamanho_atual].ano = ac.ano;
    vetor[tamanho_atual].paises_envolvidos = ac.paises_envolvidos;
    vetor[tamanho_atual].removido = false;
    tamanho_atual++;
}

void remove_acontecimento(Acontecimento *&vetor, unsigned int &tamanho_atual,
                          Acontecimento &ac)
{
    bool removeu = false;
    // loop pelos acontecimentos
    for (unsigned int i = 0; i < tamanho_atual; ++i)
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

int string_para_int(string str)
{
    int resposta = 0;
    bool negativo = false;
    int inicio = 0;
    if (str[0] == '-')
    {
        inicio = 1;
        negativo = true;
    }

    for (int i = inicio; i < (int)str.size(); i++)
    {
        // pensando no numero 123
        // resposta = 0 * 10 + 1 --> resposta  = 1
        // resposta = 1 * 10 + 2 --> resposta  = 12
        // resposta = 12 * 10 + 3 _-> resposta = 123
        resposta = resposta * 10 + (str[i] - '0');
    }

    if (negativo)
        // se existe um sinal de menos, inverte o sinal
        resposta = -resposta;

    return resposta;
}

bool gravar_modificacao_no_csv(Acontecimento *vet, int tamanho)
{
    ofstream gravar("novos_acontecimentos_historicos.csv");
    if (!gravar)
    {
        cout << "Erro ao gravar modificação!" << endl;
        return false;
    }
    else

        gravar << "id,nome,local,ano,paises_envolvidos\n";
    for (int i = 0; i < tamanho; i++)
    {
        if (vet[i].removido == false)
        {
            gravar << vet[i].id << ",";
            gravar << vet[i].nome << ",";
            gravar << vet[i].local << ",";
            gravar << vet[i].ano << ",";
            gravar << '"' << vet[i].paises_envolvidos << '"' << "\n";
        }
    }
    gravar.close();
    return true;
}

void imprime_vetor(Acontecimento *vet, int indice)
{
    if (vet[indice].removido == false)
    {
        cout << "Registro #" << indice << '\n';
        cout << "ID: " << vet[indice].id << '\n';
        cout << "Nome: " << vet[indice].nome << '\n';
        cout << "Local: " << vet[indice].local << '\n';
        cout << "Ano: " << vet[indice].ano << '\n';
        cout << "Paises: " << vet[indice].paises_envolvidos << '\n';
        cout << "----------------------------------------\n";
    }
}

// ordenaçao sequencial de busca por ano
void ordena_por_ano(Acontecimento *&vetor, int tamanho)
{
    int j;
    for (int gap = tamanho / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < tamanho; i++)
        {
            Acontecimento aux = vetor[i];

            j = i;
            while (j >= gap and vetor[j - gap].ano > aux.ano)
            {
                vetor[j] = vetor[j - gap];
                j -= gap;
            }
            vetor[j] = aux;
        }
    }
}

// ordenaçao por id
void ordena_por_id(Acontecimento *&vetor, int tamanho)
{
    int j;
    for (int gap = tamanho / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < tamanho; i++)
        {
            Acontecimento aux = vetor[i];

            j = i;
            while (j >= gap and vetor[j - gap].id > aux.id)
            {
                vetor[j] = vetor[j - gap];
                j -= gap;
            }
            vetor[j] = aux;
        }
    }
}

void buscaBinaria_por_id(Acontecimento *vet, int pInicial, int pFinal, int k)
{
    if (pInicial > pFinal)
    {
        cout << "Nenhum 'Acontecimento Historico' encontrado com o ID digitado!"
             << endl;
        return;
    }

    int meio = (pInicial + pFinal) / 2;
    if (vet[meio].id == k)
    {
        if (vet[meio].removido)
        {
            cout << "O acontecimento com ID " << k << " foi removido!" << endl;
        }
        else
        {
            imprime_vetor(vet, meio);
        }
        return;
    }

    if (pFinal > pInicial)
    {
        if (vet[meio].id < k)
        {
            return buscaBinaria_por_id(vet, meio + 1, pFinal, k);
        }
        else if (vet[meio].id > k)
        {
            return buscaBinaria_por_id(vet, pInicial, meio - 1, k);
        }
    }
    else
    {
        cout << "Nenhum 'Acontecimento Historico' encontrado com o ID digitado!"
             << endl;
    }
}

// usa data inicial e final do intervalo(Ex; acontecimentos,1990,2000,tamanho)
void busca_por_ano(Acontecimento *vet, int inicio, int final, int tamanho)
{
    if (inicio > final)
    {
        cout << "O ano inicial não pode ser maior que o ano final!" << endl;
        return;
    }

    bool achou = false;
    for (int i = 0; i < tamanho; i++)
    {
        if (vet[i].ano >= inicio and vet[i].ano <= final and
            vet[i].removido == false)
        {
            imprime_vetor(vet, i);
            achou = true;
        }
    }

    if (achou == false)
    {
        cout << "Nenhum acontecimento ativo encontrado nesse intervalo de anos!"
             << endl;
    }
}

void busca_por_intervalo_id(Acontecimento *vet, int inicio, int final,
                            int tamanho)
{
    if (inicio > final)
    {
        cout << "O ID inicial não pode ser maior que o ID final!" << endl;
        return;
    }

    bool achou = false;
    for (int i = 0; i < tamanho; i++)
    {
        if (vet[i].id >= inicio and vet[i].id <= final and
            vet[i].removido == false)
        {
            imprime_vetor(vet, i);
            achou = true;
        }
    }

    if (achou == false)
    {
        cout << "Nenhum acontecimento ativo encontrado nesse intervalo de IDs!"
             << endl;
    }
}

void mostra_trecho_por_posicao(Acontecimento *vet, int inicio, int final, int tamanho)
{
    if (inicio < 1 or final < 1)
    {
        cout << "As posições devem ser maiores que zero!" << endl;
        return;
    }

    if (inicio > tamanho or final > tamanho)
    {
        cout << "As posições devem estar dentro do tamanho do arquivo!" << endl;
        return;
    }

    if (inicio > final)
    {
        cout << "A posição inicial não pode ser maior que a posição final!"
             << endl;
        return;
    }

    bool achou = false;
    for (int posicao = inicio; posicao <= final; posicao++)
    {
        int indice = posicao - 1;
        if (vet[indice].removido == false)
        {
            cout << "Posição no arquivo: " << posicao << '\n';
            imprime_vetor(vet, indice);
            achou = true;
        }
    }

    if (achou == false)
    {
        cout << "Todos os registros desse trecho foram removidos!" << endl;
    }
}

// busca linear por nome
void buscaLinear_por_nome(Acontecimento *vet, int tamanho, string nome)
{
    bool achou = false;
    int i = 0;
    while (achou == false and i < tamanho)
    {
        if (nome == vet[i].nome and vet[i].removido == false)
        {
            imprime_vetor(vet, i);
            achou = true;
        }
        i++;
    }
    if (achou == false)
    {
        cout << "Nenhum 'Acontecimento Historico' encontrado com o nome digitado!"
             << endl;
    }
}

int main()
{
    // capacidade inicial
    unsigned int capacidade = QUANTIDADE_INICIAL_VETOR;
    Acontecimento *acontecimentos = new Acontecimento[capacidade]();
    
    cout << "Digite o nome do arquivo .csv de acontecimentos historicos para carregar no sistema: ";
    string arquivo;
    getline(cin, arquivo);

    ifstream entrada(arquivo);
    if (not(entrada))
    {
        cout << "Erro: não foi possível abrir o arquivo" << endl;
    }
    string campos[5];
    string campo;
    string linha;
    int idx = 0;
    int i = 0;
    int ultimo_id = 0;
    unsigned int tamanho = 0;
    bool aspas = false;

    getline(entrada, linha); // Pula o cabeçalho

    while (getline(entrada, linha))
    {
        idx = 0;
        aspas = false;
        i = 0;

        if (tamanho == capacidade)
        {
            // redimensiona_vetor aumenta a capacidade por referencia
            redimensiona_vetor(acontecimentos, capacidade, FATOR_AUMENTO_VETOR);
        }

        while (linha[i] != '\0')
        {
            // checa se é uma aspas duplas
            if (linha[i] == '"')
            {
                // Precisa ser !aspas e nao aspas = true pois se tudo for
                // true ele sempre ignorará as aspas
                aspas = !aspas;
            }
            else if (linha[i] == ',' and !aspas)
            {
                campos[idx] = campo;
                campo = "";
                idx++;
            }
            else
            {
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
        acontecimentos[tamanho].removido = false;
        tamanho++;
    }

    // Adicionei essa parte pois caso reabra o novo arquivo o ultimo_id nao necessariamente
    // é o ultimo devido as possiveis ordenações presentes no código
    unsigned int maiorid = acontecimentos[0].id;
    for (unsigned int i = 0; i < tamanho; ++i)
        if (maiorid < acontecimentos[i].id)
            maiorid = acontecimentos[i].id;
    ultimo_id = maiorid;

    // a partir de agora todos os dados estão carregadas na struct
    int opcao;

    do
    {
        cout << " SISTEMA DE ACONTECIMENTOS HISTORICOS\n";
        cout << "1 - Inserir acontecimento\n";
        cout << "2 - Remover acontecimento\n";
        cout << "3 - Buscar por nome\n";
        cout << "4 - Buscar por ano\n";
        cout << "5 - Buscar por ID\n";
        cout << "6 - Mostrar todos\n";
        cout << "7 - Buscar por intervalo de anos\n";
        cout << "8 - Mostrar trecho por intervalo de IDs\n";
        cout << "9 - Mostrar trecho por posições\n";
        cout << "10 - Ordenar por ID\n";
        cout << "11 - Ordenar por ano\n";
        cout << "12 - Salvar alteracoes\n";
        cout << "0 - Sair\n";
        cout << "Opcao: ";

        cin >> opcao;

        switch (opcao)
        {

        case 1:
        {
            string nome, local, paises;
            int ano;

            cin.ignore();

            cout << "Nome: ";
            getline(cin, nome);

            cout << "Local: ";
            getline(cin, local);

            cout << "Ano: ";
            cin >> ano;
            cin.ignore();

            cout << "Paises envolvidos: ";
            getline(cin, paises);

            Acontecimento novo = cria_acontecimento(nome, local, ano, paises);

            adiciona_acontecimento(acontecimentos, tamanho, capacidade, novo,
                                   ultimo_id);

            cout << "Registro adicionado!\n";
            break;
        }

        case 2:
        {
            int id;

            cout << "ID para remover: ";
            cin >> id;

            Acontecimento temp;
            temp.id = id;

            remove_acontecimento(acontecimentos, tamanho, temp);

            break;
        }

        case 3:
        {
            string nome;

            cin.ignore();

            cout << "Nome: ";
            getline(cin, nome);

            buscaLinear_por_nome(acontecimentos, tamanho, nome);

            break;
        }

        case 4:
        {
            int ano;

            cout << "Ano: ";
            cin >> ano;

            busca_por_ano(acontecimentos, ano, ano, tamanho);

            break;
        }

        case 5:
        {
            int id;

            cout << "ID: ";
            cin >> id;

            ordena_por_id(acontecimentos, tamanho);

            buscaBinaria_por_id(acontecimentos, 0,
                                static_cast<int>(tamanho) - 1, id);

            break;
        }

        case 6:
        {
            for (unsigned int i = 0; i < tamanho; i++)
            {
                imprime_vetor(acontecimentos, i);
            }
            break;
        }

        case 7:
        {
            int inicio, fim;

            cout << "Ano inicial: ";
            cin >> inicio;

            cout << "Ano final: ";
            cin >> fim;

            busca_por_ano(acontecimentos, inicio, fim, tamanho);

            break;
        }

        case 8:
        {
            int inicio, fim;

            cout << "ID inicial: ";
            cin >> inicio;

            cout << "ID final: ";
            cin >> fim;

            busca_por_intervalo_id(acontecimentos, inicio, fim, tamanho);

            break;
        }

        case 9:
        {
            int inicio, fim;

            cout << "Posição inicial: ";
            cin >> inicio;

            cout << "Posição final: ";
            cin >> fim;

            mostra_trecho_por_posicao(acontecimentos, inicio, fim, tamanho);

            break;
        }

        case 10:
            ordena_por_id(acontecimentos, tamanho);
            cout << "Ordenado por ID!\n";
            break;

        case 11:
            ordena_por_ano(acontecimentos, tamanho);
            cout << "Ordenado por ano!\n";
            break;

        case 12:
            {
                bool ok = gravar_modificacao_no_csv(acontecimentos, tamanho);
                if (ok)
                    cout << "Arquivo salvo!\n";
                else 
                    cout << "Erro ao gravar as alterações em um novo arquivo.\n";
                break;
            }

        case 0:
            cout << "Encerrando...\n";
            break;

        default:
            cout << "Opcao invalida!\n";
        }

    } while (opcao != 0);

    delete[] acontecimentos;

    return 0;
}
