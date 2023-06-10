#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <string>
#include <chrono>
//#include <windows.h>

#include  "frutinha.h"
#include "frutinha.cpp"

using namespace std;
using namespace cv;
using namespace std::chrono;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale, bool tryflip);

string cascadeName;



int main( int argc, const char** argv )
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
        return -1;
    }

    // if(!capture.open("video.mp4")) // para testar com um video
    if(!capture.open(0))
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    if( capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;

        auto start = steady_clock::now();
        auto end = steady_clock::now();

        for(;;)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            end = steady_clock::now();
            auto gasto = end - start;
            cout << "Tempo: " << duration_cast<seconds>(gasto).count() << " s" << endl;

            if(duration_cast<seconds>(gasto).count() == 30){
                break;
            }

            detectAndDraw( frame, cascade, scale, tryflip   );
            

            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }

        //auto end = steady_clock::now();
        auto gasto = end - start;
        //cout << "Tempo: " << duration<double>{gasto}.count() << " s" << endl;

    }

    return 0;
}

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

    
   
    // Desenha uma imagem
         //Mat overlay2 = cv::imread("C:\\Users\\biamo\\codigos\\projeto-openCV\\projeto-openCV\\noz.png", IMREAD_UNCHANGED);

        Mat overlay2 = cv::imread("NOZSFundo.png", IMREAD_UNCHANGED);
            drawTransparency(img, overlay2, laranja.getX(), laranja.getY());

    // Desenha o frame na tela
    imshow( "result", img );
}
