#include "entropy_form.h"
#include "ui_entropy_form.h"

entropy_form::entropy_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::entropy_form)
{
    ui->setupUi(this);
}

entropy_form::~entropy_form()
{
    delete ui;
}
