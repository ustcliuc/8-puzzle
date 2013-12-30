#include "PuzzleWidget.h"

//#include <Qt\qwidget.h>
#include <Qt\qpainter.h>
#include <Qt\qmessagebox.h>
#include <Qt\qevent.h>
#include <Qt\qmouseeventtransition.h>
#include <Qt\qinputdialog.h>
#include <Qt\qfiledialog.h>
#include <iostream>

PuzzleWidget::PuzzleWidget()
{
	image_ = NULL;
	image_loaded_ = false;
	state_changed_ = true;
	ori_width_ = -1;
	zoom_scale_ =1.0;
	last_position_ = QPoint(-1, -1);

	left_up_corner_ = new QPoint(0, 0);

	setMouseTracking(true);
}


PuzzleWidget::~PuzzleWidget()
{
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
	if (image_loaded_ == true)
	{
		QPainter painter(this);
//		QMessageBox::about(this, tr("width: "), QString::number(this->width()));
//		model_image_.scaled(this->width(), this->height());
		painter.drawImage(QRect(0, 0, this->width(), this->height()), *image_);
		//for (set<CPoint2f>::iterator point_it = values_.begin(); point_it != values_.end(); point_it++)*/
		zoom_scale_ = (float)this->width()/(float)ori_width_;
	}
	if (started_ == true)
	{
		board_size_ = (int)(0.8*min(this->width(), this->height())+0.5)/num_rows_*num_rows_;
		grid_size_ = board_size_/num_rows_;
		left_up_corner_->setX(this->width()/2-board_size_/2);
		left_up_corner_->setY(this->height()/2-board_size_/2);
		QPainter painter(this);
		QLinearGradient gradient(0, 0, 0, 1);
		gradient.setColorAt(0.0, QColor(0, 0, 255));
		gradient.setColorAt(1.0, QColor(0, 255, 255));
		QBrush brush(gradient);
		for (int i = 0; i < num_rows_; i++)
		{
			for (int j = 0; j < num_rows_; j++)
			{
				if (current_state_[i*num_rows_+j] == 0)
					continue;
				painter.setBrush(brush);
				painter.drawRect(left_up_corner_->x()+j*grid_size_, left_up_corner_->y()+i*grid_size_, grid_size_, grid_size_);
//				painter.restore();
				painter.setPen(QPen(QColor(255, 0, 255)));
				painter.setFont(QFont("Times", grid_size_/2));
				painter.drawText(left_up_corner_->x()+j*grid_size_+grid_size_/4, left_up_corner_->y()+i*grid_size_+grid_size_/4*3, QString::number(current_state_[i*num_rows_+j]));
			}
		}
//		painter.restore();
		painter.setPen(QPen(QColor(255, 0, 0), 6));
		for (int i = 0; i <= num_rows_; i++)
		{
			painter.drawLine(QPoint(left_up_corner_->x(), left_up_corner_->y()+i*grid_size_), QPoint(left_up_corner_->x()+board_size_, left_up_corner_->y()+i*grid_size_));
			painter.drawLine(QPoint(left_up_corner_->x()+i*grid_size_, left_up_corner_->y()), QPoint(left_up_corner_->x()+i*grid_size_, left_up_corner_->y()+board_size_));
		}
		painter.setPen(QPen(QColor(0, 255, 0), 3));
		painter.setFont(QFont("Times", this->height()*0.03));
		painter.drawText(QPoint(this->width()/8, this->height()*0.06), tr("Shortest Path Length: %1").arg(shortest_path_length_));
		painter.drawText(QPoint(this->width()/2, this->height()*0.06), tr("You have moved: %1 times").arg(path_.size()));
		if (has_arrived_ == true)
		{
			int reply = QMessageBox::question(this, "Success!", "Congratulations! Do you want to start a new game?", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::Cancel);
			if (reply == QMessageBox::Yes)
			{
				has_arrived_ = false;
				emit startAnotherGame();
			}
		}
	}
	else
	{
		QPainter painter(this);
		painter.background();
	}
}

void PuzzleWidget::setImage(QImage *image)
{
	if (image_ != NULL)
	{
		delete image_;
		image_ = NULL;
	}
	if (image->width() > 0)
	{
		image_ = new QImage(*image);
		image_loaded_ = true;
		ori_width_ = image->width();
		update();
	}

//	QMessageBox::warning(this, tr("Check"), tr("Image width %1.").arg(QString::number(image_->width())));
}

void PuzzleWidget::saveImage()
{
	if (image_ == NULL)
		return;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), ".", tr("image files (*.jpg)"));

	if (fileName.isEmpty())
		return;

	image_->save(fileName);
}

void PuzzleWidget::mousePressEvent(QMouseEvent *event)
{
	last_position_ = event->pos();
	int index = getIndex(last_position_);
	if (index == -1)
		return;
	if (current_state_[index] == 0)
		return;
	move(index);
	if (hasArrived() == true)
		has_arrived_ = true;
	update();
}

void PuzzleWidget::startNewGame(const int num, const int initial_distance)
{
	num_ = num;
	num_rows_ = sqrt((float)num_);
	initial_distance_ = initial_distance;
	puzzle_ = new Puzzle(num_, initial_distance_);
	start_state_ = puzzle_->readState();
	current_state_ = cloneState(start_state_);
	started_ = true;
	has_arrived_ = false;
	state_changed_ = false;
	answer_path_ = puzzle_->readPath();
	shortest_path_length_ = answer_path_.size();
	path_.clear();
	update();
}

void PuzzleWidget::resetGame()
{
	puzzle_->setState(start_state_);
	current_state_ = cloneState(start_state_);
	started_ = true;
	has_arrived_ = false;
	state_changed_ = false;
	answer_path_ = puzzle_->readPath();
	path_.clear();
	update();
}

int PuzzleWidget::getIndex(const QPoint point)
{
	if (point.x() <= left_up_corner_->x() || point.x() >= left_up_corner_->x()+board_size_ 
		|| point.y() <= left_up_corner_->y() || point.y() >= left_up_corner_->y()+board_size_)
		return -1;
	int i = (point.y()-left_up_corner_->y())/grid_size_;
	int j = (point.x()-left_up_corner_->x())/grid_size_;
	return i*num_rows_+j;
}

int PuzzleWidget::getIndex0()
{
	for (int i = 0; i < num_; i++)
		if (current_state_[i] == 0)
			return i;
}

void PuzzleWidget::move(const int index)
{
	int index_0 = getIndex0();
	if (index+num_rows_ == index_0 || index-num_rows_ == index_0 || index+1 == index_0 || index-1 == index_0)
	{
		int temp = current_state_[index];
		current_state_[index] = current_state_[index_0];
		current_state_[index_0] = temp;
		state_changed_ = true;

		if (index+num_rows_ == index_0)
			path_.push_back('U');
		else if (index-num_rows_ == index_0)
			path_.push_back('D');
		else if (index+1 == index_0)
			path_.push_back('L');
		else if (index-1 == index_0)
			path_.push_back('R');
	}
}

void PuzzleWidget::move(const char movement)
{
	int index_0 = getIndex0();
	int index = index_0;
	switch (movement)
	{
		case 'U':
			index = index_0-num_rows_;
			break;
		case 'D':
			index = index_0+num_rows_;
			break;
		case 'L':
			index = index_0-1;
			break;
		case 'R':
			index = index_0+1;
	}
	int temp = current_state_[index];
	current_state_[index] = current_state_[index_0];
	current_state_[index_0] = temp;
	path_.push_back(movement);
}

bool PuzzleWidget::hasArrived()
{
	for (int i = 0; i < num_-1; i++)
		if (current_state_[i] != i+1)
			return false;
	return true;
}

int *PuzzleWidget::cloneState(const int *state)
{
	int *new_state = new int[num_];
	for (int i = 0; i < num_; i++)
		new_state[i] = state[i];
	return new_state;
}

void PuzzleWidget::useHint()
{
	if (state_changed_ == true)
	{
		puzzle_->setState(current_state_);
		answer_path_ = puzzle_->readPath();
	}
	if (answer_path_.size() == 0)
		return;
	char first_movement = answer_path_.front();
	answer_path_.pop();
	move(first_movement);
	state_changed_ = false;
	if (hasArrived() == true)
		has_arrived_ = true;
	update();
}

void PuzzleWidget::undo()
{
	if (path_.size() == 0)
		return;
	char last_movement = path_.back();
	char undo_movement = 'N';
	switch (last_movement)
	{
	case 'U':
		undo_movement = 'D';
		break;
	case 'D':
		undo_movement = 'U';
		break;
	case 'L':
		undo_movement = 'R';
		break;
	case 'R':
		undo_movement = 'L';
		break;
	}
	path_.pop_back();
	move(undo_movement);
	path_.pop_back();
	state_changed_ = true;
	update();
}
