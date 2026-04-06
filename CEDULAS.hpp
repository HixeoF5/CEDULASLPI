#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <map>
#include <string>

using namespace std;

class GerenciadorFinancas {
private:
    int carteiraSessao = 0, maiorSoma = 0, ultimaSoma = 0;
    map<int, int> contagemSessao;
    const string arqRelatorio = "relatorio_lavio.txt";
    const string arqMaiorSoma = "maiorsoma.txt";
    const string arqUltimaSoma = "ultimasoma.txt";
public:
    GerenciadorFinancas() {
        ifstream fMaior(arqMaiorSoma);
        if (!(fMaior >> maiorSoma)) maiorSoma = 0;
        fMaior.close();

        ifstream fUltima(arqUltimaSoma);
        if (!(fUltima >> ultimaSoma)) ultimaSoma = 0;
        fUltima.close();
    }

    void somarNota(int valor) { 
        carteiraSessao += valor; 
        contagemSessao[valor]++; 
    }

    void finalizarESalvar() {
        if (carteiraSessao == 0) return;
        
        ultimaSoma = carteiraSessao;
        ofstream fUlt(arqUltimaSoma); 
        fUlt << ultimaSoma; 
        fUlt.close();

        if (carteiraSessao > maiorSoma) {
            maiorSoma = carteiraSessao;
            ofstream fMaior(arqMaiorSoma); 
            fMaior << maiorSoma; 
            fMaior.close();
        }

        ofstream arq(arqRelatorio, ios::app);
        time_t agora = time(0); 
        string dt = ctime(&agora);
        if (!dt.empty()) dt.pop_back(); 
        
        arq << dt << " | TOTAL: R$ " << carteiraSessao << " | CEDULAS: ";
        bool primeiro = true;
        for (auto const& [nota, qtd] : contagemSessao) {
            if (!primeiro) arq << ", ";
            arq << qtd << "x(R$" << nota << ")";
            primeiro = false;
        }
        arq << "\n";
        arq.close();

        carteiraSessao = 0; 
        contagemSessao.clear();
    }

    int getSoma()      { return carteiraSessao; }
    int getMaiorSoma() { return maiorSoma; }
    int getUltima()    { return ultimaSoma; }
    map<int,int> getContagem() { return contagemSessao; }
};