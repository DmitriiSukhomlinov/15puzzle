#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(time(NULL));
    ui->setupUi(this);
    (m_Picture[1] = ui->pic1)->setPixmap(QPixmap (":/01"));
    (m_Picture[2] = ui->pic2)->setPixmap(QPixmap (":/02"));
    (m_Picture[3] = ui->pic3)->setPixmap(QPixmap (":/03"));
    (m_Picture[4] = ui->pic4)->setPixmap(QPixmap (":/04"));
    (m_Picture[5] = ui->pic5)->setPixmap(QPixmap (":/05"));
    (m_Picture[6] = ui->pic6)->setPixmap(QPixmap (":/06"));
    (m_Picture[7] = ui->pic7)->setPixmap(QPixmap (":/07"));
    (m_Picture[8] = ui->pic8)->setPixmap(QPixmap (":/08"));
    (m_Picture[9] = ui->pic9)->setPixmap(QPixmap (":/09"));
    (m_Picture[10] = ui->pic10)->setPixmap(QPixmap (":/10"));
    (m_Picture[11] = ui->pic11)->setPixmap(QPixmap (":/11"));
    (m_Picture[12] = ui->pic12)->setPixmap(QPixmap (":/12"));
    (m_Picture[13] = ui->pic13)->setPixmap(QPixmap (":/13"));
    (m_Picture[14] = ui->pic14)->setPixmap(QPixmap (":/14"));
    (m_Picture[15] = ui->pic15)->setPixmap(QPixmap (":/15"));
    int k = 1;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if ((i == 0) || (j == 0) || (i == 5) || (j == 5))
            {
                m_Map[i][j] = 0;
            }
            else
            {
                m_Map[i][j] = k++;
            }
        }
    }
    m_nStep = 100;
    m_Animation = new QPropertyAnimation;
    m_Animation->setPropertyName("pos");
    m_Animation->setDuration(100);
    m_bVictory = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //Считываем позицию клика, определяем элемент в m_Map
        int x = event->pos().x() / m_nStep + 1;
        int y = event->pos().y() / m_nStep + 1;
        //Зануляем сдвиг
        int moveX = 0;
        int moveY = 0;
        //Определяем, где пустая ячейка
        if (m_Map[y+1][x] == 16) moveY++;
        if (m_Map[y-1][x] == 16) moveY--;
        if (m_Map[y][x+1] == 16) moveX++;
        if (m_Map[y][x-1] == 16) moveX--;
        if ((moveX == 0) && (moveY == 0)) return;//Если рядом пустой ячейки нет, то продолжать выполнение функции не имеет смысла
        //Анимация перемещения
        m_Animation->setTargetObject(m_Picture[m_Map[y][x]]);
        m_Animation->setStartValue(m_Picture[m_Map[y][x]]->pos());
        m_Animation->setEndValue(QPoint((m_Picture[m_Map[y][x]]->x()+ moveX * m_nStep), (m_Picture[m_Map[y][x]]->y()+ moveY * m_nStep)));
        m_Animation->start();
        //Меняем местами пустую ячейку в m_Map с той, по которой кликнули
        int temp = m_Map[y][x];
        m_Map[y][x] = 16;
        m_Map[y+moveY][x+moveX] = temp;
        if (!m_bVictory) VictoryCheck();//Если перемешивали, то проверяем, есть ли выигрыш
    }
}

void MainWindow::VictoryCheck()
{
    int k = 1;
    for (int i = 1; i < 5; i++)
    {
        for (int j = 1; j < 5; j++)
        {
            if (m_Map[i][j] != k) return;
            k++;
        }
    }
    QMessageBox VictoryMsgBox;
    VictoryMsgBox.setWindowIcon(QIcon("://iconVic"));
    VictoryMsgBox.setText("Победа!");
    VictoryMsgBox.setWindowTitle("!!!");
    VictoryMsgBox.exec();
    m_bVictory = true;
}

void MainWindow::on_mix_2_clicked()//Перемешать
{
    on_solve_2_clicked();//Перемешивать начинаем из собранной позиции
    m_nMixRecursionCounter = 0;//Количество рекурсивных вызово
    moveOneBlock(4, 4);//Функция, делающая случайное перемещение одного блока
    m_bVictory = false;
}

void MainWindow::moveOneBlock(int EmptyPosX, int EmptyPosY)
{
    m_nMixRecursionCounter++;
    int moveX = 0;
    int moveY = 0;
    switch (rand() % 4)
    {
    case 0:
        moveY++;
        break;
    case 1:
        moveY--;
        break;
    case 2:
        moveX++;
        break;
    case 3:
        moveX--;
        break;
    }
    if ((EmptyPosX + moveX == 0) || (EmptyPosX + moveX == 5)) moveX = -moveX;//Если на краю
    if ((EmptyPosY + moveY == 0) || (EmptyPosY + moveY == 5)) moveY = -moveY;
    m_Picture[m_Map[EmptyPosY + moveY][EmptyPosX + moveX]]->setGeometry(QRect((EmptyPosX - 1) * m_nStep, (EmptyPosY - 1) * m_nStep, m_nStep, m_nStep));
    int temp = m_Map[EmptyPosY + moveY][EmptyPosX + moveX];
    m_Map[EmptyPosY + moveY][EmptyPosX + moveX] = 16;
    m_Map[EmptyPosY][EmptyPosX] = temp;
    if (m_nMixRecursionCounter < 50)
    {
        moveOneBlock(EmptyPosX + moveX, EmptyPosY + moveY);//Делаем 50 случайных перемещений
    }
}

void MainWindow::on_solve_2_clicked()//Собрать
{
    int k = 1;
    for (int i = 1; i < 5; i++)
    {
        for (int j = 1; j < 5; j++)
        {
            if (k != 16)
            {
            m_Picture[k]->setGeometry(QRect((j - 1) * m_nStep, (i - 1) * m_nStep, m_nStep, m_nStep));
            }
            m_Map[i][j] = k++;
        }
    }
    m_bVictory = true;
}
