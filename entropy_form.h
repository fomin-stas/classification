#ifndef ENTROPY_FORM_H
#define ENTROPY_FORM_H

#include <QWidget>

namespace Ui {
class entropy_form;
}

class entropy_form : public QWidget
{
    Q_OBJECT

public:
    explicit entropy_form(QWidget *parent = 0);
    ~entropy_form();

private:
    Ui::entropy_form *ui;
};

#endif // ENTROPY_FORM_H
