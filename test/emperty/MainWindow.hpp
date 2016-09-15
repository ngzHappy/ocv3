#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory/Memory.hpp>
#include <QtWidgets/QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MEMORY_CLASS_NEW_DELETE
};

#endif // MAINWINDOW_HPP
