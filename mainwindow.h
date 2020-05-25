#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class ScribbleArea;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Possible events
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

private:
    // Tie user actions to functions
    void createActions();
    void createMenus();

    // Check if there are changes
    bool maybeSave();
    // Opens save dialog
    bool saveFile(const QByteArray &fileFormat);

    // Drawing area
    ScribbleArea *scribbleArea;
    // Menu widgets
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu; 
    // All Actions
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;


;    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
