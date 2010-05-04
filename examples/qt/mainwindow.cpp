#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QLayout>
#include <QString>
#include <QVector>

#include "RGBa.h"
#include "point.h"
#include "all_shapes.h"
#include "plane.h"

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
    ui->shapes_vlastnosti->setVisible(false);

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

void MainWindow::paintEvent(QPaintEvent *w)
{

    /*QImage muj = qimage_from_canvas(c);//.toQImage();
    QPixmap muj2 = QPixmap::fromImage(muj);*/

    plane<RGBa> colors = c.get_plane();


    QPainter painter(this);




    QVector<QRect> rects = w->region().rects();
    for (QVector<QRect>::iterator it = rects.begin(); it < rects.end(); ++it) {
        QImage newPixels(it->width(), it->height(), QImage::Format_RGB32);
        int top = it->top();
        int bottom = it->bottom();
        int left = it->left();
        int right = it->right();

        for (libcan_int y = top; y<=bottom; ++y) {
            for (libcan_int x = left; x<=right; ++x) {
                libcan_component rd,gr,bl,al;

                colors.get(x, y).get_colors(rd, gr, bl, al);

                newPixels.setPixel(x-left,y-top, qRgba(rd,gr,bl,al));
            }
        }
        painter.drawImage(QPoint(it->left(), it->top()), newPixels);
    }



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

void MainWindow::repaint_wanted() {

    if (c.should_paint()) {
        if (c.is_force_paint()) {
            repaint();
        } else {
            QRegion region;
            std::vector<libcan_info> infos = c.what_to_paint().all_infos();
            for (std::vector<libcan_info>::iterator it = infos.begin(); it < infos.end(); ++it) {
                QRegion newregion(it->min_x, it->y, (it->max_x +1 - it->min_x), 1);
                region = region.united(newregion);
            }
            repaint(region);
        }
    }

}

void MainWindow::paint_to_canvas() {

    shape_style style(carSir, RGBa(carR, carG, carB, carA), RGBa(fillR,fillG,fillB,fillA));
                    //I have no clue why Qt creator doesn't like this line of code

    //shape_style style(carSir, RGBa(50, 50, 50, 50), RGBa(50,50,50,50));
    shape_type* type;

    QList<point> points;
    for (int i=0; i<waited; i++) {
        QPoint top = clickstack.pop();
        points << point(top.x(), top.y());
                    //bacha, prehazi to poradi!
    }

    switch (expected_type) {
    case 1:
        type = new segment(points[1], points[0]);
        break;
    case 2:
        type = new disk(points[1], points[0]);
        break;
    case 3:
        type = new polygon(points.toStdList());
        break;
    case 4:
        type = new regular(points[1], points[0], polygon_hint);
        break;
    case 5:
        type = new regular_from_center(points[1], points[0], polygon_hint);
        break;
    case 6:
        type = new rectangle(points[2], points[1], points[0]);
        break;
    case 7:
        type = new elipse(points[2], points[1], points[0]);
        break;
    }

    c.push_front(style, *type);
    repaint_wanted();
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
    repaint_wanted();
}

void MainWindow::on_actionVypnout_triggered()
{
    c.set_antialias(false);
    repaint_wanted();
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
    repaint_wanted();
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
        repaint_wanted();
    }
}

void MainWindow::on_actionZmen_i_vrchn_2x_triggered()
{
    if (c.count()){
        shape& s = c.get_front();
        s.resize(0.5);
        repaint_wanted();
    }
}

void MainWindow::on_actionZv_t_vrchn_2x_triggered()
{
    if (c.count()){
        shape& s = c.get_front();
        s.resize(2);
        repaint_wanted();
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

        repaint_wanted();
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


        repaint_wanted();
    }
}

void MainWindow::on_actionZe_za_tku_triggered()
{
    if (c.count()){
        c.pop_front();
        repaint_wanted();
    }
}

void MainWindow::on_actionZ_konce_triggered()
{
    if (c.count()){
        c.pop_back();
        repaint_wanted();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->poradi->setVisible(false);
    c.change_order(ui->odkud->value()-1, ui->kam->value());
    repaint_wanted();
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

void MainWindow::on_actionSt_vaj_c_triggered()
{
    closing = false;
    std::vector<std::string> vec = c.get_names();
    QStringList list;
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
        list << QString::fromStdString(*it);
    }

    QListWidget* s = ui->shapes;   
    s->addItems(list);

    clearing = true;
    ui->properties->clear();

    ui->shapes_vlastnosti->setVisible(true);

}

void MainWindow::on_shapes_currentRowChanged(int currentRow)
{

    if (!closing) {

        selected_shape = currentRow;
        shape& s = c.get_object(currentRow);
        std::set<std::string> vec = s.get_properties();
        QStringList list;
        for (std::set<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
            list << QString::fromStdString(*it);
        }
        QListWidget* w = ui->properties;
        clearing = true;
        w->clear();
        w->addItems(list);
        clearing = false;

    }

}

void MainWindow::on_properties_currentRowChanged(int currentRow)
{
    if (!clearing) {
        std::string w = c.get_object(selected_shape).get_property(ui->properties->item(currentRow)->text().toStdString());

        ui->value->setText(QString::fromStdString(w));
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    int selected = ui->properties->currentRow();
    std::string key = ui->properties->item(selected)->text().toStdString();
    std::string value = ui->value->text().toStdString();
    c.get_object(selected_shape).set_property(key, value);
    repaint_wanted();
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->shapes_vlastnosti->setVisible(false);
    closing=true;
    while (ui->shapes->count()!=0) {

        ui->shapes->takeItem(0);
    }
}
