#ifndef KASA_H
#define KASA_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class kasa;
}

class kasa : public QMainWindow
{
    Q_OBJECT

public:
    explicit kasa(QWidget *parent = nullptr);
    ~kasa() override;

private:
    QScopedPointer<Ui::kasa> m_ui;
};

#endif // KASA_H
