#include <QApplication>
#include <QWidget>
#include <QKeyEvent>
#include <QRect>
#include <QPainter>
#include <iostream>
#include <vector>

using namespace std;
using std::vector;
#define W   (10)
#define H   (20)
enum class Direction { kLeft, kRight, kDown, kUp };
typedef struct 
{
        int x;
        int y;
} Pos;


class TetrisModel {
private:
    int shapes[7][4] = 
    {
        {1,3,5,7}, // I
        {2,4,5,7}, // Z
        {3,5,4,6}, // S
        {3,5,4,7}, // T
        {2,3,5,7}, // L
        {3,5,7,6}, // J
        {2,3,4,5}, // O
    };
        void moveBack() 
    {
        for (int i = 0; i < 4; i++) 
        {
            rep[i] = bak[i];
        }
    }

    void moveLeft() 
    {
        for (int i = 0; i < 4; i++) 
                {
                    bak[i] = rep[i];
                    rep[i].x -= 1;
                }
        if (!isValidMove())
        {
            moveBack();
        }
    }

    void moveRight() 
    {
        for (int i = 0; i < 4; i++) 
                {
                    bak[i] = rep[i];
                    rep[i].x += 1;
                }
        if (!isValidMove())
        {
            moveBack();
        }
    }

    void moveDown()
    {
        for (int i = 0; i < 4; i++) 
                {
                    bak[i] = rep[i];
                    rep[i].y += 1;
                }
        if (!isValidMove())
        {
            for (int i = 0; i < 4; i++) 
            {
                field[bak[i].y][bak[i].x] = true;
            }

            for (int i = H - 1; i > 0; i--)
            {
                int total = 0;
                for (int j = 0;	j < W; j++) 
                {
                    if (field[i][j])
                    {
                        total++;
                    }
                }
                if (total == W) 
                {

			field.erase(field.begin() + i, field.begin() + i + 1);
			field.insert(field.begin(), {false, false, false, false, false, false, false, false, false, false});
			i++;
                }
            }

            int n = rand() % 7;
            for (int i = 0; i < 4; i++)
            {
                rep[i].x = shapes[n][i] % 2;
                rep[i].y = shapes[n][i] / 2;
            }
            if (!isValidMove()) 
            {
                this->gameOver = true;
            }
        }
    }

    void moveUpRotate()
    {
        Pos p = rep[1];
        for (int i = 0; i < 4; i++)
                {
                    bak[i] = rep[i];
                    int x = rep[i].y - p.y;
                    int y = rep[i].x - p.x;
                    rep[i].x = p.x - x;
                    rep[i].y = p.y + y;
                }
        if (!isValidMove())
        {
            moveBack();
        }
    }

    bool isValidMove() 
    {
        for (int i = 0; i < 4; i++) 
        {
            if (   rep[i].x < 0 
                || rep[i].x >= W 
                || rep[i].y < 0
                || rep[i].y >= H) 
            {
                return false;
            }
            if (field[rep[i].y][rep[i].x])
            {
                return false;
            }
        }
        return true;
    }

public:
    Pos rep[4];
    Pos bak[4];
    vector<vector<bool>> field = 
    {
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false},
    	{false, false, false, false, false, false, false, false, false, false}
    };
    TetrisModel()
    {
	    cout << "Model Ctor" << endl;
            for (int i = 0; i < 4; i++)
            {
                rep[i].x = shapes[0][i] % 2;
                rep[i].y = shapes[0][i] / 2;
            }
    }
    void move()
    {
        switch(dir) {
            case Direction::kUp:
                moveUpRotate();
		moveDown();
                break;
	    case Direction::kDown:
                moveDown();
                break;
	    case Direction::kLeft:
                moveLeft();
		moveDown();
                break;
	    case Direction::kRight:
                moveRight();
		moveDown();
                break;
            default:
                break;

        }
    }
    Direction dir;
    bool gameOver = false;
};

class TetrisViewController : public QWidget {

  public:
    TetrisViewController(QWidget *parent = 0);

  protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);
    void drawObjects(QPainter *);

  private: 
    int timerId;
    static const int DELAY = 300;
    static const int BOTTOM_EDGE = 800;
    TetrisModel model;
};

TetrisViewController::TetrisViewController(QWidget *parent)
    : QWidget(parent) {
	timerId = startTimer(DELAY);
	cout << "ViewCntl Ctor" << endl;
}
 
void TetrisViewController::paintEvent(QPaintEvent *e) {
  
  Q_UNUSED(e);  
    
  QPainter painter(this);

  if (model.gameOver) {
 
  QFont font("Courier", 15, QFont::DemiBold);
  QFontMetrics fm(font);
  int textWidth = fm.width("Game Over");

  painter.setFont(font);
  int h = height();
  int w = width();

  painter.translate(QPoint(w/2, h/2));
  painter.drawText(-textWidth/2, 0, "Game Over");       
}
  else {
    drawObjects(&painter);
  }
}

void TetrisViewController::drawObjects(QPainter *painter) {
    
     for (int i = 0; i < H; i++)
     {
         for (int j = 0; j < W; j++)
         {
             if (model.field[i][j])
             {
                painter->fillRect(QRect(j * 30, i * 30, 30, 30), QColor(255, 10, 10));
             }
         }
     }

     for (int i = 0; i < 4; i++)
     {
         Pos p = model.rep[i];
         painter->fillRect(QRect(p.x * 30, p.y * 30, 30, 30), QColor(10, 10, 255));
     }
}

void TetrisViewController::timerEvent(QTimerEvent *e) {
    
  Q_UNUSED(e);  
    
  model.move();
  model.dir = Direction::kDown;
  repaint();
}

void TetrisViewController::keyPressEvent(QKeyEvent *e) {
    
    switch (e->key()) {
    case Qt::Key_Left:
        model.dir = Direction::kLeft;        
        break;
       
    case Qt::Key_Right:
       	model.dir = Direction::kRight;        
        break;

    case Qt::Key_Up:
        model.dir = Direction::kUp;
        break;        
                
    case Qt::Key_Down:
        model.dir = Direction::kDown;
        break;
        
    default:
        QWidget::keyPressEvent(e);
    }
}
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TetrisViewController TetrisViewController;
    TetrisViewController.resize(300, 600);
    TetrisViewController.setWindowTitle("Tetris Game");
    TetrisViewController.show();
    return app.exec();
}
