#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
   void setAbstand(float abstand); // Definiere Slot
   void setLinienlaenge(float abstand);
   void on_pushButton_4_clicked();

   void on_pushButton_5_clicked();

   void on_pushButton_6_clicked();

   void on_diagrammtypUmschalter_clicked();

   void on_pushButton_7_clicked();

   void on_pushButton_8_clicked();

   void on_pushButton_9_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum Dtyp {VIERTAKT, LINEAR, MICRO};

    void showAchsen();

private:
    Ui::MainWindow *ui;


   void showPunktevermessung();
   void showTestdaten(Dtyp datentyp);
};

#endif // MAINWINDOW_H
