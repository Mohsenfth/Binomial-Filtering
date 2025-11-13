#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <time.h>
#include <iomanip>

using namespace std;
using namespace cv;

void filter(Mat &input, Mat mask, Mat &output) {
    // Summe der Maske berechnen
    int sum = 0;
    for (int i = 0; i < mask.rows; i++) {
        for (int j = 0; j < mask.cols; j++) {
            sum += mask.at<float>(i, j);
        }
    }

    int rows = input.rows;
    int cols = input.cols;
    int halfMaskRows = mask.rows / 2;
    int halfMaskCols = mask.cols / 2;

    output = Mat(rows, cols, input.type());

    // Über alle Pixel des Bildes laufen
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float np = 0;

            // Über die Maske laufen
            for (int m = -halfMaskRows; m <= halfMaskRows; m++) {
                for (int n = -halfMaskCols; n <= halfMaskCols; n++) {
                    // Prüfen ob innerhalb des Bildes
                    int row = i + m;
                    int col = j + n;

                    if (row >= 0 && row < rows && col >= 0 && col < cols) {
                        np += input.at<uchar>(row, col) * mask.at<float>(m + halfMaskRows, n + halfMaskCols);
                    }
                }
            }

            np = np / sum;
            output.at<uchar>(i, j) = (uchar)np;
        }
    }
}

int main() {
    Mat grau;
    grau = imread("../plant.png", 0);

    if (grau.empty()) {
        cout << "Bild konnte nicht geladen werden!" << endl;
        return -1;
    }

    // 1. Dreifache 3x3-Binomial-Filterung
    float array3x3[] = {1, 2, 1,
                        2, 4, 2,
                        1, 2, 1};

    Mat mask3x3(3, 3, CV_32F, array3x3);
    Mat result1, temp1, temp2;

    const clock_t a_start = clock();
    filter(grau, mask3x3, temp1);
    filter(temp1, mask3x3, temp2);
    filter(temp2, mask3x3, result1);
    const clock_t a_end = clock();

    // 2. Einmalige 7x7-Binomialfilterung
    float array7x7[] = {1,  6,  15, 20, 15,  6,  1,
                        6, 36,  90, 120, 90, 36,  6,
                       15, 90, 225, 300, 225, 90, 15,
                       20, 120, 300, 400, 300, 120, 20,
                       15, 90, 225, 300, 225, 90, 15,
                        6, 36,  90, 120, 90, 36,  6,
                        1,  6,  15, 20, 15,  6,  1};
    Mat mask7x7(7, 7, CV_32F, array7x7);
    Mat result2;

    const clock_t b_start = clock();
    filter(grau, mask7x7, result2);
    const clock_t b_end = clock();

    // 3. Separable Filter: 7x1 und dann 1x7
    float array7[] = {1, 6, 15, 20, 15, 6, 1};
    Mat mask7x1(7, 1, CV_32F, array7);
    Mat mask1x7(1, 7, CV_32F, array7);

    Mat result3, temp3;

    const clock_t c_start = clock();
    filter(grau, mask7x1, temp3);
    filter(temp3, mask1x7, result3);
    const clock_t c_end = clock();


    // Werte für einen Bildpunkt ausgeben (z.B. Pixel bei 100, 100)
    int testRow = 100;
    int testCol = 100;


    cout << "Pixelwerte an Position (" << testRow << ", " << testCol << "):" << endl;
    cout << "Original: " << (int)grau.at<uchar>(testRow, testCol) << endl;
    cout << "3x 3x3 Binomial: " << (int)result1.at<uchar>(testRow, testCol) << endl;
    cout << "1x 7x7 Binomial: " << (int)result2.at<uchar>(testRow, testCol) << endl;
    cout << "Separabel (7x1 + 1x7): " << (int)result3.at<uchar>(testRow, testCol) << endl;

    cout << fixed << setprecision(2) << "CPU time used for 3x 3x3 Binomial : "
              << 1000.0 * (a_end - a_start) / CLOCKS_PER_SEC << "ms\n";
    cout << fixed << setprecision(2) << "CPU time used for 1x 7x7 Binomial : "
              << 1000.0 * (b_end - b_start) / CLOCKS_PER_SEC << "ms\n";
    cout << fixed << setprecision(2) << "CPU time used Separabel 7x1+1x7 : "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << "ms\n";



    // Bilder anzeigen
    imshow("Original", grau);
    imshow("3x 3x3 Binomial", result1);
    imshow("1x 7x7 Binomial", result2);
    imshow("Separabel 7x1+1x7", result3);
    waitKey(0);

    return 0;
}
