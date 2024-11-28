#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setInitialValues(int balls1, int blue1, int balls2, int blue2);

private:
    QGroupBox *createBasketGroup(const QString &title, int totalBalls, int blueBalls);

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QGroupBox *basket1Group;
    QGroupBox *basket2Group;

    QLabel *twoBlueLabel;
    QLabel *twoRedLabel;
    QLabel *oneBlueOneRedLabel;

    QPushButton *extractTwoBallsButton;

    int totalBallsBasket1;
    int blueBallsBasket1;
    int totalBallsBasket2;
    int blueBallsBasket2;

    void moveBallToBasket2();
    void moveBallToBasket1();
    void extractTwoBalls();
    void updateBasketGroup(QGroupBox *group, int totalBalls, int blueBalls, const QString &lastAction);
    void updateLastAction(QGroupBox *basketGroup, const QString &actionText);

    double calculateTwoBlueProbability();
    double calculateTwoRedProbability();
    double calculateOneBlueOneRedProbability();
};

#endif // MAINWINDOW_H
