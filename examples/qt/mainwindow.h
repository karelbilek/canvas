#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"
#include "qimage_paint.h"
#include <QStack>
#include <QSpinBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);

protected:
    void changeEvent(QEvent *e);

private:
    QStack<QPoint> clickstack;

    Ui::MainWindow *ui;

    libcan::canvas c;

 //   QSpinBox sb;

    int expected_type;
    int waiting;
    int waited;
    int polygon_hint;
    int selected_shape;
    bool closing;
    bool clearing;

    libcan::libcan_component carR, carG, carB, carA, fillR, fillG, fillB, fillA;
    libcan::small carSir;


private slots:
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_properties_currentRowChanged(int currentRow);
    void on_shapes_currentRowChanged(int currentRow);
    void on_actionSt_vaj_c_triggered();
    void on_actionZm_nit_triggered();
    void on_pushButton_5_clicked();
    void on_actionZ_konce_triggered();
    void on_actionZe_za_tku_triggered();
    void on_actionPosun_o_10_0_triggered();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_actionPodrobn_j_triggered();
    void on_actionZv_t_vrchn_2x_triggered();
    void on_actionZmen_i_vrchn_2x_triggered();
    void on_actionOto_vrchn_o_30_triggered();
    void on_actionElipsu_triggered();
    void on_actionObd_ln_k_triggered();
    void on_pushButton_2_clicked();
    void on_actionBarva_triggered();
    void on_actionVypnout_triggered();
    void on_actionZapnout_triggered();
    void on_action3_3_triggered();
    void on_action4_3_triggered();
    void on_action5_3_triggered();
    void on_action6_3_triggered();
    void on_action7_3_triggered();
    void on_action8_3_triggered();
    void on_action9_3_triggered();
    void on_action10_3_triggered();
    void on_action11_3_triggered();
    void on_action12_3_triggered();


    void on_action3_2_triggered();
    void on_action4_2_triggered();
    void on_action5_2_triggered();
    void on_action6_2_triggered();
    void on_action7_2_triggered();
    void on_action8_2_triggered();
    void on_action9_2_triggered();
    void on_action10_2_triggered();
    void on_action11_2_triggered();
    void on_action12_2_triggered();

    void on_carSir_valueChanged(int );
    void on_carR_valueChanged(int );
    void on_carG_valueChanged(int );
    void on_carB_valueChanged(int );
    void on_carA_valueChanged(int );
    void on_fillR_valueChanged(int );
    void on_fillG_valueChanged(int );
    void on_fillB_valueChanged(int );
    void on_fillA_valueChanged(int );

    void on_action12_triggered();
    void on_action11_triggered();
    void on_action10_triggered();
    void on_action6_triggered();
    void on_action5_triggered();
    void on_action9_triggered();
    void on_action8_triggered();
    void on_action7_triggered();
    void on_action4_triggered();
    void on_action3_triggered();
    void on_actionKruh_triggered();
    void on_actionUsecku_triggered();
    void on_actionZobraz_triggered();
    void on_pushButton_clicked();

    void paint_to_canvas();

    void change_status();

    void repaint_wanted();

};

#endif // MAINWINDOW_H
