#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Valute Converter");

    networkManager = new QNetworkAccessManager;

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultGet);

    networkManager->get(QNetworkRequest(QUrl("https://www.cbr-xml-daily.ru/daily_json.js")));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete networkManager;
}

void MainWindow::onResultGet(QNetworkReply* reply)
{
    if (!reply->error()) {
        receivedData = QJsonDocument::fromJson(reply->readAll());

        root = receivedData.object(); // корневой объект json файла
        objValute = root.value(root.keys().at(4)).toObject(); // объект Valute json файла
    }
}


void MainWindow::on_from_textEdited(const QString &arg1)
{
    from = arg1;
}


void MainWindow::on_to_textEdited(const QString &arg1)
{
    to = arg1;
}


void MainWindow::on_sumToConvert_textEdited(const QString &arg1)
{
    sumToConvert = arg1.toDouble();
}


void MainWindow::on_convert_clicked()
{
    try {
        if (from != "RUB" && to != "RUB") {

            QJsonObject from = objValute.value(this->from).toObject(); // получаем объект исходной валюты из списка всех валют объекта Valute json файла
            QJsonObject to = objValute.value(this->to).toObject(); // получаем объект нужной валюты из списка всех валют объекта Valute json файла

            double fromRate = from.value("Value").toDouble();
            double toRate = to.value("Value").toDouble();

            int fromNominal = from.value("Nominal").toInt();
            int toNominal = to.value("Nominal").toInt();

            double ratio = (fromRate / fromNominal) / (toRate / toNominal);

            if (!std::isnormal(rate)) {
                throw "Wrong input!";
            }

            ui->answer->setText(QString::number(sumToConvert) + " " + this->from +" = " + QString::number(sumToConvert * ratio) + " " + this->to);
        }

        else {
            QJsonObject from = objValute.value(this->from).toObject();
            QJsonObject to = objValute.value(this->to).toObject();

            int fromNominal = from.value("Nominal").toInt();
            int toNominal = to.value("Nominal").toInt();

            if (this->to != "RUB") {

                rate = to.value("Value").toDouble() / toNominal;
                ui->answer->setText(QString::number(sumToConvert) + " " +this->from +" = " + QString::number(sumToConvert / rate) + " " +this->to);
            }
            else {
                rate = from.value("Value").toDouble() / fromNominal;
                ui->answer->setText(QString::number(sumToConvert) + " " +this->from +" = " + QString::number(sumToConvert * rate) + " " +this->to);
            }

            if (!std::isnormal(rate)) {
                throw "Wrong input!";
            }

            qDebug() << sumToConvert << " " << rate;
        }
    }
    catch (const char* msg) {
        ui->answer->setText(msg);
    }
}

