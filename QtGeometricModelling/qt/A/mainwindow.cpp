#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include <vector>

std::vector<QPoint> generatePoints(int N, int width, int height) {
    std::vector<QPoint> points;
    for (int i = 0; i < N; i++)
        points.push_back({rand() % width, rand() % height});
    return points;
}

void MainWindow::regeneratePoints(int N) {
    pointsGenerated = false;
    points.clear();
    points = generatePoints(N, ui->widget->width(), ui->widget->height());

    if (tree != nullptr) {
        delete tree;
    }
    tree = new Node(points);

    pointsDrawn = false;
    rectCreated = false;

    QPoint tmp(0, 0);
    rect.setTopLeft(tmp);
    rect.setBottomRight(tmp);
    t_rect = rect;

    pixmap.fill(Qt::white);

    pointsGenerated = true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pixmap = QPixmap(ui->widget->width(), ui->widget->height());

    pixmap.fill(Qt::white);
}

void MainWindow::on_pushButton_clicked() {
    const QString text = ui->lineEdit->text();

    bool ok = true;
    count = text.toInt(&ok);
    if (!ok) return;

    this->regeneratePoints(count);
    update();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!rectCreated) {
        rect.setTopLeft(event->pos());
        rect.setBottomRight(event->pos());
    } else {
        dragStart = event->pos();
        t_rect = rect;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!rectCreated) {
        rect.setBottomRight(event->pos());
    } else {
        QPoint diff = event->pos() - dragStart;

        rect.setBottomRight(t_rect.bottomRight() + diff);
        rect.setBottomLeft(t_rect.bottomLeft() + diff);
        rect.setTopLeft(t_rect.topLeft() + diff);
        rect.setTopRight(t_rect.topRight() + diff);
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent*) {
    if (!rectCreated) {
        rectCreated = true;
        if (rect.bottomRight().x() < rect.topLeft().x()) {
            QPoint temp = rect.bottomRight();
            rect.setBottomRight(rect.topLeft());
            rect.setTopLeft(temp);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QPainter pixPainter(&pixmap);

    QPen pen;

    if (pointsGenerated && !pointsDrawn) {
        pen.setColor(Qt::black);
        pixPainter.setPen(pen);
        for (QPoint const& point : points) {
            pixPainter.drawPoint(point);
        }
        pointsDrawn = true;
    }

    painter.drawPixmap(0, 0, pixmap);

    if (pointsGenerated) {
        pen.setColor(Qt::red);
        painter.setPen(pen);

        auto const points = tree->insideRectangle(&rect);
        this->ui->label->setText(QString::number(points.size()));
        for (auto const& point : points)
            painter.drawPoint(point);
    }
}

MainWindow::~MainWindow()
{
    delete tree;
    delete ui;
}
