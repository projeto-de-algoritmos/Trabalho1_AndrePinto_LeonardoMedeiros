#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <queue>

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

class Bot : public Player{
public:
	int line;
	Bot(Color c) : Player(c){
		color = c;
		line = 0;
	}

	void tick(){
		if(posX>3 and posX<WIDTH-3 and posY>3 and posY<HEIGHT-3 and line >= 30){
			direction=rand()%4;
			line = 0;
		}

		int verif;
		if(direction == DOWN){
			verif = posY+1;
			if(posY+1 >= HEIGHT){
				verif = 0;
			}
			if(field[posX][verif] == 0){
				posY+=1;
			}
			else{
				direction = RIGHT;
				tick();
				return;
			}
		}
		if(direction == RIGHT){
			verif = posX+1;
			if(posX+1 >= WIDTH){
				verif = 0;
			}
			if(field[verif][posY] == 0){
				posX+=1;
			}
			else{
				direction = UP;
				tick();
				return;
			}
		}
		if(direction == UP){
			verif = posY-1;
			if(posY-1 < 0){
				verif = HEIGHT-1;
			}
			if(field[posX][verif] == 0){
				posY-=1;
			}
			else{
				direction = LEFT;
				tick();
				return;
			}
		}
		if(direction == LEFT){
			if(field[posX-1 < 0 ? WIDTH-1 : posX-1][posY] == 0){
				posX-=1;
			}
			else if(field[posX][posY+1 >= HEIGHT ? 0 : posY+1] == 0){
				direction = DOWN;
				tick();
				return;
			}
			else if(field[posX+1 >= WIDTH ? 0 : posX+1][posY] == 0){
				direction = RIGHT;
				tick();
				return;
			}
			else if(field[posX][posY-1 < 0 ? HEIGHT-1 : posY-1] == 0){
				direction = UP;
				tick();
				return;
			}
			else{
				// Bot is dead here
				// nothing to do
			}
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

		line++;
	}
};

class Obstacle{
public:
	std::queue <std::pair <int, int>> obstacleQueue;

	Obstacle(std::pair <int, int> p){
		obstacleQueue.push(p);
	}

	std::pair <int, int> nextObs(){
		std::pair <int, int> p;
		p.first = -1;
		p.second = -1;
		try{
			std::pair <int, int> obs = obstacleQueue.front();

			while(!obstacleQueue.empty()){
				if (field[obs.first][obs.second] == USED){
					obstacleQueue.pop();
					obs = obstacleQueue.front();
				}
				else{
					obstacleQueue.pop();
					break;
				}
			}

			if (field[obs.first][obs.second] == NOTUSED){
				field[obs.first][obs.second] = USED;

				p.first = obs.first+1 >= WIDTH ? 0 : obs.first+1;
				p.second = obs.second;
				obstacleQueue.push(p); // x+1, y
				p.first = obs.first-1 < 0 ? WIDTH-1 : obs.first-1;
				p.second = obs.second;
				obstacleQueue.push(p); // x-1, y
				p.first = obs.first;
				p.second = obs.second+1 >= HEIGHT ? 0 : obs.second+1;
				obstacleQueue.push(p); // x, y+1
				p.first = obs.first;
				p.second = obs.second-1 < 0 ? HEIGHT-1 : obs.second-1;
				obstacleQueue.push(p); // x, y-1

				return obs;
			}

			return p;
		}
		catch(...){
			return p;
		}

	}
};

int main(int argc, char* argv[]){

	if(argc >= 2){
		try{
			int s = atoi(argv[1]);
			if(s>=1 && s<=20){
				speed = s;
			}
			else{
				std::cout << "Speed must be between 1 and 20. Using default speed as 4." << std::endl;
			}
		}
		catch(...){
			std::cout << "Invalid argument was passed, speed must be an integer. Using default speed as 4." << std::endl;
		}
	}

	srand(time(NULL));

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "THE NORT");
	auto desktop = VideoMode::getDesktopMode();
	Vector2i v2i(desktop.width/2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2);
	window.setPosition(v2i);
	window.setFramerateLimit(60);

	Texture texture;
	texture.loadFromFile("background.jpg");
	Sprite sBackground(texture);

	Player p1(Color(231, 84, 128)); // PINK
	Player p2(Color(0, 0, 255)); // BLUE
	Bot bot(Color(50, 50, 50)); // GREY

	Sprite sprite;
	RenderTexture t;
	t.create(WIDTH, HEIGHT);
	t.setSmooth(true);
	sprite.setTexture(t.getTexture());
	t.clear();
	t.draw(sBackground);

	Font font;
	font.loadFromFile("Sansation_Regular.ttf");
	Text text("YOU WIN!", font, 35);
	text.setPosition(WIDTH/2-80, 20);

	bool isPlaying = true;

	std::pair <int, int> p;
	p.first = rand() % WIDTH;
	p.second = rand() % HEIGHT;
	Obstacle obs(p);

	while(window.isOpen()){
		Event e;
		while(window.pollEvent(e)){
			if(e.type == Event::Closed){
				window.close();
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::Up)){
			if(p1.direction != DOWN){
				p1.direction = UP;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Down)){
			if(p1.direction != UP){
				p1.direction = DOWN;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Left)){
			if(p1.direction != RIGHT){
				p1.direction = LEFT;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::Right)){
			if(p1.direction != LEFT){
				p1.direction = RIGHT;
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::W)){
			if(p2.direction != DOWN){
				p2.direction = UP;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::S)){
			if(p2.direction != UP){
				p2.direction = DOWN;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::A)){
			if(p2.direction != RIGHT){
				p2.direction = LEFT;
			}
		}
		if(Keyboard::isKeyPressed(Keyboard::D)){
			if(p2.direction != LEFT){
				p2.direction = RIGHT;
			}
		}

		if(!isPlaying){
			window.draw(text);
			window.display();
			continue;
		}

		for(int i=0; i<speed; i++){
			p1.tick();
			p2.tick();
			bot.tick();

			auto obstacle = obs.nextObs();
			CircleShape c(3);
			if(obstacle.first >= 0 && obstacle.second >= 0 && obstacle.first < WIDTH && obstacle.second < HEIGHT){
				field[obstacle.first][obstacle.second] = USED;

				c.setPosition(obstacle.first, obstacle.second);
				c.setFillColor(Color(50, 50, 50));
				t.draw(c);
			}

			if(field[p1.posX][p1.posY] == USED){
				// Player 2 wins
				text.setFillColor(p2.color);
				isPlaying = false;
			}
			if(field[p2.posX][p2.posY] == USED){
				// Player 1 wins
				text.setFillColor(p1.color);
				isPlaying = false;
			}

			field[p1.posX][p1.posY] = USED;
			field[p2.posX][p2.posY] = USED;
			field[bot.posX][bot.posY] = USED;

			c.setPosition(p1.posX, p1.posY);
			c.setFillColor(p1.color);
			t.draw(c);
			c.setPosition(p2.posX, p2.posY);
			c.setFillColor(p2.color);
			t.draw(c);
			c.setPosition(bot.posX, bot.posY);
			c.setFillColor(bot.color);
			t.draw(c);
			t.display();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

}
