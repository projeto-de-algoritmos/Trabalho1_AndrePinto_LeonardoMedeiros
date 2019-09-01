#include <SFML/Graphics.hpp>
#include <time.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define USED 1
#define NOTUSED 0

using namespace sf;

const int WIDTH=600;
const int HEIGHT=480;
int speed = 4;
bool field[WIDTH][HEIGHT] = {0};

class Player{
public:
	int posX;
	int posY;
	int direction;
	Color color;
	
	Player(Color c){
		posX = rand() % WIDTH;
		posY = rand() % HEIGHT;
		direction=rand() % 4;
		
		color = c;
	}
	
	void tick(){
		if(direction == DOWN){
			posY+=1;
		}
		if(direction == RIGHT){
			posX+=1;
		}
		if(direction == UP){
			posY-=1;
		}
		if(direction == LEFT){
			posX-=1;
		}
		
		if(posX >= WIDTH){
			posX = 0;
		}
		if(posX < 0){
			posX = WIDTH - 1;
		}
		if(posY >= HEIGHT){
			posY = 0;
		}
		if(posY < 0){
			posY = HEIGHT - 1;
		}
	}
	
	Vector3f getColor(){
		return Vector3f(color.r,color.g,color.b);
	}
};

int main(int argc, char* argv[]){
	
	srand(time(NULL));
	
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Gira gira jequiti");
	window.setFramerateLimit(60);
	
	Texture texture;
	texture.loadFromFile("background.jpg");
	Sprite sBackground(texture);
	
	Player p1(Color::Red);
	
	Sprite sprite;
	RenderTexture t;
	t.create(WIDTH, HEIGHT);
	t.setSmooth(true);
	sprite.setTexture(t.getTexture());
	t.clear();
	t.draw(sBackground);
	
	while(window.isOpen()){
		Event e;
		while(window.pollEvent(e)){
			if(e.type == Event::Closed){
				window.close();
			}
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Up)){
			if(p1.posY != DOWN){
				p1.direction = UP;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Down)){
			if(p1.posY != UP){
				p1.direction = DOWN;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Left)){
			if(p1.posY != RIGHT){
				p1.direction = LEFT;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Right)){
			if(p1.posY != LEFT){
				p1.direction = RIGHT;
			}
		}
		
		for(int i=0; i<speed; i++){
			p1.tick();
			field[p1.posX][p1.posY] = USED; 

			CircleShape c(3);
			c.setPosition(p1.posX,p1.posY);
			c.setFillColor(p1.color);
			t.draw(c);
			t.display();    
		}
		
		window.clear();
		window.draw(sprite);
		window.display();
	}

}
