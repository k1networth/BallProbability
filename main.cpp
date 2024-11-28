#include <QApplication>
#include <QString>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Проверяем количество аргументов
    if (argc != 5) {
        qCritical() << "Использование: программа <количетсво_шаров_корзина1> <количество_синих_шаров_корзина1> <количетсво_шаров_корзина2> <количество_синих_шаров_корзина2>";
        return -1;
    }

    // Считываем значения из аргументов
    int ballsBasket1 = QString(argv[1]).toInt();
    int blueBallsBasket1 = QString(argv[2]).toInt();
    int ballsBasket2 = QString(argv[3]).toInt();
    int blueBallsBasket2 = QString(argv[4]).toInt();

    // Проверяем корректность введённых данных
    if (blueBallsBasket1 > ballsBasket1 || blueBallsBasket2 > ballsBasket2) {
        qCritical() << "Ошибка: число синих шаров не может превышать общее количество шаров в корзине.";
        return -1;
    }

    MainWindow window;
    window.setInitialValues(ballsBasket1, blueBallsBasket1, ballsBasket2, blueBallsBasket2);
    window.show();

    return app.exec();
}
