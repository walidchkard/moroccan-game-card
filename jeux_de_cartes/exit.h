#ifndef EXIT_H
#define EXIT_H

#include <QDialog>

namespace Ui {
class exit;
}

class exit : public QDialog
{
    Q_OBJECT

public:
    explicit exit(QWidget *parent = nullptr);
    ~exit();

private:
    Ui::exit *ui;
};

#endif // EXIT_H
