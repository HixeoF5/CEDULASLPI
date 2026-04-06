 Sistema de Reconhecimento de Cédulas 
 
 Projeto final da cadeira de linguagem de programação 1, LPI, PROF.DERZU. UFPB.

 01 - git clone https://github.com/HixeoF5/CEDULASLPI.git
 
 02 - cd CEDULASLPI

 03 - sudo apt update

 04 - sudo apt install g++ pkg-config libopencv-dev ffmpeg -y

 05 - Se for usar o celular como câmera, instale o Iriun Webcam no PC  e conecte o celular. Certifique-se de que a imagem do celular está aparecendo na tela do PC antes de rodar o código

 06 - g++ PROJFINAL.cpp -o PROJFINAL `pkg-config --cflags --libs opencv4`

 07 - ./PROJFINAL

 08 - Para somar as cedulas (enter)


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
