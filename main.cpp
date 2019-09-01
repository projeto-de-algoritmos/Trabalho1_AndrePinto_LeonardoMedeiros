#include <SFML/Graphics.hpp>

using namespace sf;

const int WIDTH=600;
const int HEIGHT=480;

int main(int argc, char* argv[]){
	
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
