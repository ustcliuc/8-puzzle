#include "GameSetupDialog.h"

GameSetupDialog::GameSetupDialog(QWidget *parent)
	: QDialog(parent)
{
	setupWidgets();

	QGridLayout *central_layout = new QGridLayout();
	central_layout->addWidget(introduction_label_, 0, 0, 1, 4);
	central_layout->addWidget(num_grids_label_, 1, 0);
	central_layout->addWidget(num_grids_combobox_, 1, 1);
	central_layout->addWidget(level_label_, 1, 2);
	central_layout->addWidget(level_groupbox_, 1, 3);
	central_layout->addWidget(confirm_button_, 2, 1);
	central_layout->addWidget(cancel_button_, 2, 3);
	
	setLayout(central_layout);
	num_grids_ = 9;
	level_ = 'E';
}

GameSetupDialog::~GameSetupDialog()
{

}

void GameSetupDialog::setupWidgets()
{
	introduction_label_ = new QLabel(tr("Please choose the number of grids and game level"));
	num_grids_label_ = new QLabel(tr("Grid Number"));
	num_grids_combobox_ = new QComboBox();

	num_grids_combobox_->addItem(tr("3X3"));
	num_grids_combobox_->addItem(tr("4X4"));
	//num_grids_combobox_->addItem(tr("5X5"));
	//num_grids_combobox_->addItem(tr("6X6"));
	//num_grids_combobox_->addItem(tr("7X7"));
	//num_grids_combobox_->addItem(tr("8X8"));
	//num_grids_combobox_->addItem(tr("9X9"));

	num_grids_label_->setBuddy(num_grids_combobox_);
	connect(num_grids_combobox_, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseNumGrids(int)));

	level_label_ = new QLabel(tr("Level"));
	QRadioButton *easy_level_radio = new QRadioButton(tr("Easy"));
	QRadioButton *middle_level_radio = new QRadioButton(tr("Middle"));
	QRadioButton *hard_level_radio = new QRadioButton(tr("Hard"));
	connect(easy_level_radio, SIGNAL(clicked()), this, SLOT(chooseEasyLevel()));
	connect(middle_level_radio, SIGNAL(clicked()), this, SLOT(chooseMiddleLevel()));
	connect(hard_level_radio, SIGNAL(clicked()), this, SLOT(chooseHardLevel()));
	easy_level_radio->setChecked(true);

	QVBoxLayout *box_layout = new QVBoxLayout();
	box_layout->addWidget(easy_level_radio);
	box_layout->addWidget(middle_level_radio);
	box_layout->addWidget(hard_level_radio);
	level_groupbox_ = new QGroupBox("Level");
	level_groupbox_->setLayout(box_layout);

	confirm_button_ = new QPushButton(tr("&Confirm"));
	connect(confirm_button_, SIGNAL(clicked()), SLOT(confirm()));
	cancel_button_ = new QPushButton(tr("&Cancel"));
	connect(cancel_button_, SIGNAL(clicked()), this, SLOT(reject()));

	setWindowTitle(tr("New Game"));
}

void GameSetupDialog::chooseNumGrids(int index)
{
	num_grids_ = pow((float)3+index, 2);
}

void GameSetupDialog::chooseEasyLevel()
{
	level_ = 'E';
}

void GameSetupDialog::chooseMiddleLevel()
{
	level_ = 'M';
}

void GameSetupDialog::chooseHardLevel()
{
	level_ = 'H';
}

void GameSetupDialog::confirm()
{
	emit gameSetup(num_grids_, level_);
	accept();
}
