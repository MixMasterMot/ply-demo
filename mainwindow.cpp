#include <QtWidgets>

#include "mainwindow.h"

#include "happly.h"
using namespace std;

MainWindow::MainWindow()
{

    createActions();

    readSettings();


#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
#endif

    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PLY file"), "", tr("PLY (*.ply)"));
    if (fileName.isEmpty()) {
        return;
    }

    happly::PLYData plyIn(fileName.toStdString());

    vector<array<double, 3>> vPos = plyIn.getVertexPositions();

    double minX = vPos[0][0];
    double minY = vPos[0][1];
    double minZ = vPos[0][2];

    double maxX = vPos[0][0];
    double maxY = vPos[0][1];
    double maxZ = vPos[0][2];

    
    for(array<double, 3> lst :vPos){
        
        if (minX > lst[0]) {
            minX = lst[0];
        }
        else if (maxX < lst[0]) {
            maxX = lst[0];
        }

        if (minY > lst[1])
        {
            minY = lst[1];
        }
        else if (maxY < lst[1]) {
            maxY = lst[1];
        }

        if (minZ > lst[2]){
            minZ = lst[0];
        }
        else if (maxZ < lst[2]) {
            maxZ = lst[0];
        }
    }

    QString minStr = "min X: " + QString::number(minX) + " min Y: " + QString::number(minY) + " min Z: " + QString::number(minZ);
    QString maxStr = "max X: " + QString::number(maxX) + " max Y: " + QString::number(maxY) + " max Z: " + QString::number(maxZ);
    QMessageBox::about(this, tr("About Application"), tr("%1\n%2").arg(minStr, maxStr));
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}