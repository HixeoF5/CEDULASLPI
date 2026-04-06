 Sistema de Reconhecimento de Cédulas 💵
 Projeto final, LPI, PROF.DERZU.



```
classDiagram
    class GerenciadorFinancas {
        <<Arquivo: CEDULAS.hpp>>
        -int carteiraSessao
        -int maiorSoma
        -int ultimaSoma
        -map~int, int~ contagemSessao
        -string arqRelatorio
        -string arqMaiorSoma
        -string arqUltimaSoma
        +GerenciadorFinancas()
        +somarNota(int valor) void
        +finalizarESalvar() void
        +getSoma() int
        +getMaiorSoma() int
        +getUltima() int
        +getContagem() map~int, int~
    }

    class MotorReconhecimento {
        <<Arquivo: OPENCV.hpp>>
        -map~int, Mat~ templates
        -static const int JANELA
        -static const int MINVOTOS
        -static const int RESETFRAMES
        -vector~int~ historico
        -int notaConfirmada
        -int framesSemNota
        -matchInfalivel(Mat roiCinza, Mat tmpl) double
        -votarJanela(int detectado) int
        +MotorReconhecimento()
        +confianca() int
        +analisar(Mat roiBGR) int
    }

    class ModuloInterface {
        <<Arquivo: INTERFACE.hpp>>
        +onMouse(int event, int x, int y, int flags, void* userdata) void
        +desenharSidebar(Mat& frame, GerenciadorFinancas& fin, int valor, int conf) void
        +desenharMenu(Mat& menu) void
    }

    class SistemaPrincipal {
        <<Arquivo: PROJFINAL.cpp>>
        +int estadoGlobal
        +main() int
    }

    SistemaPrincipal --> GerenciadorFinancas : Instancia (Model)
    SistemaPrincipal --> MotorReconhecimento : Executa (Motor)
    SistemaPrincipal --> ModuloInterface : Renderiza (View)
    ModuloInterface ..> GerenciadorFinancas : Lê dados para exibir
