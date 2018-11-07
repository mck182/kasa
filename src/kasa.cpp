#include "kasa.h"
#include "ui_kasa.h"

kasa::kasa(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::kasa)
{
    m_ui->setupUi(this);
}

kasa::~kasa() = default;
