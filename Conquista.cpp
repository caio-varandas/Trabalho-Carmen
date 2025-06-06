#include <bits/stdc++.h>
using namespace std;
/*O objetivo do jogo é chegar do planeta Terra até o planeta Tatooine 
e conquistá-lo. Você começa na Terra com um número x de naves e precisa 
passar por planetas vizinhos que você escolhe para lutar, a luta é aleatória 
e após escolher o planeta com o qual você irá lutar, você vai receber os
 resultados da missão: quantas naves foram ganhas e quantas foram perdidas. 
 As naves perdidas vão para o planeta Tatooine, já que ele domina toda essa 
 galáxia e os planetas são aliados dele. Após escolher quais planetas você vai passar, 
 você poderá chegar em Tatooine e conquistá-lo caso tenha uma quantidade maior ou igual de naves. 
 Para dificultar ainda mais o jogo, o player vai jogar e logo depois um robô irá jogar e o robô sempre achará o melhor caminho. 
 O Player só ganha caso tenha uma quantidade maior ou igual de naves que o robô quando conquistar Tatooine.
*/
vector<vector<int>> mapa(9);
vector<int> naves(9);
vector<int> naves_j(9);
map<int, string> nomes;

/*A função gera_nomes aplica um dicionário, 
onde o planeta 1 é associado ao nome terra, o planeta 2 é associado a 
puclandia, etc. Apenas para facilitar andar pelo grafo.
*/
void gera_nomes() {
    nomes[1] = "terra";
    nomes[2] = "puclandia";
    nomes[3] = "carecopolis";
    nomes[4] = "varandas";
    nomes[5] = "alderaan";
    nomes[6] = "endor";
    nomes[7] = "andor";
    nomes[8] = "tatooine";
}


/*
A função constroi_mapa gera o mapa do jogo, criando a lista de adjacências
, ou seja, quais planetas estão ligados aos outros.
*/
void constroi_mapa() {
    mapa[1] = {2, 3, 4, 5, 7};
    mapa[2] = {1, 3, 5, 6};
    mapa[3] = {1, 2, 4, 5, 6, 7};
    mapa[4] = {1, 3, 6, 7};
    mapa[5] = {1, 2, 3, 6, 8};
    mapa[6] = {2, 3, 4, 5, 7, 8};
    mapa[7] = {1, 3, 4, 6, 8};
    gera_nomes();
}



/*Para cada planeta é gerado um valor aleatório de naves entre (0 e 5) + 4,
enquanto o planeta Tatooine recebe um valor aleatório entre (0 e 5) + 6,
para dificultar o jogo.
*/
void gera_naves() {
    srand(time(NULL));
    naves[1] = (rand() % 5) + 4;
    for (int i = 2; i < 8; i++) {
        naves[i] = (rand() % 5) + 1;
    }
    naves[8] = (rand() % 5) + 6; // mais forte
    cout << "Naves por vertice:\n";
    for (int i = 1; i <= 8; i++) {
        cout << "planeta " << nomes[i] << ": " << naves[i] << " naves\n";
    }
    naves_j = naves;
}



/*
A função soma_intermediarios_valida verifica para cada caminho possível, 
a soma das naves de cada planeta, retorna verdadeiro caso a soma de naves
daquele caminho seja maior que a quantidade de naves do planeta Tatooine, 
e falso caso seja menor.
*/
bool soma_intermediarios_valida(const vector<int>& caminho, int qtd_naves_inicial, int naves_final) {
    int soma = qtd_naves_inicial;
    for (size_t i = 1; i + 1 < caminho.size(); ++i)
    {
        soma += naves[caminho[i]];
    }
    return soma >= naves_final;
}


/*A função caminho_valido_bfs busca encontrar um caminho válido entre dois 
pontos em um grafo, usando o algoritmo de Busca em Largura (BFS). 
A ideia central é explorar todos os caminhos possíveis partindo de um nó 
inicial até o nó de destino, garantindo que o caminho escolhido seja o 
menor e tenha a quantidade de naves necessárias para conquistar o 
planeta Tatooine quando chegar lá.
*/
vector<int> caminho_valido_bfs(int atual, int destino, int qtd_naves) {
    queue<vector<int>> fila;
    fila.push({atual});
    int menor_tamanho = INT_MAX;

    while (!fila.empty()) 
    {
        vector<int> caminho = fila.front();
        fila.pop();
        int ultimo = caminho.back();

        if (ultimo == destino) {
            if (soma_intermediarios_valida(caminho, qtd_naves, naves[destino]))
                return caminho;
        }

        if ((int)caminho.size() > menor_tamanho) continue;

        for (int viz : mapa[ultimo]) {
            if (find(caminho.begin(), caminho.end(), viz) == caminho.end()) 
            {
                vector<int> novo = caminho;
                novo.push_back(viz);
                fila.push(novo);
            }
        }
    }
    return {};
}



/*impressao das conexoes dos planetas*/
void imprime_conexoes() {
    cout << "Conexoes entre planetas:\n";
    for (int i = 1; i < (int)mapa.size(); i++)
    {
        cout << nomes[i] << " (" << i << ") -> ";
        for (size_t j = 0; j < mapa[i].size(); j++) 
        {
            cout << nomes[mapa[i][j]] << " (" << mapa[i][j] << ")";
            if (j + 1 < mapa[i].size()) cout << ", ";
        }
        cout << "\n";
    }
}



/*
A função do “player” implementa o modo de jogo no qual o usuário
controla as decisões. Inicialmente, são declarados 4 variáveis, 
a variável “atual” diz em qual planeta o jogador inicia (planeta 1), 
“destino” diz onde o jogador deve chegar (planeta 8), “qtd_naves” 
e a quantidade de naves que ele possui a partir do primeiro planeta, 
e “naves_j” representa o estado atual das naves nos planeta, mantendo 
separado as escolhas do jogador e do robô. O loop principal da função 
continua enquanto o jogador ainda não alcançou o planeta de destino. 
Durante o processo, o código impede que o jogador avance diretamente 
para Tatooine caso não possua naves suficientes para vencer a defesa 
do planeta. O jogador deve escolher um planeta vizinho para avançar, 
sendo permitido apenas se houver conexão direta entre o planeta atual 
e o escolhido. Caso o planeta escolhido não seja o destino final, o 
jogador recebe uma quantidade aleatória de naves do planeta conquistado 
e também pode perder algumas naves, enquanto o planeta final aumenta sua
defesa com naves adicionais a cada turno, tornando o desafio mais difícil
a cada rodada
*/
int player() {
    int atual = 1;
    int destino = 8;
    int qtd_naves = naves[atual];
    naves_j[atual] = 0;

    cout << "\n=== MODO JOGADOR ===\n";
    cout << "Voce esta no planeta " << nomes[atual] << " com " << qtd_naves << " naves.\n";

    vector<string> visitados;
    visitados.push_back(nomes[atual]);

    while (atual != destino) 
    {
        cout << "\nPlanetas vizinhos:\n";
        vector<int> vizinhos_validos;

        for (int viz : mapa[atual]) 
        {
            if (viz == destino && qtd_naves < naves_j[destino]) 
            {
                cout << "tatooine indisponivel - " << nomes[viz] << " (X), com " << naves_j[viz] << " naves\n";
                continue;
            }
            cout << viz << " - " << nomes[viz] << " (" << viz << "), com " << naves_j[viz] << " naves\n";
            vizinhos_validos.push_back(viz);
        }

        cout << "Digite o numero do planeta para onde deseja ir: ";
        int escolha;
        cin >> escolha;

        if (find(vizinhos_validos.begin(), vizinhos_validos.end(), escolha) == vizinhos_validos.end()) 
        {
            cout << "Planeta invalido. Tente novamente.\n";
            continue;
        }

        int naves_ganhas = 0;
        int naves_perdidas = 0;

        if (escolha != destino) 
        {
            naves_ganhas = rand() % (naves_j[escolha] + 1);
            naves_perdidas = rand() % 3;
            naves_j[destino] += naves_perdidas;
        }

        int naves_destino = naves_j[destino];

        qtd_naves += naves_ganhas;
        qtd_naves -= naves_perdidas;
        if (qtd_naves < 0) qtd_naves = 0;

        cout << "Voando de " << nomes[atual] << " para " << nomes[escolha] << ": +"
             << naves_ganhas << " naves, -" << naves_perdidas << " naves. Total agora: "
             << qtd_naves << "\n";
        cout << "Naves restantes em Tatooine: " << naves_j[destino] << "\n";

        atual = escolha;
        naves_j[atual] = 0;
        visitados.push_back(nomes[atual]);

        if (atual == destino) {
            if (qtd_naves >= naves_destino) {
                cout << "\nVoce conquistou " << nomes[destino] << " com " << qtd_naves
                     << " naves (necessario: " << naves_destino << "). VITORIA!\n";
            } else {
                cout << "\nVoce chegou em " << nomes[destino] << " com " << qtd_naves
                     << " naves, mas precisava de " << naves_destino << ". DERROTA!\n";
            }
            break;
        }
    }

    cout << "\nPlanetas visitados:\n";
    for (const string& nome : visitados)
        cout << "- " << nome << "\n";
    cout << "Total de planetas visitados: " << visitados.size() << "\n";
    return visitados.size();
}



/*
O robô funciona com a mesma regra de jogo do “player”, 
sua diferença é que ele acha o menor caminho válido usando 
uma bfs para conseguir chegar no planeta final tatooine com 
a quantidade de naves necessária para conquistá-lo, caso contrário 
ele perde.
*/
int robo() {
    cout << "\n=== VEZ DO ROBO ===\n";
    int atual = 1;
    int destino = 8;
    int naves_atual = naves[1];
    int naves_dest;

    cout << "\nInicio no vertice " << atual << " (" << nomes[atual] << ") com " << naves_atual << " naves.\n";

    vector<int> caminho = caminho_valido_bfs(atual, destino, naves_atual);
    if (caminho.empty()) {
        cout << "Sem caminho viavel com " << naves_atual << " naves a partir de " << nomes[atual] << ". Derrota!\n";
        return 1;
    }

    int qtd_naves = naves[atual];
    naves[atual] = 0;

    cout << "\nCaminho inicial: ";
    for (int p : caminho)
        cout << nomes[p] << " -> ";
    cout << "FIM\n\n";

    vector<string> visitados;
    visitados.push_back(nomes[atual]);

    for (size_t i = 1; i < caminho.size(); ++i) {
        int prox = caminho[i];
        int naves_ganhas = 0;
        int naves_perdidas = 0;

        if (prox != destino) {
            naves_ganhas = rand() % (naves[prox] + 1);
            naves_perdidas = rand() % 2;
            qtd_naves += naves_ganhas;
            qtd_naves -= naves_perdidas;
            if (qtd_naves < 0) qtd_naves = 0;
            naves[destino] += naves_perdidas;
        }

        naves_dest = naves[destino];

        cout << "Voando de " << nomes[atual] << " para " << nomes[prox] << ": +"
             << naves_ganhas << " naves, -" << naves_perdidas << " naves. Total agora: "
             << qtd_naves << "\n"
             << "quantidade atual de naves de Tatooine: " << naves[destino] << "\n";

        naves[prox] = 0;
        atual = prox;
        visitados.push_back(nomes[atual]);

        if (atual == destino) {
            if (qtd_naves >= naves[destino]) {
                cout << "\nVoce chegou ao planeta " << nomes[destino] << " com " << qtd_naves
                     << " naves (era necessario " << naves_dest << "). Vitoria!\n";
            } else {
                cout << "\nVoce chegou ao planeta " << nomes[destino] << " com " << qtd_naves
                     << " naves, mas precisava de " << naves_dest << ". Derrota!\n";
            }
            break;
        }

        if (i == caminho.size() - 1 || qtd_naves < naves[destino]) {
            cout << "\nRecalculando caminho...\n";
            caminho = caminho_valido_bfs(atual, destino, qtd_naves);
            if (caminho.empty()) {
                cout << "\nSem caminho viavel a partir de " << nomes[atual] << " com " << qtd_naves << " naves. Derrota!\n";
                break;
            }
            i = 0;
        }
    }

    cout << "\nPlanetas visitados:\n";
    for (const string& nome : visitados)
        cout << "- " << nome << "\n";
    cout << "Total de planetas visitados: " << visitados.size() << "\n";
    return visitados.size();
}

int main() { 
    constroi_mapa();
    gera_nomes();
    gera_naves();
    imprime_conexoes();
    int p = player();
    int r = robo();
    cout <<endl<< "RESULTADOS" << endl <<  "Quantidade de planetas visitados pelo robo: " << r << "\n";
    cout << "Quantidade de planetas visitados pelo player: " << p << "\n";
    if (p <= r) {
        cout << "PLAYER WINS\n";
    } else {
        cout << "\nPLAYER AMASSADO\n";
    }
    return 0;
}
