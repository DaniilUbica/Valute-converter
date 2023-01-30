#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onResultGet(QNetworkReply* reply);

    void on_from_textEdited(const QString &arg1);

    void on_to_textEdited(const QString &arg1);

    void on_sumToConvert_textEdited(const QString &arg1);

    void on_convert_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager* networkManager;

    QJsonDocument receivedData;
    QJsonObject root;
    QJsonObject objValute;

    QString from, to;
    double sumToConvert;
    double rate;

};
#endif // MAINWINDOW_H
