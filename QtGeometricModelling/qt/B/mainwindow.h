#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include "Predicates.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    void redrawPixmap();
    bool cross(double, double);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

//    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);

    bool inside = false;
    std::vector<QPoint> polygon;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
