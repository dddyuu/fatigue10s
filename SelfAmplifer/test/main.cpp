
#include "QApplication"
#include "selfamplifer.h"
#include "QWidget"
#include "QPushButton"
#include "QDebug"
class Test :public QWidget
{
    //Q_OBJECT
public:
    Test();
    ~Test();


private:
    QPushButton* button;
    SelfAmplifer w;
};

Test::Test()
{
    w.getConnectWidget()->show();
    w.start();
    button = new QPushButton(this);
    connect(button, &QPushButton::clicked, this, [=]() {
        QList<QList<double>> data = w.getChartData();
        QList<QList<double>> raw_data = w.getRawData();
        if (!data.isEmpty())
        {
            qDebug() << data[0];
            qDebug() << raw_data[0].mid(0, 32);
        }
        });

}

Test::~Test()
{
}
int main(int args, char** argv)
{
    QApplication app(args, argv);
    Test t;
    t.show();
    return  app.exec();
}
