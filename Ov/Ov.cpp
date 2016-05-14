// Ov.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <string>
#include <opencv2/opencv.hpp> // インクルードファイル指定
#include <opencv2/opencv_lib.hpp> // 静的リンクライブラリの指定
#include "Labeling.h"

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

bool isRedHSV(Mat hsv, int index) {
    if (hsv.data[index] >= 150 && hsv.data[index] <= 180 && // H
        hsv.data[index + 1] >= 70 && // S
        hsv.data[index + 2] >= 70) { // V
        return true;
    }
    return false;
}

void extractColor(Mat org) {
    if (org.empty()) throw exception();

    //画像を入れるMatの宣言
    Mat hsv;    //HSV変換後の画像

    //ラベリング関連の変数
    LabelingBS labeling;
    RegionInfoBS *ri;

    //画像サイズの指定
    const int height = org.rows;
    const int width = org.cols;

    Mat result(height, width, CV_8UC1);    //青部分を切り出した白黒画像
    short *dst = new short[width * height];//ラベリング出力先

    result = Mat::zeros(height, width, CV_8UC1);

    //HSV画像への変換
    //この方法ではHが0-180の範囲で表される
    cvtColor(org, hsv, CV_BGR2HSV);

    for (int y = 0; y < height;y++) {
        for (int x = 0; x < width; x++) {
            int index = hsv.step*y + (x * 3);
            if (!isRedHSV(hsv, index)) continue;

            //該当色範囲を白くする
            result.data[result.step*y + x] = 255;            
        }
    }
    //ラベリングの実行
    labeling.Exec((uchar *)result.data, dst, width, height, true, 30);

    int ltop_x, ltop_y, rbottom_x, rbottom_y;

    for (int i = 0; i < labeling.GetNumOfResultRegions(); i++) {
        ri = labeling.GetResultRegionInfo(i);
        ri->GetMin(ltop_x, ltop_y);
        ri->GetMax(rbottom_x, rbottom_y);
        cout << i << ", min: (" << ltop_x << ", " << ltop_y << ")" << endl;
        cout << i << ", max: (" << rbottom_x << ", " << rbottom_y << ")" << endl;
        rectangle(org, Point(ltop_x, ltop_y), Point(rbottom_x, rbottom_y), Scalar(150, 150, 150));
    }
/*
    //最大の領域を四角で囲む
    ri = labeling.GetResultRegionInfo(0);
    int ltop_x, ltop_y, rbottom_x, rbottom_y;
    ri->GetMin(ltop_x, ltop_y);
    ri->GetMax(rbottom_x, rbottom_y);
    rectangle(org, Point(ltop_x, ltop_y), Point(rbottom_x, rbottom_y), Scalar(255, 255, 255));
*/
    //画像の表示
    imshow("Original", org);
    imshow("Extract", result);
    waitKey(5000);
}

int main()
{
    const string FILES_DIR = "C:/Users/eiurur/Documents/Visual Studio 2015/Projects/Ov/x64/Debug/data/";

    // 相対パスから指定する方法が分からず。
    string original_image_path = FILES_DIR + "original.png";
    string original_rotate_image_path = FILES_DIR + "original_rotate.png";
    string original_image_drew_path = FILES_DIR + "original_drew.png";
    string original_image_drew2_path = FILES_DIR + "original_drew2.png";
    string test_blur_image_path = FILES_DIR + "test_blue.png";

    //cv::Mat gray_img = cv::imread(test_blur_image_path);

    try {
 /*       Mat bin_img = binarize(original_image_path);
        contours(bin_img);
        binarize(original_rotate_image_path);
*/
        // 右半分をトリミング(ROI領域の抽出)

        // 右側の箇所から四角形を抽出し、それぞれclone()

        // 

        // 赤い領域の抽出
        extractColor(imread(original_image_drew_path));

        // OCR

        // 
    }
    catch (exception e) {
        // 受け取った例外のwhat()を呼び出すことで、エラーメッセージが取得できます。
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

