#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>
#include <ctime>
#include <iostream>
#include <vector>
#include <QGenericMatrix>
#include <math.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef QGenericMatrix<1, 3, float> QMatrix1x3;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void redrawPixmap();
    void update_edges(QMatrix3x3);
    void paintEvent(QPaintEvent *);

private slots:
    void on_xSlider_valueChanged(int value);
    void on_ySlider_valueChanged(int value);
    void on_zSlider_valueChanged(int value);

private:
    std::vector<std::pair<QMatrix1x3, QMatrix1x3>> edges;
    QPainter painter;
    QPixmap mPix;
    Ui::MainWindow *ui;
    int HEIGHT = 150;
    const double angle = M_PI / 4;

    int xv = 0;
    int yv = 0;
    int zv = 0;
};

#endif // MAINWINDOW_H
