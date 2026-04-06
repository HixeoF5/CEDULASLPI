#include <opencv2/opencv.hpp>
#include <iostream>
#include <map>

#include "CEDULAS.hpp"
#include "OPENCV.hpp"
#include "INTERFACE.hpp"

using namespace cv;
using namespace std;

int estadoGlobal = 0;

int main() {
    MotorReconhecimento motor;
    GerenciadorFinancas financeiro;

    map<int, Mat> imagensDisplay;
    for (int v : {2, 5, 10, 20, 50, 100}) {
        Mat img = imread(to_string(v) + ".jpg"); 
        if (img.empty()) img = imread(to_string(v) + "template.jpg"); 
        imagensDisplay[v] = img;
    }

    const string winName = "Sistema Lavio - Prof. Derzu";
    namedWindow(winName, WINDOW_NORMAL);
    resizeWindow(winName, 1280, 720);
    setMouseCallback(winName, onMouse);

    while (estadoGlobal != 2) {
        if (estadoGlobal == 0) {
            Mat menu;
            desenharMenu(menu);
            imshow(winName, menu);
            if (waitKey(30) == 27) break;
        } 
        else if (estadoGlobal == 1) {
            VideoCapture cap(0);
            cap.set(CAP_PROP_FRAME_WIDTH,  1280);
            cap.set(CAP_PROP_FRAME_HEIGHT, 720);
            cap.set(CAP_PROP_FPS,          30);

            while (estadoGlobal == 1) {
                Mat frame;
                cap >> frame;
                if (frame.empty()) break;
                flip(frame, frame, 1);

                int aW = 450;
                int aH = 300;
                int sidebarW = 180;
                
                int areaUtil = frame.cols - sidebarW;
                
                if (areaUtil < aW) aW = areaUtil - 20;
                if (frame.rows < aH) aH = frame.rows - 20;

                int aX = (areaUtil - aW) / 2;
                int aY = (frame.rows - aH) / 2;
                
                Rect alvo(max(0, aX), max(0, aY), min(aW, frame.cols), min(aH, frame.rows));

                rectangle(frame, alvo, Scalar(255, 150, 0), 2, LINE_AA);

                int valor = 0;
                if (alvo.width > 0 && alvo.height > 0) {
                    valor = motor.analisar(frame(alvo));
                }

                desenharSidebar(frame, financeiro, valor, motor.confianca());

                if (valor > 0) {
                    Mat imgModelo = imagensDisplay[valor];
                    if (!imgModelo.empty()) {
                        int seloW = 160;
                        int seloH = 75;
                        Mat modeloResized;
                        resize(imgModelo, modeloResized, Size(seloW, seloH));

                        int pX = alvo.x;
                        int pY = alvo.y - seloH - 10;
                        
                        if (pY < 0) pY = alvo.y + 10;

                        if (pX >= 0 && pY >= 0 && pX + seloW <= frame.cols && pY + seloH <= frame.rows) {
                            modeloResized.copyTo(frame(Rect(pX, pY, seloW, seloH)));
                        }
                    }
                }

                imshow(winName, frame);

                char k = (char)waitKey(1);
                if (k == 13 && valor > 0) {
                    financeiro.somarNota(valor);
                    system("ffplay -nodisp -autoexit money.mp3 > /dev/null 2>&1 &");
                }
                if (k == 'v' || k == 'V' || k == 27) {
                    financeiro.finalizarESalvar();
                    estadoGlobal = 0;
                }
            }
            cap.release();
        }
    }
    return 0;
}