#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <thread>
//#include <windows.h>

#include  "frutinha.h"
#include "frutinha.cpp"

using namespace std;
using namespace cv;
using namespace std::chrono;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);
void drawTransparency(Mat frame, Mat transp, int xPos, int yPos);
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region);

string cascadeName;

void salvarPlacar(int placar) {
    ofstream arquivo("recorde.txt");
    if (arquivo.is_open()) {
        arquivo << placar;
        arquivo.close();
    }
}

int lerRecorde() {
    ifstream arquivo("recorde.txt");
    int recorde = 0;
    if (arquivo.is_open()) {
        arquivo >> recorde;
        arquivo.close();
    }
    return recorde;
}

int recorde = lerRecorde();
int temporizador = 0;

/**
 * @brief Draws a transparent image over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */

int j = 0;
int eixoX = 30, eixoY = 60;
int contador = 0;


void drawTransparency(Mat frame, Mat transp, int xPos, int yPos) {
    Mat mask;
    vector<Mat> layers;

    split(transp, layers); // seperate channels
    Mat rgb[3] = { layers[0],layers[1],layers[2] };
    mask = layers[3]; // png's alpha channel used as mask
    merge(rgb, 3, transp);  // put together the RGB channels, now transp insn't transparent 
    transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}


/**
 * @brief Draws a transparent rect over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param regin rect region where the should be positioned
 */
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

Frutinha laranja;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip)
{

    double t = 0;
    vector<Rect> faces;
    Mat gray, smallImg;
    Scalar color = Scalar(255,0,0);

    if( tryflip )
        flip(img, img, 1);

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
    if (scale > 1)
        resize( img, img, Size(), fx, fx, INTER_LINEAR_EXACT );
    equalizeHist( smallImg, smallImg );

    t = (double)getTickCount();

    cascade.detectMultiScale( smallImg, faces,
        1.3, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(40, 40) );
    t = (double)getTickCount() - t;
    //printf( "detection time = %g ms\n", t*1000/getTickFrequency());
    // PERCORRE AS FACES ENCONTRADAS
    
   

    
    for ( size_t i = 0; i < faces.size(); i++)
    {
        
        Rect r = faces[i];
        // Desenha uma imagem
        //Mat overlay3 = cv::imread("C:\\Users\\biamo\\codigos\\projeto-openCV\\projeto-openCV\\esquilo.png", IMREAD_UNCHANGED);
        Mat overlay3 = cv::imread("esquiloSFundo.png", IMREAD_UNCHANGED);
        try{
           drawTransparency(img, overlay3, cvRound(r.x), cvRound(r.y));
            }catch(Exception e){}

        //rectangle( img, Point(cvRound(r.x), cvRound(r.y)),
                    //Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                    //color, 0); //rectangle(img, UPPER_RIGHT, BOTTOM_LEFT)

        
        
        
        //condição em que o rosto esta na laranja
        if(laranja.getX() > cvRound(r.x) && laranja.getX() < cvRound((r.x + r.width-1)) && laranja.getY() > cvRound(r.y) && laranja.getY() < cvRound((r.y + r.height-1)) && laranja.getComeu() == 0){
                j++;
                laranja.setComeu(1);
                //Beep(523,500); // som WINDOWS
                system("play -q SOM.ogg"); //som LINUX    
                laranja.move();          
        }
        
        if (j > recorde) {
            recorde = j;
            salvarPlacar(recorde);
            cout << "Novo recorde: " << recorde << endl;
        }
    }

        
    //Mat overlay1 = cv::imread("C:\\Users\\biamo\\codigos\\projeto-openCV\\projeto-openCV\\cerca.png", IMREAD_UNCHANGED);
    Mat overlay1 = cv::imread("cerca.png", IMREAD_UNCHANGED);
    int cercaX = 45;
    drawTransparency(img, overlay1, cercaX, 10);
    drawTransparency(img, overlay1, cercaX+=75, 10);
    drawTransparency(img, overlay1, cercaX+=75, 10);
    drawTransparency(img, overlay1, cercaX+=75, 10);
    drawTransparency(img, overlay1, cercaX+=75, 10);
    drawTransparency(img, overlay1, cercaX+=75, 10);
    drawTransparency(img, overlay1, cercaX+=75, 10);
    color = Scalar(245,245,245);
    putText	(img, "Placar: " + to_string(j), Point(200, 50), FONT_HERSHEY_TRIPLEX, 1.5, color); // fonte
    putText	(img, "Recorde: " + to_string(recorde), Point(200, 100), FONT_HERSHEY_TRIPLEX, 1.5, color); // fonte
    putText	(img, "Tempo: " + to_string(temporizador) + "s", Point(200, 150), FONT_HERSHEY_TRIPLEX, 1.5, color); // fonte

    // Desenha uma imagem
         //Mat overlay2 = cv::imread("C:\\Users\\biamo\\codigos\\projeto-openCV\\projeto-openCV\\noz.png", IMREAD_UNCHANGED);

    Mat overlay2 = cv::imread("NOZSFundo.png", IMREAD_UNCHANGED);
    drawTransparency(img, overlay2, laranja.getX(), laranja.getY());

    // Desenha o frame na tela
    imshow( "result", img );
}

void drawGameOver(Mat& imgGameOver, bool& restart, bool& quit) {
    Scalar black = Scalar(0, 0, 0);
    drawTransRect(imgGameOver, black, 0.8, Rect( 0, 0, imgGameOver.cols, imgGameOver.rows));

    putText(imgGameOver, "Game Over", Point(100, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 3);
    putText(imgGameOver, "Placar: " + to_string(j), Point(100, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(imgGameOver, "Recorde: " + to_string(recorde), Point(100, 200), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(imgGameOver, "Pressione 'R' para Reiniciar o jogo", Point(100, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(imgGameOver, "Pressione 'Q' para Sair do jogo", Point(100, 450), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

    imshow("result", imgGameOver);


    char key = (char)waitKey(0);
    if (key == 'r' || key == 'R') {
        restart = true;
    } else if (key == 'q' || key == 'Q') {
        quit = true;
    }
}

void drawMenu(Mat &img) {
    flip(img, img, 1);
    Scalar black = Scalar(0, 0, 0);
    drawTransRect(img, black, 0.8, Rect( 0, 0, img.cols, img.rows));

    putText(img, "MENU", Point(100, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 3);
    putText(img, "Recorde: " + to_string(recorde), Point(100, 200), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(img, "Pressione 'I' para iniciar o jogo", Point(100, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
    putText(img, "Pressione 'Q' para sair do jogo", Point(100, 450), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

    imshow("result", img);
    // waitKey(0);
}


int main(int argc, const char** argv)
{
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;
    string PNG;

    //cascadeName = "C:\\Users\\biamo\\codigos\\projeto-openCV\\projeto-openCV\\haarcascade_frontalface_default.xml";
    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cerr << "ERROR: Could not load classifier cascade" << endl;
    }

    // if(!capture.open("video.mp4")) // para testar com um video
    if(!capture.open(0))
    {
        cout << "Capture from camera #0 didn't work" << endl;
    }

    if( capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;

        int placar = 0;
        int recorde = lerRecorde();

        bool menuAtivo = true;
        bool jogoIniciado = false;
        bool iniciarContagem = false;
        
        while(frame.empty()){
            capture >> frame;
        }

        drawMenu(frame);

        while (menuAtivo) {
            char opcao = (char)waitKey(10);
            if (opcao == ' ') {
                break;
            }
            if (opcao == 'q' || opcao == 'Q') {
                // Sair do jogo
                cout << "Fechando..." << endl;
                return 0;
            }
            if (opcao == 'i' || opcao == 'I') {
                // Iniciar o jogo
                cout << "Iniciando o jogo..." << endl;
                menuAtivo = false;
                jogoIniciado = true;
                iniciarContagem = true;
                temporizador = 0;
                j = 0;
            }
        }
        
        if (jogoIniciado && !menuAtivo) {
            auto start = steady_clock::now();
            auto end = steady_clock::now();
            while (jogoIniciado && !menuAtivo) {
                for(;;)
                {
                    capture >> frame;
                    if( frame.empty() )
                        break;

                    end = steady_clock::now();
                    auto gasto = end - start;
                    temporizador = duration_cast<seconds>(gasto).count();
                    cout << "Tempo: " << temporizador << "s" << endl;

                    if (iniciarContagem && temporizador >= 5) {
                        jogoIniciado = false;
                        menuAtivo = true;
                        iniciarContagem = false;
                        temporizador = 0;
                    }

                    if (!jogoIniciado && menuAtivo) {
                        drawGameOver(frame, jogoIniciado, menuAtivo);
                        temporizador = 0;
                        j = 0;
                    }

                    detectAndDraw( frame, cascade, scale, tryflip );
                    
                    char c = (char)waitKey(10);
                    if( c == 27 || c == 'q' || c == 'Q' )
                        return 0;
                }
            }

            auto gasto = end - start;
        }   

        //auto end = steady_clock::now();
        
        //cout << "Tempo: " << duration<double>{gasto}.count() << " s" << endl;

        // this_thread::sleep_for(seconds(2));
    }
    return 0;
}