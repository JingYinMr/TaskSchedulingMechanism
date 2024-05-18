#include "SimulationUI.h"
#include <QtWidgets/QApplication>

#include <QtCore/QTextCodec>

int main(int argc, char *argv[]) 
{
    QApplication a(argc, argv);

    // ���ñ��ر���ΪϵͳĬ�ϱ���
    QTextCodec *codec = QTextCodec::codecForLocale();
    if (codec) {
        QTextCodec::setCodecForLocale(codec);
    }

    SimulationUI w;
    w.show();
    return a.exec();
}

