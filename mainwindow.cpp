#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);
    createActions();
    createMenus();
    setWindowTitle(tr("Scribble"));
    resize(500, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Close app
void MainWindow::closeEvent(QCloseEvent *event)
{
    // maybeSave() returns true if no changes have been made
    if(maybeSave()){
        event->accept();
    } else {
      event->ignore();
    }
}

// Open file
void MainWindow::open()
{
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"),
                                                        QDir::currentPath());

        if(!fileName.isEmpty()){
            // Load image on drawing area
            scribbleArea->openImage((fileName));
        }

    }
}

// Save file
void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender()); // Action of the user clicking
    QByteArray fileFormat = action->data().toByteArray(); // Stores the array of bytes of the users data
    saveFile(fileFormat);
}

// Opens a dialog to change penColor
void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if(newColor.isValid()){
        scribbleArea->setPenColor(newColor);
    }
}

// Opens a dialog to change penWidth
void MainWindow::penWidth()
{
    bool ok;
    // min=1, max=50, step=1
    int newWidth = QInputDialog::getInt(this,
                                        tr("Scribble"),
                                        tr("Select pen width: "),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if(ok){
        scribbleArea->setPenWidth(newWidth);
    }
}

// Opens an about dialog
void MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About Scribble"),
                       tr("<p>This is a basic Qt app made for demo purposes.</p>"));
}

// Define menu actions that call functions
void MainWindow::createActions()
{
    // Create the action tied to the menu, and tie it to MainWindow::open()
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    // Get a list of the supported file formats
    // QImageWriter is used to write images to files
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this); // Create an action for each file format
        action->setData(format); // Set an action
        connect(action, SIGNAL(triggered()), this, SLOT(save())); // When clicked call MainWindow::save()
        saveAsActs.append(action); // Attach each file format option menu item to Save As
    }

    // create actions and tie them to slots in MainWindow
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Pen Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("&About &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

// Create Menubar
void MainWindow::createMenus()
{
    // Create SaveAs option and the list of file types
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs){
        saveAsMenu->addAction(action);
    }

    // actions for file, option, and help menu dropdowns
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    // Add items to menubar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
    if(scribbleArea->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save |
                                   QMessageBox::Discard |
                                   QMessageBox::Cancel);

        if(ret == QMessageBox::Save){
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel){
            return false;
        }
    }

    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Get selected file from dialog, and add file formats & extensions
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("&1 Files (*.%2;; All Files(*")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));
    if(fileName.isEmpty()){
        return false;
    } else {
        // call for the file to be saved
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}



