// deck.h
#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QList>
#include "carte.h"

class Deck : public QObject
{
    Q_OBJECT

public:
    explicit Deck(QObject *parent = nullptr);
    void initializeDeck();
    void shuffleDeck();
    carte* dealCard();
    bool isEmpty() const;

private:
    QList<carte*> cards;
    int currentIndex;

signals:

};

#endif // DECK_H
