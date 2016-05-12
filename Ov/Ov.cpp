// Ov.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <string>
#include <opencv2/opencv.hpp> // インクルードファイル指定
#include <opencv2/opencv_lib.hpp> // 静的リンクライブラリの指定

using namespace std;
using namespace cv; 

/*
画像処理 — OpenCV-CookBook http://opencv.jp/cookbook/opencv_img.html#image-binarize
*/
Mat binarize(string path) {

    cv::Mat gray_img = cv::imread(path, 0);

    if (gray_img.empty()) throw exception();
    //"gray_img is empty.";

    // 固定の閾値処理
    cv::Mat bin_img, bininv_img, trunc_img, tozero_img, tozeroinv_img;
    // 入力画像，出力画像，閾値，maxVal，閾値処理手法
    cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 適応的な閾値処理
    cv::Mat adaptive_img;
    // 入力画像，出力画像，maxVal，閾値決定手法，閾値処理手法，blockSize，C
    cv::adaptiveThreshold(gray_img, adaptive_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 7, 8);

    cv::namedWindow("Binary", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
    cv::imshow("Binary", bin_img);
    return bin_img;
}

int contours(Mat binaryImage) {
    // 輪郭を抽出
    // CV_RETR_EXTERNAL：最も外側の輪郭のみ抽出
    // CV_CHAIN_APPROX_NONE：輪郭の全部の点を出力
    vector< vector<cv::Point> > contours;
    cv::findContours(binaryImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    // 抽出した輪郭を描画
    cv::Mat contourImage(binaryImage.size(), CV_8U, cv::Scalar(255));
    const int drawAllContours = -1;
    cv::drawContours(contourImage, contours, drawAllContours, cv::Scalar(0));

    // 画像の表示
    cv::imshow("Contour Image", contourImage);
    return 0;
}

int main()
{
    try {
        // 相対パスから指定する方法が分からず。
        string original_image_path = "C:/Users/eiurur/Documents/Visual Studio 2015/Projects/Ov/x64/Debug/data/original.png";
        Mat bin_img = binarize(original_image_path);
        contours(bin_img);
        string original_rotate_image_path = "C:/Users/eiurur/Documents/Visual Studio 2015/Projects/Ov/x64/Debug/data/original_rotate.png";
        binarize(original_rotate_image_path);

    }
    catch (exception e) {
        // 受け取った例外のwhat()を呼び出すことで、エラーメッセージが取得できます。
        cout << e.what() << endl;
        return 1;
    }
    //width220, height150の画像を作成
    Mat src = Mat::zeros(150, 220, CV_8UC3);

    //赤色で画像にHello Worldを描く
    putText(src, "Hello World", Point(5, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 200), 2, CV_AA);

    //緑色で画像に線を描く
    line(src, Point(190, 25), Point(190, 45), Scalar(0, 200, 0), 3);

    //要素を直接操作し画像に点を描く
    for (int x = 188; x < 192; x++) {
        for (int y = 53; y < 57; y++) { // y座標
            for (int i = 0; i < 2; i++) { // i < 3にするとBGRすべての要素を200にする
                src.at<uchar>(Point(x * 3 + i, y)) = saturate_cast<uchar>(200);
            }
        }
    }

    //画像を表示
    imshow("", src);

    //ウインドウの表示時間(0はキーが押されるまで閉じない)
    waitKey(0);

    return 0;
}

