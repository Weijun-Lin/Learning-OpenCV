## Mat - 基础图像容器
> 参考自：https://docs.opencv.org/4.5.0/d6/d6d/tutorial_mat_the_basic_image_container.html

## Mat

C++ 版 OpenCV（OpenCV 2.0 后）弥补了 C 语言版本中麻烦的内存管理。Mat 只有在需要的时候才会申请内存，并且在 Mat 不被使用时自动释放内存。

Mat 类主要由两部分组成：

- 矩阵头：包括矩阵大小，存储方式，矩阵真实数据的指针等
- 数据：矩阵占用的内存

OpenCV 中 Mat 用于存储图像，往往都会占用很大的空间，频繁进行内存分配十分浪费空间和时间资源。所以 OpenCV 通过引用计数（参考 C++ 内存管理方式）的方式来避免不必要的拷贝。拷贝矩阵时只需要拷贝矩阵头就可以，而不拷贝其真实数据。当有必要拷贝真实数据时，需要显示申明。

```cpp
Mat A, C;                          // creates just the header parts
A = imread(argv[1], IMREAD_COLOR); // here we'll know the method used (allocate matrix)
Mat B(A);                          // Use the copy constructor
C = A;                             // Assignment operator
```

其中 A、B、C 的矩阵数据都指向同一块内存，对任意一个进行操作都会影响其他两个。我们还可以仅创建矩阵中某一部分的引用（ROI，a region of interest），而不进行数据拷贝：

```cpp
Mat D (A, Rect(10, 10, 100, 100) );  // using a rectangle
Mat E = A(Range::all(), Range(1,3)); // using row and column boundaries
```
D、E 都只引用了 A 的一部分，这里并没有开辟新的内存，对 D、E 的修改都会作用到 A 上。

当引用矩阵数据的最后一个对象的生存周期结束时，这块数据才会被释放。需要拷贝矩阵数据必须显示调用拷贝函数（浅拷贝、深拷贝的区别）。`cv::Mat::clone()` 和 `cv::Mat::copyTo()` 函数深拷贝一个矩阵：

```cpp
Mat F = A.clone();
Mat G;
A.copyTo(G);
```

## 存储方式

Mat 存储图像的容器，矩阵的每一个值代表了图像的像素，根据不同的图像格式，存在不同的色彩空间，其像素的表示也会不同。最简单的灰度图像，只需要一个字节就可以表示。

一般图像都有三个通道，通过红绿蓝的组合表示一个颜色，即 RGB。某些格式还会存在第四个通道，alpha (A)，存储了像素的透明度。在不同的应用场景下，颜色会有不同的表达方式，常见的有：

- RGB：最常见的色彩表达方式，但 OpenCV 默认是使用 BGR 色彩空间（蓝色和红色位置调换了）
- HSV、HLS：通过色调 Hue、饱和度 Saturation、明度 Value / 亮度表示颜色，是一种十分自然的颜色表示法，通过忽略最后一个通道可以使得算法对光照不敏感
- YCrCb：用于 JPEG 格式的图像
- CIELab：一种感知均匀的表达方式，可用于表示两个颜色之间的距离


## 显示创建 Mat

除了通过 `cv::imread` 系列函数直接读取图像并创建矩阵外，可以通过多种方式显示创建矩阵：

- `cv::Mat::Mat`，通过 Mat 的构造函数创建
    ```cpp
    Mat M(2,2, CV_8UC3, Scalar(0,0,255));
    cout << "M = " << endl << " " << M << endl << endl;
    /* output:
    M = 
    [  0,   0, 255,   0,   0, 255;
    0,   0, 255,   0,   0, 255]
    */
    ```
    Mat 构造函数四个变量依次为：行、列、数据类型、初始化值。
    数据类型的格式为：

    ```txt
    CV_[The number of bits per item][Signed or Unsigned][Type Prefix]C[The channel number]
    ```

    本质是为一个宏：
    ```c++
    #define CV_8UC	(n)	   CV_MAKETYPE(CV_8U,(n))  
    #define CV_8UC1        CV_MAKETYPE(CV_8U,1)
    ```

    `CV_8uC3` 代表这个矩阵每一个元素由 3 个通道，每个通道都为 8 比特无符号数组成。`cv::Scalar` 是一个四元素短向量，其构造函数依次指定每个通道的值，默认为 0。

- 使用 C/C++ 的数组创建
    ```cpp
    int sz[3] = {2,2,2};
    Mat L(3,sz, CV_8UC(1), Scalar::all(0));
    // 超过 2 维的矩阵不可打印
    ```
    通过数组来指定矩阵维度，通过这个构造函数可以构建超过 2 维的矩阵。`Scalar::all` 创建包含相同值的向量。

- `cv::Mat::create` 函数
    ```cpp
    M.create(4,4, CV_8UC(2));
    cout << "M = "<< endl << " "  << M << endl << endl;
    /* output:
    M = 
     [  0,   0,   0,   0, 134,  85,   0,   0;
       0,   0,   0,   0,   0,   0,   0,   0;
       0,   0,   0,   0,   0,   0,   0,   0;
       0,   0,   0,   0,   0,   0,   0,   0]
    */    
    ```
    这个函数仅仅用于改变矩阵大小，按下列算法执行：

    1. 如果当前矩阵大小和数据类型等同于 create 参数则直接返回。否则，释放对当前矩阵的引用，即计数器 -1
    2. 初始化新的矩阵头 
    3. 申请对应的内存
    4. 将当前矩阵的引用计数设置为 1

- matlab 格式的初始化函数 `cv::Mat::zeros`、 `cv::Mat::ones`、 `cv::Mat::eye`
    ```cpp
    Mat E = Mat::eye(4, 4, CV_64F);
    Mat O = Mat::ones(2, 2, CV_32F);
    Mat Z = Mat::zeros(3,3, CV_8UC1);
    ```

- C++ 的逗号表达式初始化或 C++11 的列表初始化方法
    ```cpp
    Mat C = (Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    C = (Mat_<double>({0, -1, 0, -1, 5, -1, 0, -1, 0})).reshape(3);
    /* output:
    C = 
     [0, -1, 0;
     -1, 5, -1;
     0, -1, 0]
    */
    ```

- 通过已有矩阵创建
    ```cpp
    Mat RowClone = C.row(1).clone();
    /* output:
    RowClone = 
     [-1, 5, -1]
    */
    ```

可通过 `cv::randu()` 随机填充矩阵

```cpp
Mat R = Mat(3, 2, CV_8UC3);
randu(R, Scalar::all(0), Scalar::all(255));
/* output:
R = 
 [ 91,   2,  79, 179,  52, 205;
 236,   8, 181, 239,  26, 248;
 207, 218,  45, 183, 158, 101]
*/
```

## 输出格式

OpenCV 可以设置矩阵的输出格式：

- 默认
    ```cpp
    cout << "R (default) = " << endl << R << endl << endl;
    R (default) = 
    [ 91,   2,  79, 179,  52, 205;
     236,   8, 181, 239,  26, 248;
     207, 218,  45, 183, 158, 101]
    ```
- Python
    ```cpp
    cout << "R (python)  = " << endl << format(R, Formatter::FMT_PYTHON) << endl << endl;
    R (python)  = 
    [[[ 91,   2,  79], [179,  52, 205]],
     [[236,   8, 181], [239,  26, 248]],
     [[207, 218,  45], [183, 158, 101]]]
    ```
- CSV
    ```cpp
    cout << "R (csv)     = " << endl << format(R, Formatter::FMT_CSV   ) << endl << endl;
    R (csv)     = 
     91,   2,  79, 179,  52, 205
    236,   8, 181, 239,  26, 248
    207, 218,  45, 183, 158, 101
    ```
- Numpy
    ```cpp
    cout << "R (numpy)   = " << endl << format(R, Formatter::FMT_NUMPY ) << endl << endl;
    R (numpy)   = 
    array([[[ 91,   2,  79], [179,  52, 205]],
           [[236,   8, 181], [239,  26, 248]],
           [[207, 218,  45], [183, 158, 101]]], dtype='uint8')
    ```
- C
    ```cpp
    cout << "R (c)       = " << endl << format(R, Formatter::FMT_C     ) << endl << endl;
    R (c)       = 
    { 91,   2,  79, 179,  52, 205,
     236,   8, 181, 239,  26, 248,
     207, 218,  45, 183, 158, 101}
    ```

感觉 Python 和 numpy 格式的输出比较清楚，默认不容易分辨元素格式。

## 其它常见格式的输出

- 2D Point
    ```cpp
    Point2f P(5, 1);
    cout << "Point (2D) = " << P << endl << endl;
    /* output:
    Point (2D) = [5, 1]
    */
    ```
- 3D Point
    ```cpp
    Point3f P3f(2, 6, 7);
    cout << "Point (3D) = " << P3f << endl << endl;
    /* output:
    Point (3D) = [2, 6, 7]
    */
    ```
- 从 std::vector 构建的 cv::Mat
    ```cpp
    vector<float> v;
    v.push_back( (float)CV_PI);   v.push_back(2);    v.push_back(3.01f);
    cout << "Vector of floats via Mat = " << Mat(v) << endl << endl;
    /* output:
    Vector of floats via Mat = [3.1415927;
     2;
     3.01]    
    */
    ```
- Point 类型的 std::vector
    ```cpp
    vector<Point2f> vPoints(20);
    for (size_t i = 0; i < vPoints.size(); ++i)
        vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
    cout << "A vector of 2D Points = " << vPoints << endl << endl;
    /* output:
    A vector of 2D Points = [0, 0;
     5, 1;
     10, 2;
     15, 3;
     20, 4;
     25, 5;
     30, 6;
     35, 0;
     40, 1;
     45, 2;
     50, 3;
     55, 4;
     60, 5;
     65, 6;
     70, 0;
     75, 1;
     80, 2;
     85, 3;
     90, 4;
     95, 5]
    */
    ```