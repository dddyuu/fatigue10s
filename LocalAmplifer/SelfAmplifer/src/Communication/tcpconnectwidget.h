#ifndef TCPCONNECTWIDGET_H
#define TCPCONNECTWIDGET_H

#include <QWidget>

namespace Ui {
class TCPConnectWidget;
}

class TCPConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TCPConnectWidget(QWidget *parent = nullptr);
    ~TCPConnectWidget();
signals:
    void close();
    void start(quint16);
    void connection(QString,quint64);
private slots:
    void on_listen_clicked();
    void on_connection_clicked();

private:
    QString read_ip_address();
    Ui::TCPConnectWidget *ui;
    bool status;

    bool connection_status;
};

#endif // TCPCONNECTWIDGET_H
