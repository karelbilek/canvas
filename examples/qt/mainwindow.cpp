#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QLayout>
#include <QString>
#include <QVector>

#include "RGBa.h"
#include "point.h"
#include "all_shapes.h"

using namespace libcan;//::canvas

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    expected_type=waiting=waited=0;


    c = canvas(800,800,RGBa(255,0,0),false);
    ui->setupUi(this);
    ui->vlastnosti->setVisible(false);
    ui->barvaCanvas->setVisible(false);
    ui->transformace->setVisible(false);
    ui->poradi->setVisible(false);

    ui->carA->setValue(carA=255);
    ui->carR->setValue(carR=0);
    ui->carG->setValue(carG=0);
    ui->carB->setValue(carB=0);

    ui->fillA->setValue(fillA=255);
    ui->fillR->setValue(fillR=255);
    ui->fillG->setValue(fillG=0);
    ui->fillB->setValue(fillB=255);

    ui->carSir->setValue(carSir=10);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    QImage muj = qimage_from_canvas(c);//.toQImage();
    QPixmap muj2 = QPixmap::fromImage(muj);
    QPainter painter(this);

    painter.drawPixmap(0,0,muj2);

}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    clickstack.push(e->pos());
    if (waiting) {
        if (!--waiting) {
            paint_to_canvas();
        }
    }
    change_status();

}

void MainWindow::paint_to_canvas() {

    shape_style style(carSir, RGBa(carR, carG, carB, carA), RGBa(fillR,fillG,fillB,fillA));
                    //I have no clue why Qt creator doesn't like this line of code

    //shape_style style(carSir, RGBa(50, 50, 50, 50), RGBa(50,50,50,50));
    shape_type type;

    QList<point> points;
    for (int i=0; i<waited; i++) {
        QPoint top = clickstack.pop();
        points << point(top.x(), top.y());
                    //bacha, prehazi to poradi!
    }

    switch (expected_type) {
    case 1:
        type = segment(points[1], points[0]);
        break;
    case 2:
        type = disk(points[1], points[0]);
        break;
    case 3:
        type = polygon(points.toStdList());
        break;
    case 4:
        type = regular(points[1], points[0], polygon_hint);
        break;
    case 5:
        type = regular_from_center(points[1], points[0], polygon_hint);
        break;
    case 6:
        type = rectangle(points[2], points[1], points[0]);
        break;
    case 7:
        type = elipse(points[2], points[1], points[0]);
        break;
    }

    c.push_front(style, type);
    repaint();
}


void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->vlastnosti->setVisible(false);
}

void MainWindow::on_actionZobraz_triggered()
{
    ui->vlastnosti->setVisible(true);

}

void MainWindow::change_status()
{
    QString new_status="";
    if (waiting){
        new_status += QString::fromUtf8("Právě kreslím ");
        switch (expected_type) {
        case 1:
            new_status += QString::fromUtf8("úsečku");
            break;
        case 2:
            new_status += QString::fromUtf8("kruh");
            break;
        case 3:
            new_status += QString::fromUtf8("nepravidelný N-úhelník");
            break;
        case 4:
            new_status += QString::fromUtf8("pravidelný N-úhelník");
            break;
        case 5:
            new_status += QString::fromUtf8("pravidelný N-úhelník ze středu");
            break;
        case 6:
            new_status += QString::fromUtf8("obdélník");
            break;
        case 7:
            new_status += QString::fromUtf8("elipsu");
            break;
        default:
            new_status += QString::fromUtf8("neznámý objekt");
            break;
        }
        new_status+=QString::fromUtf8(", ještě ") + QString::number(waiting,10) + QString::fromUtf8(" mi zbývá.");
    }

    ui->status->setText(new_status);


}

void MainWindow::on_actionUsecku_triggered()
{
    waited=waiting=2;
    expected_type=1;
    change_status();
}

void MainWindow::on_actionKruh_triggered()
{
    waited=waiting=2;
    expected_type=2;
    change_status();
}

void MainWindow::on_action3_triggered()
{
    waited=waiting=3;
    expected_type=3;
    change_status();
}

void MainWindow::on_action4_triggered()
{
    waited=waiting=4;
    expected_type=3;
    change_status();
}

void MainWindow::on_action7_triggered()
{
    waited=waiting=7;
    expected_type=3;
    change_status();
}

void MainWindow::on_action8_triggered()
{
    waited=waiting=8;
    expected_type=3;
    change_status();
}

void MainWindow::on_action9_triggered()
{
    waited=waiting=9;
    expected_type=3;
    change_status();
}

void MainWindow::on_action5_triggered()
{
    waited=waiting=5;
    expected_type=3;
    change_status();
}

void MainWindow::on_action6_triggered()
{
    waited=waiting=6;
    expected_type=3;
    change_status();
}

void MainWindow::on_action10_triggered()
{
    waited=waiting=10;
    expected_type=3;
    change_status();
}

void MainWindow::on_action11_triggered()
{
    waited=waiting=11;
    expected_type=3;
    change_status();
}

void MainWindow::on_action12_triggered()
{
    waited=waiting=12;
    expected_type=3;
    change_status();
}


void MainWindow::on_carSir_valueChanged(int w)
{
    carSir=w;
}

void MainWindow::on_carA_valueChanged(int w)
{
    carA=w;
}

void MainWindow::on_carG_valueChanged(int w)
{
    carG=w;
}

void MainWindow::on_carR_valueChanged(int w)
{
    carR=w;
}

void MainWindow::on_carB_valueChanged(int w)
{
    carB=w;
}

void MainWindow::on_fillA_valueChanged(int w)
{
    fillA=w;
}

void MainWindow::on_fillG_valueChanged(int w)
{
    fillG=w;
}

void MainWindow::on_fillR_valueChanged(int w)
{
    fillR=w;
}

void MainWindow::on_fillB_valueChanged(int w)
{
    fillB=w;
}

void MainWindow::on_action5_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=5;
    change_status();
}
void MainWindow::on_action4_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=4;
    change_status();
}

void MainWindow::on_action3_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=3;
    change_status();
}

void MainWindow::on_action6_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=6;
    change_status();
}

void MainWindow::on_action7_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=7;
    change_status();
}

void MainWindow::on_action8_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=8;
    change_status();
}

void MainWindow::on_action9_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=9;
    change_status();
}

void MainWindow::on_action10_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=10;
    change_status();
}

void MainWindow::on_action11_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=11;
    change_status();
}

void MainWindow::on_action12_2_triggered()
{
    waited=waiting=2;
    expected_type=4;
    polygon_hint=12;
    change_status();
}

void MainWindow::on_action6_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=6;
    change_status();
}

void MainWindow::on_action5_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=5;
    change_status();
}

void MainWindow::on_action4_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=4;
    change_status();
}

void MainWindow::on_action3_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=3;
    change_status();
}

void MainWindow::on_action7_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=7;
    change_status();
}

void MainWindow::on_action8_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=8;
    change_status();
}

void MainWindow::on_action9_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=9;
    change_status();
}

void MainWindow::on_action10_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=10;
    change_status();
}

void MainWindow::on_action11_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=11;
    change_status();
}

void MainWindow::on_action12_3_triggered()
{
    waited=waiting=2;
    expected_type=5;
    polygon_hint=12;
    change_status();
}

void MainWindow::on_actionZapnout_triggered()
{
    c.set_antialias(true);
    repaint();
}

void MainWindow::on_actionVypnout_triggered()
{
    c.set_antialias(false);
    repaint();
}

void MainWindow::on_actionBarva_triggered()
{
    libcan_component colors[4];
    c.get_colors(colors, colors+1, colors+2, colors+3);

    ui->R->setValue( colors[0]);
    ui->G->setValue( colors[1]);
    ui->B->setValue( colors[2]);
    ui->A->setValue( colors[3]);

    ui->barvaCanvas->setVisible(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    c.set_RGBa(RGBa(ui->R->value(), ui->G->value(), ui->B->value(), ui->A->value()));

    ui->barvaCanvas->setVisible(false);
    repaint();
}

void MainWindow::on_actionObd_ln_k_triggered()
{
    waited=waiting=3;
    expected_type=6;
    change_status();
}

void MainWindow::on_actionElipsu_triggered()
{
    waited=waiting=3;
    expected_type=7;
    change_status();
}

void MainWindow::on_actionOto_vrchn_o_30_triggered()
{
    if (c.count()){
        shape& s = c.get_front();
        s.rotate(30);
        repaint();
    }
}

void MainWindow::on_actionZmen_i_vrchn_2x_triggered()
{
    if (c.count()){
        shape& s = c.get_front();
        s.resize(0.5);
        repaint();
    }
}

void MainWindow::on_actionZv_t_vrchn_2x_triggered()
{
    if (c.count()){
        shape& s = c.get_front();
        s.resize(2);
        repaint();
    }
}

void MainWindow::on_actionPodrobn_j_triggered()
{
    ui->transformace->setVisible(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    if (c.count()){
        shape& s = c.get_front();
        int posunX = ui->posunX->value();
        int posunY = ui->posunY->value();
        if (posunX || posunY) {

            s.move(point(posunX, posunY));
        }
        if (ui->rotace->value()) {
            s.rotate(ui->rotace->value());
        }

        if (ui->zvetseni->value()) {
            s.resize(ui->zvetseni->value());
        }

        repaint();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->transformace->setVisible(false);
}

void MainWindow::on_actionPosun_o_10_0_triggered()
{
    if (c.count()){
        shape& s = c.get_front();
        s.move(point(10, 0));


        repaint();
    }
}

void MainWindow::on_actionZe_za_tku_triggered()
{
    if (c.count()){
        c.pop_front();
        repaint();
    }
}

void MainWindow::on_actionZ_konce_triggered()
{
    if (c.count()){
        c.pop_back();
        repaint();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->poradi->setVisible(false);
    c.change_order(ui->odkud->value()-1, ui->kam->value());
    repaint();
}

void MainWindow::on_actionZm_nit_triggered()
{
    int i=c.count();
    if (!i){
        ui->status->setText(QString::fromUtf8("Nejde to, je málo objektů :-("));
    } else {
        ui->poradi->setVisible(true);
        ui->odkud->setMaximum(i);
        ui->kam->setMaximum(i);
        ui->odkud->setMinimum(1);
    }
}
