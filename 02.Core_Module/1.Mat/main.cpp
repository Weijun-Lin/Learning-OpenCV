#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

#define PRINT_MAT(mat) ( cout << (#mat) << " = " << endl << " " << (mat) << endl )

int main(int argc, char **argv)
{
    if (argc != 2) {
        cerr << "usage: Mat image_path" << endl;
        return -1;
    }

    Mat A, C;   // 仅分配矩阵头
    A = imread(argv[1], IMREAD_COLOR);  // imread 内部会调用 cv::Mat::create 分配矩阵
    imshow("A source", A);

    // 不做显示拷贝都是浅拷贝，仅仅拷贝矩阵头信息
    Mat B(A);   // 拷贝构造
    C = A;      // 赋值运算

    cout << "A, B, C using the same memory:" << endl;
    cout << "A.data: " << static_cast<void*>(A.data) << endl
         << "B.data: " << static_cast<void*>(B.data) << endl
         << "C.data: " << static_cast<void*>(C.data) << endl;

    // ROI 也只是 A 一部分的引用
    Mat D (A, Rect(10, 10, 100, 100) );  // 使用 Rect 构建 ROI
    Mat E = A(Range::all(), Range(1,3)); // 使用行列构建 ROI

    Scalar bule(255, 0, 0), red(0, 0, 255);
    D.setTo(bule);
    E.setTo(red);

    imshow("A after DE setTo", A);
    
    // 深拷贝 Mat 
    Mat F = A.clone();
    Mat G;
    A.copyTo(G);

    cout << "using clone or copyTo create new Mat:" << endl;
    cout << "A.data: " << static_cast<void*>(A.data) << endl
         << "F.data: " << static_cast<void*>(F.data) << endl
         << "G.data: " << static_cast<void*>(G.data) << endl;

    // ---------- 显示创建 Mat ----------
    Mat M(2,2, CV_8UC3, Scalar(0,0,255));
    PRINT_MAT(M);

    int sz[3] = {2,2,2};
    Mat L(3,sz, CV_8UC(1), Scalar::all(0));

    M.create(4,4, CV_8UC(2));
    PRINT_MAT(M);

    Mat eye = Mat::eye(4, 4, CV_64F);
    PRINT_MAT(eye);
    Mat ones = Mat::ones(2, 2, CV_32F);
    PRINT_MAT(ones);
    Mat zeros = Mat::zeros(3,3, CV_8UC1);
    PRINT_MAT(zeros);

    C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    C = (Mat_<double>({0, -1, 0, -1, 5, -1, 0, -1, 0})).reshape(3);
    PRINT_MAT(C);

    Mat RowClone = C.row(1).clone();
    PRINT_MAT(RowClone);

    Mat R = Mat(3, 2, CV_8UC3);
    randu(R, Scalar::all(0), Scalar::all(255));
    PRINT_MAT(R);

    // ---------- 输出格式 ----------
    cout << "R (default) = " << endl << R << endl << endl;
    cout << "R (python)  = " << endl << format(R, Formatter::FMT_PYTHON) << endl << endl;
    cout << "R (csv)     = " << endl << format(R, Formatter::FMT_CSV   ) << endl << endl;
    cout << "R (numpy)   = " << endl << format(R, Formatter::FMT_NUMPY ) << endl << endl;
    cout << "R (c)       = " << endl << format(R, Formatter::FMT_C     ) << endl << endl;

    // ---------- 其它常见格式的输出 ---------- 
    Point2f P2d(5, 1);
    cout << "Point (2D) = " << P2d << endl << endl;

    Point3f P3f(2, 6, 7);
    cout << "Point (3D) = " << P3f << endl << endl;

    vector<float> v;
    v.push_back( (float)CV_PI );   v.push_back(2);    v.push_back(3.01f);
    cout << "Vector of floats via Mat = " << Mat(v) << endl << endl;
    
    vector<Point2f> vPoints(20);
    for (size_t i = 0; i < vPoints.size(); ++i) {
        vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
    }
    cout << "A vector of 2D Points = " << vPoints << endl << endl;
    
    waitKey(0);
    return 0;
}