#include "mainwindow.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    totalBallsBasket1(0), blueBallsBasket1(0),
    totalBallsBasket2(0), blueBallsBasket2(0) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    basket1Group = nullptr;
    basket2Group = nullptr;

    QGroupBox *probabilityGroup = new QGroupBox(this);
    QVBoxLayout *probabilityLayout = new QVBoxLayout(probabilityGroup);

    twoBlueLabel = new QLabel("Вероятность достать 2 синих шара: %", this);
    twoRedLabel = new QLabel("Вероятность достать 2 красных шара: %", this);
    oneBlueOneRedLabel = new QLabel("Вероятность достать 1 красный и 1 синий шары: %", this);

    probabilityLayout->addWidget(twoBlueLabel);
    probabilityLayout->addWidget(twoRedLabel);
    probabilityLayout->addWidget(oneBlueOneRedLabel);

    extractTwoBallsButton = new QPushButton("Извлечь 2 шара из случайной корзины", this);
    probabilityLayout->addWidget(extractTwoBallsButton);

    // Подключаем кнопку "Извлечь 2 шара"
    connect(extractTwoBallsButton, &QPushButton::clicked, this, &MainWindow::extractTwoBalls);

    mainLayout->addWidget(probabilityGroup);

    setWindowTitle("Вероятности шаров");
    resize(600, 400);
}

MainWindow::~MainWindow() = default;

void MainWindow::setInitialValues(int balls1, int blue1, int balls2, int blue2) {
    totalBallsBasket1 = balls1;
    blueBallsBasket1 = blue1;
    totalBallsBasket2 = balls2;
    blueBallsBasket2 = blue2;

    // Удаляем старые группы, если они есть
    delete basket1Group;
    delete basket2Group;

    // Создаём новые группы
    basket1Group = createBasketGroup("Корзина 1", totalBallsBasket1, blueBallsBasket1);
    basket2Group = createBasketGroup("Корзина 2", totalBallsBasket2, blueBallsBasket2);

    mainLayout->insertWidget(0, basket1Group);
    mainLayout->insertWidget(1, basket2Group);

    // Расчет вероятности для извлечения двух шаров
    double probTwoBlue = calculateTwoBlueProbability();
    double probTwoRed = calculateTwoRedProbability();
    double probOneBlueOneRed = calculateOneBlueOneRedProbability();

    // Обновление текста с вероятностями
    twoBlueLabel->setText(QString("Вероятность достать 2 синих шара: %1%").arg(probTwoBlue * 100, 0, 'f', 4));
    twoRedLabel->setText(QString("Вероятность достать 2 красных шара: %1%").arg(probTwoRed * 100, 0, 'f', 4));
    oneBlueOneRedLabel->setText(QString("Вероятность достать 1 красный и 1 синий шар: %1%").arg(probOneBlueOneRed * 100, 0, 'f', 4));
}

QGroupBox* MainWindow::createBasketGroup(const QString &title, int totalBalls, int blueBalls) {
    QGroupBox *group = new QGroupBox(title, this);
    QGridLayout *layout = new QGridLayout(group);

    int redBalls = totalBalls - blueBalls;

    layout->addWidget(new QLabel("Шаров в корзине:", this), 0, 0);
    layout->addWidget(new QLabel(QString::number(totalBalls), this), 0, 1);

    layout->addWidget(new QLabel("Синих шаров в корзине:", this), 1, 0);
    layout->addWidget(new QLabel(QString::number(blueBalls), this), 1, 1);

    layout->addWidget(new QLabel("Красных шаров в корзине:", this), 2, 0);
    layout->addWidget(new QLabel(QString::number(redBalls), this), 2, 1);

    double blueProbability = (totalBalls > 0) ? (100.0 * blueBalls / totalBalls) : 0.0;
    double redProbability = (totalBalls > 0) ? (100.0 * redBalls / totalBalls) : 0.0;

    layout->addWidget(new QLabel(QString("Вероятность достать синий шар: %1%").arg(blueProbability), this), 3, 0, 1, 2);
    layout->addWidget(new QLabel(QString("Вероятность достать красный шар: %1%").arg(redProbability), this), 4, 0, 1, 2);

    layout->addWidget(new QLabel("Последнее действие: -", this), 5, 0, 1, 2);

    QPushButton *moveButton = new QPushButton(title == "Корзина 1" ? "Переложить шар в корзину 2" : "Переложить шар в корзину 1", this);
    layout->addWidget(moveButton, 6, 0, 1, 2);

    if (title == "Корзина 1") {
        connect(moveButton, &QPushButton::clicked, this, &MainWindow::moveBallToBasket2);
    } else {
        connect(moveButton, &QPushButton::clicked, this, &MainWindow::moveBallToBasket1);
    }

    return group;

}

void MainWindow::moveBallToBasket2() {
    if (totalBallsBasket1 > 0) {
        bool isBlue = QRandomGenerator::global()->bounded(totalBallsBasket1) < blueBallsBasket1;
        if (isBlue) {
            blueBallsBasket1--;
            blueBallsBasket2++;
        }
        totalBallsBasket1--;
        totalBallsBasket2++;
        setInitialValues(totalBallsBasket1, blueBallsBasket1, totalBallsBasket2, blueBallsBasket2);

        QString actionText = QString("Переложили %1 шар в Корзину 2").arg(isBlue ? "синий" : "красный");
        updateLastAction(basket1Group, actionText);
        updateLastAction(basket2Group, "Приняли шар");
    }
}

void MainWindow::moveBallToBasket1() {
    if (totalBallsBasket2 > 0) {
        bool isBlue = QRandomGenerator::global()->bounded(totalBallsBasket2) < blueBallsBasket2;
        if (isBlue) {
            blueBallsBasket2--;
            blueBallsBasket1++;
        }
        totalBallsBasket2--;
        totalBallsBasket1++;
        setInitialValues(totalBallsBasket1, blueBallsBasket1, totalBallsBasket2, blueBallsBasket2);

        QString actionText = QString("Переложили %1 шар в Корзину 1").arg(isBlue ? "синий" : "красный");
        updateLastAction(basket2Group, actionText);
        updateLastAction(basket1Group, "Приняли шар");
    }
}


void MainWindow::extractTwoBalls() {
    if (totalBallsBasket1 + totalBallsBasket2 < 2) return;

    QString actionTextBasket1 = "-";
    QString actionTextBasket2 = "-";

    for (int i = 0; i < 2; ++i) {
        bool fromBasket1 = (totalBallsBasket1 > 0 && (totalBallsBasket2 == 0 || QRandomGenerator::global()->bounded(2) == 0));
        if (fromBasket1) {
            bool isBlue = QRandomGenerator::global()->bounded(totalBallsBasket1) < blueBallsBasket1;
            if (isBlue) {
                blueBallsBasket1--;
            }
            totalBallsBasket1--;
            actionTextBasket1 = QString("Извлекли %1 шар").arg(isBlue ? "синий" : "красный");
        } else {
            bool isBlue = QRandomGenerator::global()->bounded(totalBallsBasket2) < blueBallsBasket2;
            if (isBlue) {
                blueBallsBasket2--;
            }
            totalBallsBasket2--;
            actionTextBasket2 = QString("Извлекли %1 шар").arg(isBlue ? "синий" : "красный");
        }
    }

    setInitialValues(totalBallsBasket1, blueBallsBasket1, totalBallsBasket2, blueBallsBasket2);

    updateLastAction(basket1Group, actionTextBasket1);
    updateLastAction(basket2Group, actionTextBasket2);
}


void MainWindow::updateBasketGroup(QGroupBox *group, int totalBalls, int blueBalls, const QString &lastAction) {
    auto layout = qobject_cast<QGridLayout *>(group->layout());
    if (!layout) return;

    int redBalls = totalBalls - blueBalls;

    dynamic_cast<QLabel *>(layout->itemAtPosition(0, 1)->widget())->setText(QString::number(totalBalls));
    dynamic_cast<QLabel *>(layout->itemAtPosition(1, 1)->widget())->setText(QString::number(blueBalls));
    dynamic_cast<QLabel *>(layout->itemAtPosition(2, 1)->widget())->setText(QString::number(redBalls));

    double blueProbability = (totalBalls > 0) ? (100.0 * blueBalls / totalBalls) : 0.0;
    double redProbability = (totalBalls > 0) ? (100.0 * redBalls / totalBalls) : 0.0;

    dynamic_cast<QLabel *>(layout->itemAtPosition(3, 0)->widget())->setText(QString("Вероятность достать синий шар: %1%").arg(blueProbability, 0, 'f', 2));
    dynamic_cast<QLabel *>(layout->itemAtPosition(4, 0)->widget())->setText(QString("Вероятность достать красный шар: %1%").arg(redProbability, 0, 'f', 2));

    dynamic_cast<QLabel *>(layout->itemAtPosition(5, 0)->widget())->setText("Последнее действие: " + lastAction);
}

void MainWindow::updateLastAction(QGroupBox *basketGroup, const QString &actionText) {
    QGridLayout *layout = qobject_cast<QGridLayout *>(basketGroup->layout());
    if (layout) {
        QLabel *lastActionLabel = qobject_cast<QLabel *>(layout->itemAtPosition(5, 0)->widget());
        if (lastActionLabel) {
            lastActionLabel->setText(QString("Последнее действие: %1").arg(actionText));
        }
    }
}

double MainWindow::calculateTwoBlueProbability() {
    int totalBlue = blueBallsBasket1 + blueBallsBasket2;
    int totalRed = (totalBallsBasket1 - blueBallsBasket1) + (totalBallsBasket2 - blueBallsBasket2);
    int totalBalls = totalBlue + totalRed;

    if (totalBalls < 2) return 0.0;

    double probBB = (double)totalBlue / totalBalls * (double)(totalBlue - 1) / (totalBalls - 1);
    return probBB;
}

double MainWindow::calculateTwoRedProbability() {
    int totalBlue = blueBallsBasket1 + blueBallsBasket2;
    int totalRed = (totalBallsBasket1 - blueBallsBasket1) + (totalBallsBasket2 - blueBallsBasket2);
    int totalBalls = totalBlue + totalRed;

    if (totalBalls < 2) return 0.0;

    double probRR = (double)totalRed / totalBalls * (double)(totalRed - 1) / (totalBalls - 1);
    return probRR;
}

double MainWindow::calculateOneBlueOneRedProbability() {
    int totalBlue = blueBallsBasket1 + blueBallsBasket2;
    int totalRed = (totalBallsBasket1 - blueBallsBasket1) + (totalBallsBasket2 - blueBallsBasket2);
    int totalBalls = totalBlue + totalRed;

    if (totalBalls < 2) return 0.0;

    double probBB = calculateTwoBlueProbability();
    double probRR = calculateTwoRedProbability();
    return 1.0 - probBB - probRR;
}
