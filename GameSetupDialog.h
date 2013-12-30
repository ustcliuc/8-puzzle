#ifndef GAMESETUPDIALOG_H
#define GAMESETUPDIALOG_H

#include <QDialog>
#include <Qt\qlabel.h>
#include <Qt\qcombobox.h>
#include <Qt\qradiobutton.h>
#include <Qt\qpushbutton.h>
#include <Qt\qgroupbox.h>
#include <Qt\qboxlayout.h>

class GameSetupDialog : public QDialog
{
	Q_OBJECT

public:
	GameSetupDialog(QWidget *parent);
	~GameSetupDialog();

private:
	int num_grids_;
	char level_;

	QLabel *introduction_label_;
	QLabel *num_grids_label_;
	QComboBox *num_grids_combobox_;
	QLabel *level_label_;
	QGroupBox *level_groupbox_;
	QPushButton *confirm_button_;
	QPushButton *cancel_button_;

	void setupWidgets();

private slots:
	void chooseNumGrids(int index);
	void chooseEasyLevel();
	void chooseMiddleLevel();
	void chooseHardLevel();
	void confirm();

signals:
	void gameSetup(int num, char level);
};

#endif // GAMESETUPDIALOG_H
