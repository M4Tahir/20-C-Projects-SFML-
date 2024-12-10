#include<iostream>
#include "SFML/Graphics.hpp"
int main()
{
	sf::Vector2u winSize{ 720,600 };
	sf::RenderWindow window(sf::VideoMode(winSize.x, winSize.y), "Click The Shape");
	window.setFramerateLimit(120);
	sf::ContextSettings context;
	context.antialiasingLevel = 8;

	sf::Clock clock;

	// load texture (just image mapped to entity[a drawable, transformable object in scene])
	sf::Texture image;
	auto isLoaded = image.loadFromFile("../../../res/coding.jpg");
	// creates rectangle of that size and fit the image in it to make texture.
	//auto isLoaded = image.loadFromFile("../../../res/coding.png", sf::IntRect(0,0,800,600));
	if (!isLoaded) {
		std::cout << "Failed to load texture(image)\n";
		return EXIT_FAILURE;
	}
	image.setSmooth(true);

	// Can't directly draw texture we either have to embed it into text of sprite, and then we can draw it.
	sf::Sprite imageSprite(image);
	//imageSprite.setTextureRect(sf::IntRect(0, 0, 800, 600)); // can't just put random values it needs to be the same size as the texture.
	imageSprite.setTextureRect(sf::IntRect(0, 0, image.getSize().x, image.getSize().y));
	imageSprite.setPosition(0, 0);
	// Fit the image to the window size
	//float scaleX = static_cast<float>(winSize.x) / image.getSize().x;
	//float scaleY = static_cast<float>(winSize.y) / image.getSize().y;
	//float scale = std::min(scaleX, scaleY);
	//imageSprite.setScale(scale, scale);
	imageSprite.scale(0.5, 0.5);
	imageSprite.setPosition(winSize.x / 2, winSize.y / 2);
	imageSprite.setOrigin(image.getSize().x / 2, image.getSize().y / 2);
	imageSprite.setColor(sf::Color::White);


	bool isDragging = false;
	sf::Vector2f dragOffset;

	while (window.isOpen())
	{
		sf::Event event;
		float dt = clock.restart().asSeconds();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::View view(sf::FloatRect(0, 0, static_cast<float>(event.size.width),
					static_cast<float>(event.size.height)));
				window.setView(view);
				winSize = { event.size.width, event.size.height };
			}

			/*if (event.type == sf::Event::MouseButtonPressed) // this is just to check for 1 click if you want to check for holding this then use if isMouse buttonPressed.
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
					if (imageSprite.getGlobalBounds().contains(pos))
						imageSprite.setPosition(pos);
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
				//if (sf::Vector2f pos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) }; imageSprite.getGlobalBounds().contains(pos))
				if (imageSprite.getGlobalBounds().contains(pos))
					imageSprite.setPosition(pos);
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (sf::Vector2f pos = { static_cast<float>(event.mouseButton.x),static_cast<float>(event.mouseButton.y) }; imageSprite.getGlobalBounds().contains(pos))
					imageSprite.setPosition(pos);
			}*/


			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
				if (imageSprite.getGlobalBounds().contains(mousePos))
				{
					isDragging = true;
					dragOffset = imageSprite.getPosition() - mousePos;
					// As when we click a sprite we want to know the position, and then when mouse moved then we want to subtract  sprite position if we don't then
					// when we click on very corner of the sprite and the move it then the new position of the sprite is to position where the origin of the sprite
					// is in center of the mouse (mean that relative position of the mouse inside the sprite is not maintained).
					// As we have set  origin of the sprite to be center so when set new position for the sprite using mouse or other than the center of the shape will bve in that
					// location just like in our case if we don't use this offset then we move when click then the mouse new position of sprite is set so that mouse will in the
					// origin of the sprite.
				}
			}

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				isDragging = false;
			}

			if (event.type == sf::Event::MouseMoved && isDragging)
			{
				sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
				// As when we click a sprite we want to know the position, and then when mouse moved then we want to subtract  sprite position if we don't then
				// when we click on very corner of the sprite and the move it then the new position of the sprite is to position where the origin of the sprite
				// is in center of the mouse (mean that relative position of the mouse inside the sprite is not maintained).
				// As we have set  origin of the sprite to be center so when set new position for the sprite using mouse or other than the center of the shape will bve in that
				// location just like in our case if we don't use this offset then we move when click then the mouse new position of sprite is set so that mouse will in the
				// origin of the sprite.
				imageSprite.setPosition(mousePos + dragOffset);
				std::cout << dragOffset.x << " " << dragOffset.y << "\n";

			}


			if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && imageSprite.getGlobalBounds().contains(event.mouseWheelScroll.x, event.mouseWheelScroll.y))
			{
				// to check weather is moving up or down we can use delta

				std::cout << "Mouse wheel delta: " << event.mouseWheelScroll.delta << "\n";
				auto [x, y] = imageSprite.getScale();
				/*if (event.mouseWheelScroll.delta > 0)
					imageSprite.setScale(x + 0.05, y + 0.05);

				if (event.mouseWheelScroll.delta < 0)
					imageSprite.setScale(std::max(0.01f, x - 0.05f), std::max(0.01f, y - 0.05f));*/
				float scaleF = 1.f + event.mouseWheelScroll.delta * 0.1f;
				imageSprite.setScale(std::max(0.01f, x * scaleF), std::max(0.01f, y * scaleF));
			}
		}

		window.clear();

		window.draw(imageSprite);

		window.display();
	}

}