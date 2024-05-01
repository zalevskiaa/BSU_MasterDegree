#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QMouseEvent>

#include <vector>

#include "node.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent*);

    void regeneratePoints(int N);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap pixmap;

    std::vector<QPoint> points;
    uint count = 0;
    Node* tree = nullptr; // 2-d-tree
    QRect rect, t_rect;

    QPoint dragStart;
    bool rectCreated = false;
    bool pointsDrawn = false;
    bool pointsGenerated = false;
};

#endif // MAINWINDOW_H
