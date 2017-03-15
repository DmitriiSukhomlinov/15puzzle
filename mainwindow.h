#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "QPropertyAnimation"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mix_2_clicked();
    void on_solve_2_clicked();

private:
    void mousePressEvent(QMouseEvent *event);
    void moveOneBlock(int EmptyPosX, int EmptyPosY);
    void VictoryCheck();
    Ui::MainWindow *ui;
    QLabel *m_Picture[16];
    QPropertyAnimation *m_Animation;
    int m_Map[6][6];
    int m_nStep;
    int m_nMixRecursionCounter;
    bool m_bVictory;
};

#endif // MAINWINDOW_H
