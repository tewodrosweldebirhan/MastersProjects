#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

protected:

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_caclulateHarrisBtn_clicked();

    void on_chooseFileBtn_clicked();

private:
    Ui::Window *ui;
};

#endif // WINDOW_H
