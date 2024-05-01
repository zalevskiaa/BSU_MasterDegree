#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <algorithm>

#include "intersect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsPixmapItem* bg;
    QPixmap bgi;
    void redrawBackground();
    int frame_width;
    int frame_height;
    QGraphicsScene *scene;

    int numLines;
    QVector<QLine> genLines();
    void drawLines(QVector<QLine>, QColor);
//    void drawPoints(std::vector<Point>);
    QVector<QLine> lines;
    bool linesDrawn = false;
    QVector<QLine> computeIntersections();
};

#endif // MAINWINDOW_H
