#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTextStream>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int HEIGHT = 50;
    int DELTA = 1;

protected:
    void redrawPixmap();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *);
    void updateLAndDeltasOnBuild();
    void updateBVectorGeneral();
    void updateBVectorOnBuild();
    QRect createDRect(QPoint dPoint);

    int indexOfDragStartPoint = -1;
    QRect tempRect;

private:
    std::vector<QPoint> d_points;
    std::vector<QPoint> b_points;
    std::vector<double> deltas;
    int L;

    QPixmap mPix;
    Ui::MainWindow *ui;

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
