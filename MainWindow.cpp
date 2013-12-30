//#include <QtGui>
//#include <QtOpenGL>
#include <Qt\qapplication.h>
#include <Qt\qwidget.h>
#include <Qt\qmainwindow.h>
#include <Qt\qaction.h>
#include <Qt\qdialog.h>
#include <Qt\qfiledialog.h>
#include <Qt\qinputdialog.h>
#include <Qt\qmessagebox.h>
#include <QtGui\qmenubar.h>
#include <QtGui\qtoolbar.h>
#include <QtOpenGL\qgl.h>
#include <QtGui\qscrollarea.h>
#include <QtGui\qslider.h>
#include <QtGui\qlabel.h>
#include <QtGui\qgridlayout.h>
#include <QtGui\qgroupbox.h>
#include <QtGui\qradiobutton.h>

#include <iostream>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central_widget = new QWidget;
    setCentralWidget(central_widget);

    //numSliderLabel = new QLabel;
    //numSliderLabel->setText("change split level");
    //zoomSliderLabel = new QLabel;
    //zoomSliderLabel->setText("change size");

    //numGroupBox = createNumGroupBox();

    //createActions();
    //createMenus();
    //setupToolBar();

//    centralLayout->addWidget(pixmapLabelArea, 0, 1);
//    centralLayout->addWidget(xSlider, 1, 0, 1, 2);
//    centralLayout->addWidget(zoomSliderLabel1, 2, 0);
    //QGridLayout *sliderLayout1 = new QGridLayout;
    //sliderLayout1->addWidget(numSliderLabel, 0, 0);
    //sliderLayout1->addWidget(numGroupBox, 0, 1, 1, 5);
    //centralLayout->addLayout(sliderLayout1, 1, 0, 1, 2);

	//QGridLayout *controlLayout = new QGridLayout;
 //   controlLayout->addWidget(zoomSliderLabel, 0, 0);
 //   controlLayout->addWidget(zoomSlider, 0, 1, 1, 5);
	//controlLayout->addWidget(numGroupBox, 0, 6);

	puzzle_widget_ = new PuzzleWidget;
    QScrollArea *puzzle_area = new QScrollArea;
    puzzle_area->setWidget(puzzle_widget_);
    puzzle_area->setWidgetResizable(true);
    puzzle_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    puzzle_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    puzzle_area->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    puzzle_area->setMinimumSize(50, 50);

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(puzzle_area, 0, 0);
//   centralLayout->addWidget(sliderLayout2, 3, 0, 1, 2);
    central_widget->setLayout(centralLayout);

    setWindowTitle(tr("8_puzzle"));
    resize(1024, 768);

	game_setup_dialog_ = new GameSetupDialog(this);
	createActions();
	createMenus();
	createToolBars();

	loadBackground();
	showGameSetupDialog();
}

MainWindow::~MainWindow()
{  
}

void MainWindow::grabFrameBuffer()
{
}

void MainWindow::createActions()
{
	connect(game_setup_dialog_, SIGNAL(gameSetup(int, char)), this, SLOT(startNewGame(int, char)));
	connect(puzzle_widget_, SIGNAL(startAnotherGame()), this, SLOT(showGameSetupDialog()));
}

void MainWindow::createMenus()
{
	//QMenu *game_menu = menuBar()->addMenu(tr("&Game"));
	//QAction *new_game_action = game_menu->addAction(tr("&New Game"));
	//connect(new_game_action, SIGNAL(triggered()), this, SLOT(showGameSetupDialog()));
}

void MainWindow::createToolBars()
{
	QToolBar *new_game_bar = addToolBar(tr("New Game"));
	QAction *new_game_action = new_game_bar->addAction(tr("&New Game"));
	connect(new_game_action, SIGNAL(triggered()), this, SLOT(showGameSetupDialog()));

	QToolBar *reset_game_bar = addToolBar(tr("Reset Game"));
	QAction *reset_game_action = reset_game_bar->addAction(tr("&Reset Game"));
	connect(reset_game_action, SIGNAL(triggered()), puzzle_widget_, SLOT(resetGame()));

	QToolBar *hint_bar = addToolBar(tr("Hint"));
	QAction *hint_action = hint_bar->addAction(tr("&Hint"));
	connect(hint_action, SIGNAL(triggered()), puzzle_widget_, SLOT(useHint()));

	QToolBar *undo_bar = addToolBar(tr("Undo"));
	QAction *undo_action = hint_bar->addAction(tr("&Undo"));
	connect(undo_action, SIGNAL(triggered()), puzzle_widget_, SLOT(undo()));

	QToolBar *help_bar = addToolBar(tr("Help"));
	QAction *help_action = hint_bar->addAction(tr("&Help"));
	connect(help_action, SIGNAL(triggered()), this, SLOT(help()));

	QToolBar *about_bar = addToolBar(tr("About"));
	QAction *about_action = hint_bar->addAction(tr("&About"));
	connect(about_action, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::loadBackground()
{
	QImage *image = new QImage(tr(":/background.jpg"));
	puzzle_widget_->setImage(image);
}

void MainWindow::showGameSetupDialog()
{
	game_setup_dialog_->show();
}

void MainWindow::help()
{
    QMessageBox::about(this, tr("Help"),
            tr("Try to recover the order of all numbers(i.e. 1, 2, 3, 4, 5, 6, 7, 8, empty)?\n"
			"<b>Click</b> on a grid next to the empty grid to move it."
			"Have fun!"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Help"),
            tr("I wrote this sofware to learn Recursive Best First Search and Iterative Depth First Search algorithms.\n"
			"ustcliuc@mail.ustc.edu.cn"));
}

void MainWindow::startNewGame(int num, char level)
{
	int initial_distance = 0;
	if (level == 'E')
		initial_distance = 5*num;
	else if (level == 'M')
		initial_distance = 10*num;
	else
		initial_distance = 20*num;
	puzzle_widget_->startNewGame(num, initial_distance);
}
