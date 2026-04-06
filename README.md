 Sistema de Reconhecimento de Cédulas 💵
 Projeto final, LPI, PROF.DERZU.


```mermaid
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
        +onMouse(event: int, x: int, y: int, flags: int, userdata: void*) void
        +desenharSidebar(frame: Mat&, fin: GerenciadorFinancas&, valor: int, conf: int) void
        +desenharMenu(menu: Mat&) void
    }

    class SistemaPrincipal {
        <<Arquivo: PROJFINAL.cpp>>
        +int estadoGlobal
        +main() int
    }

    SistemaPrincipal --> GerenciadorFinancas : Instancia (Model)
    SistemaPrincipal --> MotorReconhecimento : Executa (Motor)
    SistemaPrincipal --> ModuloInterface : Renderiza (View)
    ModuloInterface ..> GerenciadorFinancas : Lê```
