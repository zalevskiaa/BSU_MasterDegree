#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "grahamhull.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    std::vector<point> *points = new std::vector<point>;
    GrahamHull *hull = nullptr;
    void paintEvent(QPaintEvent *event);
    void onInitButtonClicked();
    void onSolveButtonClicked();
    void drawPoints(QPainter *painter);
    void drawGrahamHull(GrahamHull &hull, QPainter *painter);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
