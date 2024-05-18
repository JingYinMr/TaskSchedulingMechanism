#include "SimulationUI.h"
#include <QtWidgets/QApplication>

#include <QtCore/QTextCodec>

int main(int argc, char *argv[]) 
{
    QApplication a(argc, argv);

    // 设置本地编码为系统默认编码
    QTextCodec *codec = QTextCodec::codecForLocale();
    if (codec) {
        QTextCodec::setCodecForLocale(codec);
    }

    SimulationUI w;
    w.show();
    return a.exec();
}

