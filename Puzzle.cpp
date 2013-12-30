锘?include "Puzzle.h"

#include <time.h>
#include <math.h>

Puzzle::Puzzle()
{
}

Puzzle::Puzzle(const int num, const int initial_distance)
{
	num_ = num;
	num_rows_ = sqrt((float)num_);

	start_state_ = NULL;
	goal_state_ = new int[num_];
	for (int i = 0; i < num_-1; i++)
		goal_state_[i] = i+1;
	goal_state_[num_-1] = 0;
	path_ = NULL;

	initPuzzle(initial_distance);
	//rbfs();
}

Puzzle::~Puzzle()
{
}

void Puzzle::setState(const int *state)
{
	if (start_state_ != NULL)
		delete start_state_;
	start_state_ = new int[num_];
	for (int i = 0; i < num_; i++)
		start_state_[i] = state[i];
}

int *Puzzle::readState()
{
	if (start_state_ == NULL)
		return NULL;
	int *state = new int[num_];
	for (int i = 0; i < num_; i++)
		state[i] = start_state_[i];
	return state;
}

queue<char> Puzzle::readPath()
{
	idas();
	queue<char> path;
	if (path_ == NULL)
		return path;
	for (int i = 0; i < MAX_PATH_LENGTH; i++)
	{
		if (path_[i] == '\0')
			break;
		path.push(path_[i]);
	}
	return path;
}

void Puzzle::initPuzzle(const int d)
{
	srand(time(NULL));
	start_state_ = new int[num_];
	for (int i = 0; i < num_-1; i++)
		start_state_[i] = i+1;
	start_state_[num_-1] = 0;

	int move = 0;
	int index_0 = num_-1;
	for (int i = 0; i < d; )
	{
		index_0 = getIndex(start_state_, 0);
		move = rand()%4;		//浜ゆ崲鏂瑰紡
		switch(move){
		case 0:
			if (belongToBorder(index_0, 'U') == false)
			{	//鍒ゆ柇鏄惁鍙互浜ゆ崲
				swap(start_state_, index_0, index_0-num_rows_);
				i += 1;	//浜ゆ崲涓€姝ワ紝i鍊煎姞1
//					cout<<"u ";
			}
			break;
		case 1:
			if (belongToBorder(index_0, 'D') == false)
			{
				swap(start_state_, index_0, index_0+num_rows_);
				i += 1;
//					cout<<"d ";
			}
			break;
		case 2:
			if (belongToBorder(index_0, 'L') == false)
			{
				swap(start_state_, index_0, index_0-1);
				i += 1;
//					cout<<"l ";
			}
			break;
		case 3:
			if (belongToBorder(index_0, 'R') == false)
			{
				swap(start_state_, index_0, index_0+1);
				i += 1;
//					cout<<"r ";
			}
			break;
		default: break;
		}
	}
//	cout << "start:\n";
//	showState(start);
}

void Puzzle::idas()
{
	length_ = 1;
	start_node_ = new PuzzleNode;
	start_node_->h = distance(start_state_);
	start_node_->g = 0;
	start_node_->f = start_node_->h+start_node_->g;
	start_node_->path = new char[MAX_PATH_LENGTH];
	start_node_->path[0]='\0';
	start_node_->state = new int[num_];
	for (int i = 0; i < num_; i++)
		start_node_->state[i] = start_state_[i];
	start_node_->next = NULL;
	int d_limit = start_node_->f;
	int next_d_limit = d_limit;
	while (next_d_limit < MY_INFINITE)
	{
		next_d_limit = MY_INFINITE;
		current_node_ = start_node_;
		PuzzleNode *pre_node = start_node_;
		while (current_node_ != NULL)
		{
			int ds = current_node_->g+distance(current_node_->state);
			if (ds > d_limit)
			{		//鍒ゆ柇鏄惁鍙互鎵╁睍
				if (ds < next_d_limit)
					next_d_limit = ds;	//鏇存柊涓嬩竴涓埅鏂€?				pre_node = current_node_;
				current_node_ = current_node_->next;
			}
			else
			{
				if (hasArrived(current_node_->state))
				{
					if (path_ != NULL)
						delete path_;
					path_ = new char[MAX_PATH_LENGTH];
					strcpy(path_, current_node_->path);
					return;
				}
				PuzzleNode *tail_node = findTailNode();
				if (tail_node == NULL)
					path_ = NULL;
				addChildren(tail_node);
				deleteNode(current_node_, pre_node);
			}
		}
		d_limit = next_d_limit;	//鏇存柊鎴柇鍊?	}
}

void Puzzle::rbfs()
{
	length_ = 1;
	start_node_ = new PuzzleNode;
	start_node_->h = distance(start_state_);
	start_node_->g = 0;
	start_node_->f = start_node_->h+start_node_->g;
	start_node_->path = new char[MAX_PATH_LENGTH];
	start_node_->path[0] = '\0';
	start_node_->state = new int[num_];
	for (int i = 0; i < num_; i++)
		start_node_->state[i] = start_state_[i];
	start_node_->next = NULL;
	current_node_ = start_node_;
	int new_f = MY_INFINITE;
	path_ = loopRBFS(start_node_, new_f, new_f);		//寮€濮嬮€掑綊
}

char *Puzzle::loopRBFS(PuzzleNode *current, const int limit, int &new_f)
{		//RBFS閫掑綊鍑芥暟
	if (hasArrived(current->state) == true)
		return current->path;

	current_node_ = current;
//		cout<<length<<"\n";

	int previous_length = length_;
	addChildren(current);
	int length_RBFS = length_-previous_length;
	if (length_RBFS == 0)
	{
		new_f = MY_INFINITE;
		return NULL;
	}
	PuzzleNode *successor = current->next;
	while (successor != NULL)
	{
		successor->f = max(successor->f, current->f);
		successor = successor->next;
	}
	while (true)
	{
		PuzzleNode *min_node = new PuzzleNode;
		int min_f = MY_INFINITE;
		int alternative = MY_INFINITE;
		successor = current->next;
		int i = 0;
		while (successor != NULL)
		{
			i++;
			if (i > length_RBFS)
				break;
			if (successor->f < min_f)
			{		//瀵绘壘鏈€灏廸鍊?				alternative = min_f;
				min_f = successor->f;
				min_node = successor;
			}
			else if (successor->f < alternative)		//鏇存柊alternative鍊?				alternative = successor->f;
			successor = successor->next;
		}
		if (min_f > limit)
		{
			new_f = min_f;
			return NULL;
		}
		int new_f = MY_INFINITE;
		char *result = loopRBFS(min_node, min(limit, alternative), new_f);
		min_node->f = new_f;
		if (result != NULL)
			return result;
	}
}

int Puzzle::getIndex(const int *a, const int value)
{
	//寰楀埌鍏冪礌鍦ㄦ暟缁勪腑鐨勭储寮?	for (int i = 0; i < num_; i++)
		if (a[i] == value)
			return i;
	return -1;
}

void Puzzle::swap(int *a, const int index_1, const int index_2)
{		//浜ゆ崲鍏冪礌

	int temp = a[index_1];
	a[index_1] = a[index_2];
	a[index_2] = temp;
}

void Puzzle::swap(const int *src, int *dst, const int index_1, const int index_2)
{		//浜ゆ崲鍏冪礌

	for (int i = 0; i < num_; i++)
		dst[i] = src[i];
	int temp = dst[index_1];
	dst[index_1] = dst[index_2];
	dst[index_2] = temp;
}

bool Puzzle::belongToBorder(const int index, const char border)
{
	if ((index < num_rows_ && border == 'U') 
		|| (index >= num_rows_*(num_rows_-1) && border == 'D') 
		|| (index%num_rows_ == 0 && border == 'L') 
		|| (index%num_rows_ == num_rows_-1 && border == 'R'))
		return true;
	else
		return false;
}

char Puzzle::lastAction(const char *path)
{	//寰楀埌鐖惰妭鐐圭殑鏈€鍚庝竴姝?	if (path == NULL || path[0] == '\0')
		return 'O';

	int i = 0;
	for (i = 0; i < MAX_PATH_LENGTH; i++)
		if (path[i] == '\0')
			break;
	return path[i-1];
}

void Puzzle::addChildren(PuzzleNode *tail_node)
{
	if (current_node_ == NULL)
		return;

	//鎵╁睍鑺傜偣
	int index_0 = getIndex(current_node_->state, 0);
	char last_action = lastAction(current_node_->path);

	if (tail_node == NULL)
		return;

	PuzzleNode *tail = tail_node;
	if (belongToBorder(index_0, 'U') == false)
	{
		int *temp = new int[num_];
		swap(current_node_->state, temp, index_0, index_0-num_rows_);
		if(last_action != 'D')
		{
			PuzzleNode *new_node = new PuzzleNode;
			int ng = current_node_->g+1;
			new_node->g = ng;
			int h = distance(temp);
			new_node->h = h;
			new_node->f = h+ng;
			new_node->path = new char[MAX_PATH_LENGTH];
			strcpy(new_node->path, current_node_->path);
			strcat(new_node->path, "U");
			new_node->state = cloneState(temp);
			new_node->next = tail->next;
			tail->next = new_node;
			tail = new_node;
			length_++;
		}
		else
			delete temp;
	}
	if (belongToBorder(index_0, 'D') == false)
	{
		int *temp = new int[num_];
		swap(current_node_->state, temp, index_0, index_0+num_rows_);
		if(last_action != 'U')
		{
			PuzzleNode *new_node = new PuzzleNode;
			int ng = current_node_->g+1;
			new_node->g = ng;
			int h = distance(temp);
			new_node->h = h;
			new_node->f = h+ng;
			new_node->path = new char[MAX_PATH_LENGTH];
			strcpy(new_node->path, current_node_->path);
			strcat(new_node->path, "D");
			new_node->state = cloneState(temp);
			new_node->next = tail->next;
			tail->next = new_node;
			tail = new_node;
			length_++;
		}
		else
			delete temp;
	}
	if (belongToBorder(index_0, 'L') == false)
	{
		int *temp = new int[num_];
		swap(current_node_->state, temp, index_0, index_0-1);
		if(last_action != 'R')
		{
			PuzzleNode *new_node = new PuzzleNode;
			int ng = current_node_->g+1;
			new_node->g = ng;
			int h = distance(temp);
			new_node->h = h;
			new_node->f = h+ng;
			new_node->path = new char[MAX_PATH_LENGTH];
			strcpy(new_node->path, current_node_->path);
			strcat(new_node->path, "L");
			new_node->state = cloneState(temp);
			new_node->next = tail->next;
			tail->next = new_node;
			tail = new_node;
			length_++;
		}
		else
			delete temp;
	}
	if (belongToBorder(index_0, 'R') == false)
	{
		int *temp = new int[num_];
		swap(current_node_->state, temp, index_0, index_0+1);
		if(last_action != 'L')
		{
			PuzzleNode *new_node = new PuzzleNode;
			int ng = current_node_->g+1;
			new_node->g = ng;
			int h = distance(temp);
			new_node->h = h;
			new_node->f = h+ng;
			new_node->path = new char[MAX_PATH_LENGTH];
			strcpy(new_node->path, current_node_->path);
			strcat(new_node->path, "R");
			new_node->state = cloneState(temp);
			new_node->next = tail->next;
			tail->next = new_node;
			tail = new_node;
			length_++;
		}
		else
			delete temp;
	}
}

void Puzzle::deleteNode(PuzzleNode *&node, PuzzleNode *pre_node)
{	//鍒犻櫎宸叉墿灞曡妭鐐?	if (node == NULL)
		return;
	if (start_node_ == node)
	{
		start_node_ = node->next;
		delete node;
		node = start_node_;
	}
	else
	{
		pre_node->next = node->next;
		delete node;
		node = pre_node->next;
	}
	length_--;
}

bool Puzzle::hasArrived(const int *a)
{
	//鍒ゆ柇鏄惁鍒拌揪
	for (int i = 0; i < num_; i++)
		if (a[i] != goal_state_[i])
			return false;
	return true;
}

int Puzzle::distance(const int *a)
{
	//寰楀埌鍚彂鍑芥暟鍊?	int h = 0;
	for (int i = 0; i < num_; i++)
	{
		if (a[i] == 0)
			continue;
		int index = getIndex(goal_state_, a[i]);
		h += abs(index/num_rows_-i/num_rows_)+abs(index%num_rows_-i%num_rows_);
	}
	return h;
}

PuzzleNode *Puzzle::cloneNode(const PuzzleNode *node)
{
	if (node == NULL)
		return NULL;
	PuzzleNode *new_node = new PuzzleNode;
	new_node->f = node->f;
	new_node->g = node->g;
	new_node->h = node->h;
	new_node->next = NULL;
	new_node->path = new char[MAX_PATH_LENGTH];
	strcpy(new_node->path, node->path);
	new_node->state = new int[num_];
	for (int i = 0; i < num_; i++)
		new_node->state[i] = node->state[i];
}

PuzzleNode *Puzzle::findTailNode()
{
	if (start_node_ == NULL)
		return NULL;
	PuzzleNode *tail_node = start_node_;
	while (tail_node->next != NULL)
		tail_node = tail_node->next;
	return tail_node;
}

int *Puzzle::cloneState(const int *state)
{
	int *new_state = new int[num_];
	for (int i = 0; i < num_; i++)
		new_state[i] = state[i];
	return new_state;
}

//#include <math.h>
//#include <stdio.h>
//#include <iostream>
//#include <string.h>
//
//using namespace std;
//
//int *mySwap(int i1,int i2,int *old);
//int myIndex(int *a,int value);
//
//typedef struct myNode{
//	int f;
//	int h;
//	int g;
//	int* state;		//褰撳墠鐘舵€?//	char *path;		//褰撳墠宸茶蛋杩囩殑璺緞
//	myNode* next;
//}myNode;
//
//myNode* node;	//褰撳墠鑺傜偣
//myNode* pre;	//褰撳墠鑺傜偣鐨勫墠涓€涓妭鐐?//myNode* add;	//鐢ㄤ簬娣诲姞鑺傜偣鐨勬寚閽?//myNode* start;	//绗竴涓妭鐐?////	int* state;
//char *path;
//int length;
//int infinite;	//鏃犻檺澶э紝鏈緥涓敤100鍗冲彲
//	
//char lastAction(char *path){		//寰楀埌鐖惰妭鐐圭殑鏈€鍚庝竴姝?//	if(path==NULL||path[0]=='\0'){
//		return 'o';
//	}
//	char* last=path;
//	while(*(last+1)!='\0'){
//		last++;
//	}
//	return *last;
//}
//bool arrive(int *a,int *b){		//鍒ゆ柇鏄惁鍒拌揪
//	for(int i=0;i<9;i++){
//		if(a[i]!=b[i]){
//			return false;
//		}
//	}
//	return true;
//}
//int distance(int *a,int *b){		//寰楀埌鍚彂鍑芥暟鍊?//	int h=0;
//	int ib;
//	for(int i=0;i<9;i++){
//		if(a[i]==0){
//			continue;
//		}
//		ib=myIndex(b,a[i]);
//		h+=abs(ib/3-i/3)+abs(ib%3-i%3);
//	}
//	return h;
//}
//void addChildren(int *s,int *g){	//鎵╁睍鑺傜偣
//	int myindex_0=myIndex(s,0);
//	int *temp;
//	int h;
//	int ng=node->g+1;
//	myNode *newnode;
//	char lastaction=lastAction(node->path);
//	add=node;
//
//	if(add==NULL)
//		return;
//	if(myindex_0!=0&&myindex_0!=3&&myindex_0!=6){
//		temp=mySwap(myindex_0,myindex_0-1,s);
//		if(lastaction!='d'){
//			h=distance(temp,g);
//			newnode=new myNode;
//			newnode->g=ng;
//			newnode->h=h;
//			newnode->f=h+ng;
//			newnode->path=new char[30];
//			strcpy(newnode->path,node->path);
//			strcat(newnode->path,"u");
//			newnode->state=temp;
//			newnode->next=add->next;
//			add->next=newnode;
//			add=newnode;
//			length++;
//		}
//	}
//	if(myindex_0!=2&&myindex_0!=5&&myindex_0!=8){
//		temp=mySwap(myindex_0,myindex_0+1,s);
//		if(lastaction!='u'){
//			h=distance(temp,g);
//			newnode=new myNode;
//			newnode->g=ng;
//			newnode->h=h;
//			newnode->f=h+ng;
//			newnode->path=new char[30];
//			strcpy(newnode->path,node->path);
//			strcat(newnode->path,"d");
//			newnode->state=temp;
//			newnode->next=add->next;
//			add->next=newnode;
//			add=newnode;
//			length++;
//		}
//	}
//	if(myindex_0!=0&&myindex_0!=1&&myindex_0!=2){
//		temp=mySwap(myindex_0,myindex_0-3,s);
//		if(lastaction!='r'){
//			h=distance(temp,g);
//			newnode=new myNode;
//			newnode->g=ng;
//			newnode->h=h;
//			newnode->f=h+ng;
//			newnode->path=new char[30];
//			strcpy(newnode->path,node->path);
//			strcat(newnode->path,"l");
//			newnode->state=temp;
//			newnode->next=add->next;
//			add->next=newnode;
//			add=newnode;
//			length++;
//		}
//	}
//	if(myindex_0!=6&&myindex_0!=7&&myindex_0!=8){
//		temp=mySwap(myindex_0,(myindex_0+3),s);
//		if(lastaction!='l'){
//			h=distance(temp,g);
//			newnode=new myNode;
//			newnode->g=ng;
//			newnode->h=h;
//			newnode->f=h+ng;
//			newnode->path=new char[30];
//			strcpy(newnode->path,node->path);
//			strcat(newnode->path,"r");
//			newnode->state=temp;
//			newnode->next=add->next;
//			add->next=newnode;
//			add=newnode;
//			length++;
//		}
//	}
//}
//void deleteNode(){		//鍒犻櫎宸叉墿灞曡妭鐐?//	myNode* temp=node;
//	if(start==node&&node!=NULL){
//		start=node->next;
//		node=start;
//	}
//	else{
//		pre->next=node->next;
//		node=pre->next;
//	}
//	delete(temp);
//	length--;
//}
//int myIndex(int* a,int value){	//寰楀埌鍏冪礌鍦ㄦ暟缁勪腑鐨勭储寮?//	for(int i=0;i<9;i++){
//		if(a[i]==value){
//			return i;
//		}
//	}
//	return -1;
//}
//int* mySwap(int i1,int i2,int *old){		//浜ゆ崲鍏冪礌
//	int *a;
//	a=new int[9];
//	for(int i=0;i<9;i++){
//		a[i]=old[i];
//	}
//	int temp=a[i1];
//	a[i1]=a[i2];
//	a[i2]=temp;
//	return a;
//}
//int *randep(int d){
//	int* start;
//	start=new int[9];
//	for(int j=0;j<3;j++){
//		for(int k=0;k<3;k++){
//			start[k*3+j]=k+j*3+1;
//		}
//	}
//	start[8]=0;
//	int move=0;
//	int myindex_0=8;
//	for(int i=0;i<d;){
//		myindex_0=myIndex(start,0);
//		move=rand()%4;		//浜ゆ崲鏂瑰紡
//		switch(move){
//		case 0:
//			if(myindex_0!=0&&myindex_0!=3&&myindex_0!=6){	//鍒ゆ柇鏄惁鍙互浜ゆ崲
//				start=mySwap(myindex_0,myindex_0-1,start);
//				i+=1;	//浜ゆ崲涓€姝ワ紝i鍊煎姞1
////					cout<<"u ";
//			}
//			break;
//		case 1:
//			if(myindex_0!=2&&myindex_0!=5&&myindex_0!=8){
//				start=mySwap(myindex_0,myindex_0+1,start);
//				i+=1;
////					cout<<"d ";
//			}break;
//		case 2:
//			if(myindex_0!=0&&myindex_0!=1&&myindex_0!=2){
//				start=mySwap(myindex_0,myindex_0-3,start);
//				i+=1;
////					cout<<"l ";
//			}break;
//		case 3:
//			if(myindex_0!=6&&myindex_0!=7&&myindex_0!=8){
//				start=mySwap(myindex_0,myindex_0+3,start);
//				i+=1;
////					cout<<"r ";
//			}break;
//		default: break;
//		}
//	}
//	//cout<<"start:\n";
//	//showState(start);
//	return start;
//}
//
//char* idas(int* s,int* g){
//	length=1;
//	pre=new myNode;
//	pre->h=distance(s,g);
//	pre->g=0;
//	pre->f=pre->h+pre->g;
//	pre->path=new char[30];
//	pre->path[0]='\0';
//	pre->state=s;
//	pre->next=NULL;
//	start=pre;
//	add=pre;
//	path=new char[30];
//	int d_limit=pre->f;
//	int next_d_limit=0;
//	int ds;
//	int* state;
//	while(next_d_limit<100){
//		next_d_limit=100;
//		node=start;
//		pre=start;
//		while(node!=NULL){
//			state=node->state;
//			ds=node->g+distance(state,g);
//			if(ds>d_limit){		//鍒ゆ柇鏄惁鍙互鎵╁睍
//				if(ds<next_d_limit){
//					next_d_limit=ds;	//鏇存柊涓嬩竴涓埅鏂€?//				}
//				pre=node;
//				node=node->next;
//			}
//			else{
//				if(arrive(state,g)){
//					strcpy(path,node->path);
//					return path;
//				}
//				addChildren(state,g);
//				deleteNode();
//			}
//		}
//		d_limit=next_d_limit;	//鏇存柊鎴柇鍊?//	}
//	return NULL;
//}
//char* loopRBFS(myNode* current,int* g,int limit,int &newf){		//RBFS閫掑綊鍑芥暟
//	if(arrive(current->state,g)){
//		return current->path;
//	}
////		cout<<length<<"\n";
//	start=current;
//	myNode* startRBFS=new myNode;
//	int lengthRBFS=length;
//	int i=0;
//	int alternative;
//	int minmax;
//	startRBFS->f=current->f;
//	startRBFS->g=current->g;
//	startRBFS->h=current->h;
//	startRBFS->next=NULL;
//	startRBFS->path=current->path;
//	startRBFS->state=current->state;
//	node=startRBFS;
//	addChildren(startRBFS->state,g);
//	lengthRBFS=length-lengthRBFS;
//	if(add==startRBFS){
//		newf=infinite;
//		return NULL;
//	}
//	myNode* minNode=new myNode;
//	myNode* successor=startRBFS->next;
//	while(successor!=NULL){
//		successor->f=max(successor->f,current->f);
//		successor=successor->next;
//	}
//	while(1){
//		minmax=infinite;
//		successor=startRBFS->next;
//		i=0;
//		while(successor!=NULL){
//			i++;
//			if(i>lengthRBFS){
//				break;
//			}
//			if(successor->f<minmax){		//瀵绘壘鏈€灏廸鍊?//				alternative=minmax;
//				minmax=successor->f;
//				minNode=successor;
//			}
//			else if(successor->f<alternative){		//鏇存柊alternative鍊?//				alternative=successor->f;
//			}
//			successor=successor->next;
//		}
//		if(minmax>limit){
//			newf=minmax;
//			return NULL;
//		}
//		int newf=infinite;
//		char* result=loopRBFS(minNode,g,min(limit,alternative),newf);
//		minNode->f=newf;
//		if(result!=NULL){
//			return result;
//		}
//	}
//}
//char* rbfs(int* s,int* g){
//	length=1;
//	pre=new myNode;
//	pre->h=distance(s,g);
//	pre->g=0;
//	pre->f=pre->h+pre->g;
//	pre->path=new char[30];
//	pre->path[0]='\0';
//	pre->state=s;
//	pre->next=NULL;
//	start=pre;
//	path=new char[30];
//	infinite=100;
//	int newf=infinite;
//	char* pathRBFS=loopRBFS(start,g,newf,newf);		//寮€濮嬮€掑綊
//	path=pathRBFS;
//	return pathRBFS;
//}
