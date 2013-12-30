#ifndef PuzzleWidget_H
#define PuzzleWidget_H

#include <QGLWidget>
#include <queue>
#include <vector>

#include "Puzzle.h"

using namespace std;

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:

	PuzzleWidget();
	~PuzzleWidget();

	void setImage(QImage *);
	void startNewGame(const int num, const int initial_distance);

private:
	QImage *image_;

	Puzzle *puzzle_;
	int num_;
	int num_rows_;
	int initial_distance_;
	int *current_state_;
	int *start_state_;
	vector<char> path_;
	queue<char> answer_path_;
	int shortest_path_length_;

	bool started_;
	bool has_arrived_;
	bool state_changed_;
	int board_size_;
	int grid_size_;
	QPoint *left_up_corner_;

	bool image_loaded_;
	int ori_width_;
	float zoom_scale_;
	QPoint last_position_;

	int getIndex(const QPoint point);
	int getIndex0();
	void move(const int index);
	void move(const char movement);
	bool hasArrived();
	int *cloneState(const int *state);

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);

signals:
	void startAnotherGame();

private slots:
	void resetGame();
	void useHint();
	void undo();

private slots:
//	void finishDesign(bool);
	void saveImage();
};

#endif //PuzzleWidget_H
