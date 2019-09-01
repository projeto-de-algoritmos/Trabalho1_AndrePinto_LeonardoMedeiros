#include <SFML/Graphics.hpp>
#include <time.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

using namespace sf;

const int WIDTH=600;
const int HEIGHT=480;

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
		if(direction == UP){
			posY+=1;
		}
		if(direction == RIGHT){
			posX+=1;
		}
		if(direction == DOWN){
			posY-=1;
		}
		if(direction == LEFT){
			posX-=1;
		}
		
		if(posX > WIDTH){
			posX = 0;
		}
		if(posX < WIDTH){
			posX = WIDTH - 1;
		}
		if(posY > HEIGHT){
			posY = 0;
		}
		if(posY < HEIGHT){
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
		
		window.clear();
		window.draw(sprite);
		window.display();
	}

}
