#include <cmath>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

inline const constinit size_t jointsCount = 6;
inline const constinit size_t matrixSize = 4;

//DH parameters
constinit const std::array<double, jointsCount> theta = {15 * M_PI / 180.0, -50 * M_PI / 180.0, -60 * M_PI / 180.0, 95 * M_PI / 180.0, 50 * M_PI / 180.0, 0 * M_PI / 180.0};
constinit const std::array<double, jointsCount> a = {0, -0.8, -0.590, 0, 0, 0};
constinit const std::array<double, jointsCount> d = {0.213, 0.193, 0.16, 0.25, 0.28, 0.25};
constinit const std::array<double, jointsCount> alpha = {M_PI / 2, 0, 0, M_PI / 2, -M_PI / 2, 0};

//function for multiply 4x4 matrix
constexpr std::array<std::array<double, matrixSize>, matrixSize> multiplyMatrix
    (const std::array<std::array<double, matrixSize>, matrixSize>& A,
     const std::array<std::array<double, matrixSize>,matrixSize>& B)
{
    std::array<std::array<double, matrixSize>, matrixSize> C {0};
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < matrixSize; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

//function for creation transformaion matrix by DH-parameters
constexpr std::array<std::array<double, matrixSize>, matrixSize> computeDHMatrix(double theta, double a, double d, double alpha)
{
    return {{
        {cos(theta), -sin(theta) * cos(alpha), sin(theta) * sin(alpha), a * cos(theta)},
        {sin(theta), cos(theta) * cos(alpha), -cos(theta) * sin(alpha), a * sin(theta)},
        {0, sin(alpha), cos(alpha), d},
        {0, 0, 0, 1}
    }};
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // create transformation matrix (identity matrix)
    std::array<std::array<double, matrixSize>, matrixSize> T_total = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};

    //compute result matrix
    for (size_t i = 0; i < jointsCount; ++i) {
        auto T = computeDHMatrix(theta[i], a[i], d[i], alpha[i]);
        T_total = multiplyMatrix(T_total, T);
    }

    //take position of final element
    double x = T_total[0][3];
    double y = T_total[1][3];
    double z = T_total[2][3];

    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QLabel *label = new QLabel(QString("Position: x = %1, y = %2, z = %3")
                                   .arg(x)
                                   .arg(y)
                                   .arg(z));
    layout->addWidget(label);
    window.setLayout(layout);
    window.show();

    return app.exec();
}

