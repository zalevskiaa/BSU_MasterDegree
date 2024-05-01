#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new QGraphicsScene();

    frame_width = ui->graphicsView->width();
    frame_height = ui->graphicsView->height();

    QPixmap pixM = QPixmap(frame_width, frame_height);
    pixM.fill(Qt::white);
    bg = scene->addPixmap(pixM);
    bgi = pixM;
    ui->graphicsView->setScene(scene);
}


void MainWindow::redrawBackground(){
    bg->setPixmap(bgi);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::drawLines(QVector<QLine> lines, QColor color=Qt::black) {
    auto img = bg->pixmap().toImage();
    QPainter nPainter(&img);
    nPainter.setPen(color);
    nPainter.drawLines(lines);
    bg->setPixmap(QPixmap::fromImage(img));
}

//void MainWindow::drawPoints(std::vector<Point> points) {
//    auto img = bg->pixmap().toImage();
//    QPainter qPainter(&img);
//    qPainter.setPen(Qt::red);
//    for (auto point: points){
//        qPainter.drawEllipse(QPointF(point.x, point.y), 2, 2);
//    }
//    bg->setPixmap(QPixmap::fromImage(img));
//}

QVector<QLine> MainWindow::genLines() {
    QVector<QLine> res;
    res.reserve(numLines);

    for (int i = 0; i < numLines; i++){
        int x1 = rand() % frame_width;
        int y1 = rand() % frame_height;
        int x2, y2;
        if (rand() % 2 < 1){
            x2 = x1;
            y2 = y1 + rand() % 10 + 10;
        } else {
            y2 = y1;
            x2 = x1 + rand() % 10 + 10;
        }
        res.push_back(QLine(QPoint(x1, y1), QPoint(x2, y2)));
    }
    return res;
}


void MainWindow::on_pushButton_clicked() {
    linesDrawn = false;
    redrawBackground();
    numLines = ui->lineEdit->text().toInt();
    lines = genLines();
    drawLines(lines);
    linesDrawn = true;
}


//QVector<QVector<QLine>> MainWindow::computeIntersections() {
QVector<QLine> MainWindow::computeIntersections() {

    std::vector<Line> converted_lines;
    converted_lines.reserve(lines.size());
    for (QLine line: lines){
        converted_lines.push_back(Line(Point(line.x1(), line.y1()), Point(line.x2(), line.y2())));
    }

    std::vector<std::pair<int, int>> intersected_lines_inds_pairs = ComputeIntersections(converted_lines);


    QVector<QLine> result;
    for (std::pair<int, int> i : intersected_lines_inds_pairs){
        result.push_back(lines[i.first]);
        result.push_back(lines[i.second]);
    }
//    QVector<QVector<QLine>> result;
//    result.reserve(intersected_lines_inds_pairs.size());
//    for (std::pair<int, int> i : intersected_lines_inds_pairs){
//        result.push_back(QVector<QLine>{lines[i.first], lines[i.second]});
//    }
    return result;
}

void MainWindow::on_pushButton_2_clicked() {
    QVector<QLine> intersected_lines_pairs = computeIntersections();
    drawLines(intersected_lines_pairs, Qt::magenta);
//    for (auto pair : intersected_lines_pairs){
//         drawLines(pair, Qt::magenta);
//    }
}
