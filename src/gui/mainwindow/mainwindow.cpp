#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui -> setupUi(this);
    initializeTuned();
    setFormStyle();
    updateProfile();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tunedManager;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event -> button() == Qt::LeftButton)
    {
        mMoving = true;
        mLastMousePosition = event -> pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event -> buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        move(pos() + (event -> pos() - mLastMousePosition));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event -> button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}

void MainWindow::initializeTuned()
{
    tunedManager = new TunedManager();
    availableProfiles = tunedManager -> GetAvailableProfiles();
}

void MainWindow::setFormStyle()
{
    // Setting form style...
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    // Adding shadows for widgets...
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect -> setBlurRadius(9.0);
    shadowEffect -> setColor(QColor(0, 0, 0, 160));
    shadowEffect -> setOffset(4.0);
    ui -> WidgetMain -> setGraphicsEffect(shadowEffect);
}

void MainWindow::updateProfile()
{
    ui -> ProfileSelector -> addItems(availableProfiles);
    ui -> ProfileSelector -> setCurrentText(tunedManager -> GetActiveProfile());
}

void MainWindow::on_ButtonApply_clicked()
{
    QString profile = ui -> ProfileSelector -> currentText();
    if (tunedManager -> SetActiveProfile(profile))
        QMessageBox::information(this, "Tuned Switcher", QString(tr("The active profile was successfully switched to %1.")).arg(profile));
    else
        QMessageBox::critical(this, "Tuned Switcher", QString(tr("Failed to switch the active profile to %1!")).arg(profile));
}

void MainWindow::on_ButtonCancel_clicked()
{
    QApplication::exit(0);
}
