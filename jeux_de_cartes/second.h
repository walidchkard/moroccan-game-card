#ifndef SECOND_H
#define SECOND_H

class MainWindow;
#include <QWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QPaintEvent>
#include <QPointF>
#include <QList>
#include "carte.h"
#include "deck.h"
#include <QtMultimedia>

namespace Ui {
class second;
}

class QGraphicsScene;
class QGraphicsRectItem;

class second : public QGraphicsView
{
    Q_OBJECT

public:
    explicit second(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~second();
    QGraphicsRectItem *findEmptyTableRect();
    QVector<QGraphicsRectItem *> buttons;
    QGraphicsScene *scene;
    void checkGameResult();
    void checkAndPerformActions();
    QTimer autoAddCardsTimer;
    void showMessage(const QString &message);

private slots:
    void on_bt3_clicked();
    void addCarteToRectFromDeck(int row, int col, bool showFront);
    void setDeckImage();
    bool areRectsEmpty(int row, int col);
    void computerMove();

signals:
    void allRectsEmptySignal();

private:
    Ui::second *ui;
    MainWindow *w;
    QList<carte *> addedCards;
    Deck deck;
    QPixmap deckPixmap;
    QMediaPlayer M_player;
    QList<carte *> tableCards;
    QList<carte *> computerCollectedCards;


Q_SIGNALS:
    void carteClicked(carte* clickedCarte);
};

#endif // SECOND_H
