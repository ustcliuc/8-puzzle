#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Qt\qmainwindow.h>

#include "PuzzleWidget.h"
#include "GameSetupDialog.h"

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QSlider;
class QGroupBox;
QT_END_NAMESPACE

class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
	PuzzleWidget *puzzle_widget_;

	GameSetupDialog *game_setup_dialog_;

    QMenu *fileMenu;
    QMenu *helpMenu;
    //QAction *grabFrameBufferAct;
    //QAction *renderIntoPixmapAct;
    //QAction *loadMeshAct;
    //QAction *clearPixmapAct;
    QAction *exitAct;
    QAction *helpAct;
	QAction *aboutAct;

	void createActions();
	void createMenus();
	void createToolBars();
	void loadBackground();

public slots:
	void startNewGame(int num, char level);
	void showGameSetupDialog();

private slots:
	void grabFrameBuffer();
	void help();
	void about();

signals:
    void fileChoosed(bool, const char *);
//    void showMappedFaces(int);

    //void createActions();
    //void createMenus();
    //void setupToolBar();

    //QSlider *createSlider(const char *changedSignal, const char *setterSlot);
    //QSlider *createZoomSlider(const char *changedSignal, const char *setterSlot);
    //QSlider *createNumSlider(const char *changedSignal, const char *setterSlot);

    //QGroupBox *createNumGroupBox();

    //void setPixmap(const QPixmap &pixmap);
    //QSize getSize();
};

#endif // MAINWINDOW_H
