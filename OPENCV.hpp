#pragma once
#include <opencv2/opencv.hpp>
#include <map>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

class MotorReconhecimento {
private:
    map<int, Mat> templates;
    static const int JANELA = 5;   
    static const int MINVOTOS = 2; 
    static const int RESETFRAMES = 10; 
    
    vector<int> historico;         
    int notaConfirmada  = 0;
    int framesSemNota   = 0;

    map<int, double> thresholdMatch = {
        {2, 0.55}, {5, 0.55}, {10, 0.55},
        {20, 0.55}, {50, 0.55}, {100, 0.55}
    };

    double matchEscalas(const Mat& roiCinza, const Mat& tmpl) {
        if (roiCinza.empty() || tmpl.empty()) return 0.0;
        
        double melhorScore = 0.0;
        vector<double> escalas = {0.7, 1.0, 1.3}; 
        
        for (double sc : escalas) {
            Mat tmplRes;
            resize(tmpl, tmplRes, Size(), sc, sc);
            
            if (tmplRes.cols > roiCinza.cols || tmplRes.rows > roiCinza.rows) continue;

            Mat result;
            matchTemplate(roiCinza, tmplRes, result, TM_CCOEFF_NORMED);
            double maxVal;
            minMaxLoc(result, nullptr, &maxVal);
            
            if (maxVal > melhorScore) {
                melhorScore = maxVal;
            }
        }
        return melhorScore;
    }

    int votarJanela(int detectado) {
        historico.push_back(detectado);
        if ((int)historico.size() > JANELA) historico.erase(historico.begin());

        map<int, int> freq;
        for (int v : historico) if (v > 0) freq[v]++;

        int melhorValor = 0, melhorFreq = 0;
        for (auto& [v, f] : freq)
            if (f > melhorFreq) { melhorFreq = f; melhorValor = v; }

        if (melhorFreq >= MINVOTOS) return melhorValor;
        return 0;
    }

public:
    MotorReconhecimento() {
        for (int v : {2, 5, 10, 20, 50, 100})
            templates[v] = imread(to_string(v) + "template.jpg", IMREAD_GRAYSCALE);
        historico.reserve(JANELA);
    }

    int confianca() {
        if (historico.empty()) return 0;
        map<int,int> freq;
        for (int v : historico) if (v > 0) freq[v]++;
        int mx = 0;
        for (auto& [v,f] : freq) mx = max(mx, f);
        return min(100, mx * 100 / JANELA);
    }

    int analisar(const Mat& roiBGR) {
        if (roiBGR.empty()) {
            votarJanela(0);
            return notaConfirmada;
        }

        Mat roiCinza;
        cvtColor(roiBGR, roiCinza, COLOR_BGR2GRAY);

        int detectado = 0;
        double melhorScore = 0;

        for (auto& [valor, tmpl] : templates) {
            if (tmpl.empty()) continue;

            double score = matchEscalas(roiCinza, tmpl);
            if (score > thresholdMatch[valor] && score > melhorScore) {
                melhorScore = score;
                detectado = valor;
            }
        }

        int candidato = votarJanela(detectado);
        if (candidato > 0) {
            notaConfirmada = candidato;
            framesSemNota = 0;
        } else {
            framesSemNota++;
            if (framesSemNota >= RESETFRAMES) {
                notaConfirmada = 0;
                historico.clear();
                framesSemNota = 0;
            }
        }
        return notaConfirmada;
    }
};