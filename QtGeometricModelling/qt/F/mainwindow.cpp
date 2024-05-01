#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPix = QPixmap(this->width(), this->height()-HEIGHT);
    mPix.fill(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(&mPix);

    mPix.fill(Qt::white);

    for (size_t i = 0; i < d_points.size(); i++) {
        painter.setPen(QPen(QBrush(Qt::black), 2));
        painter.drawRect(createDRect(d_points[i]));

        painter.setPen(QPen(QBrush(Qt::gray), 1));
        if(i > 0) {
            painter.drawLine(d_points[i-1], d_points[i]);
        }
    }

    if (d_points.size() >= 4){
        for (int i = 0; i < L; i++){
            painter.setPen(QPen(Qt::magenta));
            for(double t = 0; t < 1; t+=0.001){
                QPoint val =
                        pow(1-t, 3)*b_points[i*3] +
                        3*t*pow(1-t, 2)*b_points[i*3+1] +
                        3*t*t*(1-t)*b_points[i*3+2] +
                        t*t*t*b_points[i*3+3];
                painter.drawEllipse(val, 1, 1);
            }
        }
    }

    redrawPixmap();
}

void MainWindow::redrawPixmap() {
    QPainter painter(this);
    painter.drawPixmap(0, 0, mPix);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(indexOfDragStartPoint != -1){
        d_points[indexOfDragStartPoint] = event->pos();
        updateBVectorGeneral();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (indexOfDragStartPoint != -1){
        d_points[indexOfDragStartPoint]=event->pos();
        indexOfDragStartPoint = -1;
        updateBVectorGeneral();
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    auto curPosition = event->pos();
    for (size_t i = 0; i < d_points.size(); i++){
        tempRect = createDRect(d_points[i]);
        if(tempRect.contains(curPosition)){
            indexOfDragStartPoint = i;
            break;
        }
    }

    if (indexOfDragStartPoint == -1){
        d_points.push_back(curPosition);
        updateBVectorOnBuild();
    }

    update();
}

void MainWindow::updateBVectorOnBuild(){
    if (d_points.size() <= 4){
        b_points.resize(d_points.size());
        for(size_t i = 0; i < d_points.size(); i++){
            b_points[i] = d_points[i];
        }

        if (d_points.size() == 4){
            updateLAndDeltasOnBuild();
        }
    }
    else {
        updateLAndDeltasOnBuild();
        updateBVectorGeneral();
    }
}

void MainWindow::updateBVectorGeneral(){
    b_points[0] = d_points[0];
    b_points[1] = d_points[1];

    if (L != 1) {
        b_points[2] = (deltas[1] * d_points[1] + deltas[0] * d_points[2] ) / (deltas[0] + deltas[1]);
    }

    for(int i = 1; i < L; i++){
        if(i != 1) {
            b_points[3*i-1] = (deltas[i]*d_points[i]+(deltas[i-2]+deltas[i-1])*d_points[i+1]) / (deltas[i-2]+deltas[i-1]+deltas[i]);
        }
        b_points[3*i+1] = ((deltas[i] + deltas[i+1]) * d_points[i+1] + deltas[i-1] * d_points[i+2]) / (deltas[i-1] + deltas[i] + deltas[i+1]);
        b_points[3*i] = (deltas[i] * b_points[3*i-1]) / (deltas[i-1] + deltas[i]) + (deltas[i-1] * b_points[3*i+1]) / (deltas[i-1] + deltas[i]);
    }

    if(L != 1){
        b_points[3*L-2] = (deltas[L-1] * d_points[L] + deltas[L-2] * d_points[L+1]) / (deltas[L-2] + deltas[L-1]);
    }

    b_points[3*L-1] = d_points[L+1];
    b_points[3*L] = d_points.back();
}

void MainWindow::updateLAndDeltasOnBuild(){
    deltas.push_back(DELTA);
    L = d_points.size() - 3;
    b_points.resize(3*L+1);
}

QRect MainWindow::createDRect(QPoint dPoint){
    return QRect(dPoint.x(), dPoint.y(), 9, 9);
}

void MainWindow::on_pushButton_clicked() {
    mPix.fill(Qt::white);
    d_points.clear();
    b_points.clear();
    deltas.clear();
    indexOfDragStartPoint = -1;
    update();
}
