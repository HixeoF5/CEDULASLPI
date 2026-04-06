#pragma once
#include <opencv2/opencv.hpp>
#include "CEDULAS.hpp"

using namespace cv;
using namespace std;

extern int estadoGlobal;

inline void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        if (x >= 450 && x <= 750 && y >= 220 && y <= 290) estadoGlobal = 1;
        if (x >= 450 && x <= 750 && y >= 350 && y <= 420) estadoGlobal = 2;
    }
}

inline void desenharSidebar(Mat& frame, GerenciadorFinancas& fin, int valor, int conf) {
    int W = frame.cols, H = frame.rows;
    int sbW = 180, x0 = W - sbW;

    Mat overlay = frame.clone();
    rectangle(overlay, Rect(x0, 0, sbW, H), Scalar(25,25,25), -1);
    addWeighted(overlay, 0.6, frame, 0.4, 0, frame);

    int xt = x0 + 10;
    auto T = [&](const string& s, int y, Scalar cor, double sc = 0.5, int esp = 1) {
        putText(frame, s, Point(xt, y), FONT_HERSHEY_SIMPLEX, sc, cor, esp, LINE_AA);
    };

    T("MAIOR SOMA",   35, Scalar(0,255,255), 0.5, 2);
    T("R$ "+to_string(fin.getMaiorSoma()), 65, Scalar(255,255,255), 0.6);

    T("ULTIMA SOMA", 110, Scalar(180,180,180), 0.5, 2);
    T("R$ "+to_string(fin.getUltima()),  140, Scalar(180,180,180), 0.6);

    T("SESSAO:", 200, Scalar(0,220,0), 0.5, 2);
    int yh = 230;
    for (auto& [n, q] : fin.getContagem()) {
        T("R$"+to_string(n)+": "+to_string(q)+" un", yh, Scalar(230,230,230), 0.4);
        yh += 30;
        if (yh > H - 100) break; 
    }

    T("CONFIANCA", yh+10, Scalar(200,200,0), 0.45, 2);
    rectangle(frame, Rect(xt, yh+22, 150, 12), Scalar(70,70,70), -1);
    Scalar corBarra = conf >= 100 ? Scalar(0,230,80) : Scalar(0,160,255);
    rectangle(frame, Rect(xt, yh+22, 150*conf/100, 12), corBarra, -1);
    T(to_string(conf)+"%", yh+46, Scalar(200,200,200));

    if (fin.getSoma() > fin.getMaiorSoma() && fin.getMaiorSoma() > 0)
        putText(frame, "NOVA MAIOR SOMA!", Point(20, 120), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255), 2, LINE_AA);

    putText(frame, "SOMA: R$ " + to_string(fin.getSoma()), Point(20, H - 40), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(255,255,255), 2, LINE_AA);
}

inline void desenharMenu(Mat& menu) {
    menu = Mat::zeros(600, 1200, CV_8UC3);

    putText(menu, "SISTEMA LAVIO", Point(380, 110), FONT_HERSHEY_SIMPLEX, 1.8, Scalar(0,255,255), 3, LINE_AA);
    putText(menu, "Prof. Derzu", Point(510, 160), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(180,180,180), 1, LINE_AA);

    rectangle(menu, Rect(450,220,300,70), Scalar(0,150,0), -1);
    rectangle(menu, Rect(450,220,300,70), Scalar(0,220,0), 2);
    putText(menu, "INICIAR CAMERA", Point(480, 264), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255,255,255), 2, LINE_AA);

    rectangle(menu, Rect(450,350,300,70), Scalar(0,0,150), -1);
    rectangle(menu, Rect(450,350,300,70), Scalar(80,80,220), 2);
    putText(menu, "SAIR DO PROGRAMA", Point(468, 394), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255,255,255), 2, LINE_AA);

    putText(menu, "ENTER: somar nota  |  V / ESC: voltar ao menu", Point(320, 540), FONT_HERSHEY_SIMPLEX, 0.55, Scalar(140,140,140), 1, LINE_AA);
}