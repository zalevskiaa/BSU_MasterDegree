#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    exact::Init();

    setMouseTracking(true);
    centralWidget()->setMouseTracking(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    polygon.push_back(event->pos());
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (inside != cross(event->pos().x(), event->pos().y())) {
        inside = !inside;
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    for (size_t i = 0; i < polygon.size(); i++) {
        painter.drawEllipse(polygon[i], 2, 2);
    }

    QPoint arr[100];
    std::copy(polygon.begin(), polygon.end(), arr);

    painter.setBrush(inside ? Qt::yellow : Qt::white);
    painter.drawPolygon(arr, polygon.size());
}

bool MainWindow::cross(double xh, double yh) {
    REAL point[2] = {xh, yh};
    int count = 0;

    QPoint arr[100];
    std::copy(polygon.begin(), polygon.end(), arr);

    for (size_t j = 0; j < polygon.size(); j++) {
        REAL minPoint[2] = {polygon[j].x() * 1.0, polygon[j].y() * 1.0};
        REAL maxPoint[2] = {polygon[(j + 1) % polygon.size()].x() * 1.0, polygon[(j + 1) % polygon.size()].y() * 1.0};
        if (minPoint[1] > maxPoint[1]) {
            std::swap(minPoint, maxPoint);
        }
        if (maxPoint[1] <= point[1] || minPoint[1] > point[1]) {
            continue;
        }
        exact::position orient = exact::orient2d(minPoint, maxPoint, point);
        if (orient == exact::on) {
            return true;
        }
        if (orient == exact::right) {
            count++;
        }
    }

    return count & 1;
}

void MainWindow::on_pushButton_clicked() {
    polygon.clear();
    update();
}
