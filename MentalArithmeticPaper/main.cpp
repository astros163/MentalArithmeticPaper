#include "MentalArithmeticPaper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MentalArithmeticPaper w;
    w.show();
    return a.exec();
}
