#include<iostream>
#include "SFML/Graphics.hpp"
#include"SFML/Graphics/Drawable.hpp"


static void moveCircle(sf::CircleShape& shape, const sf::Vector2f& newPos, const sf::Event& e)
{
	if (e.key.code == sf::Keyboard::W)
		shape.move(0, -newPos.y);
	else if (e.key.code == sf::Keyboard::S)
		shape.move(0, newPos.y);
	else if (e.key.code == sf::Keyboard::A)
		shape.move(-newPos.x, 0);
	else if (e.key.code == sf::Keyboard::D)
		shape.move(newPos.x, 0);
}

struct Ball : public sf::CircleShape {
public:
	Ball(const float radius)
		:m_Radius(radius)
	{
		this->setRadius(m_Radius);
		this->setPosition(100, 300);
		this->setFillColor(sf::Color::Magenta);
	}

	void update(float dt)
	{
		auto& [x, y] = m_Velocity;
		if (m_Velocity.x != 0.f || m_Velocity.y != 0.f)
		{
			float deceleration = 50;
			if (m_Velocity.x > 0) m_Velocity.x -= deceleration * dt;
			else if (m_Velocity.x < 0) m_Velocity.x += deceleration * dt;

			if (m_Velocity.y > 0) m_Velocity.y -= deceleration * dt;
			else if (m_Velocity.y < 0)m_Velocity.y += deceleration * dt;
		}
		if (std::abs(m_Velocity.x) < 0.1f) m_Velocity.x = 0.f;
		if (std::abs(m_Velocity.y) < 0.1f) m_Velocity.y = 0.f;
		this->move(m_Velocity * dt);
	}

	void setVelocity(const sf::Vector2f& velocity)
	{
		m_Velocity = velocity;
	}

private:
	sf::Vector2f m_Velocity{ 0,0 };
	float m_Radius;

};

int main()
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(720, 480), "Windows Basics");
	window.setFramerateLimit(60);
	sf::Clock clock;

	auto [width, height] = window.getSize();

	sf::CircleShape circle(50);
	circle.setPosition(width / 2, height / 2);
	circle.setFillColor(sf::Color::Red);

	sf::Font font;
	if (font.loadFromFile("../Poppins-Regular.ttf"))
		return EXIT_FAILURE;

	// Text
	sf::Text name("Tahir is good!", font, 50);
	name.setFillColor(sf::Color::Green);
	name.setPosition(100.f, 200.f);
	/*
	 * Since the default view never changes after the window is created, the viewed contents are always the same. So when the window is resized, everything is squeezed/stretched to the new size.
	 *	If, instead of this default behavior, you'd like to show more/less stuff depending on the new size of the window, all you have to do is update the size of the view with the size of the window.
	 *
	 *	// the event loop
	 *	sf::Event event;
	 *	while (window.pollEvent(event))
	 *	{
	 *	    // catch the resize events
	 *	    if (event.type == sf::Event::Resized)
	 *	    {
	 *	        // update the view to the new size of the window
	 *	        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
	 *	        window.setView(sf::View(visibleArea));
	 *	    }
	 *	}
	 *	Coordinates conversions
	 *	When you use a custom view, or when you resize the window without using the code above, pixels displayed on the target no longer match units in the 2D world. For example, clicking on pixel (10, 50) may hit the point (26.5, -84) of your world. You end up having to use a conversion function to map your pixel coordinates to world coordinates: mapPixelToCoords.
	 *
	 *	// get the current mouse position in the window
	 *	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	 *
	 *	// convert it to world coordinates
	 *	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
	 *	By default, mapPixelToCoords uses the current view. If you want to convert the coordinates using view which is not the active one, you can pass it as an additional argument to the function.
	 *
	 *	The opposite, converting world coordinates to pixel coordinates, is also possible with the mapCoordsToPixel function.
	 */
	 //sf::View fixedView(sf::FloatRect(0, 0, 720, 480));
	 //window.setView(fixedView);
	sf::View view = window.getDefaultView();

	//
	Ball ball(20);
	float acceleration = 1.f;

	while (window.isOpen())
	{
		sf::Event event;

		// let reset the clock each time and get time it takes to run and print it.
		float dt = clock.restart().asSeconds();
		std::cout << "FPS: " << 1.0f / dt << "\n";

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized) {
				// when window is resized then this will reset the rendering area to the new window dimension, rather than just scaling
				// and setting the rendering dim to the dim drawable etc.
				//sf::View view(sf::FloatRect(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
				//window.setView(view);

				view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
				window.setView(view);
				std::cout << event.size.width << "x" << event.size.height << " Window resized!!!\n";
				// now lets move the shape to its correct position where we want to be after resize.
				circle.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize() / 2u)));
			}
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::Resized:
					view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
					window.setView(view);
					std::cout << event.size.width << "x" << event.size.height << " Window resized!!!\n";
					// now lets move the shape to its correct position where we want to be after resize.
					circle.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize() / 2u)));
					break;

				case sf::Event::KeyPressed:
					moveCircle(circle, { 10,10 }, event);
					/*if (event.key.code == sf::Keyboard::W)
						ball.setVelocity({ 0.f, -100.f });
					if (event.key.code == sf::Keyboard::S)
						ball.setVelocity({ 0.f, 100.f });
					if (event.key.code == sf::Keyboard::A)
						ball.setVelocity({ -100.f, 0.f });
					if (event.key.code == sf::Keyboard::D)
						ball.setVelocity({ 100.f, 0.f });*/
						// for checking for weather key is pressed and hold then we can use isKeyPressed() or use repeat count to check weather is being hold or not
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
						ball.setVelocity({ 0.f, -100.f - acceleration });
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
						ball.setVelocity({ 0.f, 100.f + acceleration });
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
						ball.setVelocity({ -100.f - acceleration, 0.f });
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
						ball.setVelocity({ 100.f + acceleration, 0.f });

					acceleration += 20.f * dt;
					break;
				case sf::Event::KeyReleased:
					acceleration = 1.f;
					break;
				default:
					break;
			}
		}
		ball.update(dt);

		window.clear();

		window.draw(name);
		window.draw(circle);
		window.draw(ball);

		window.display();
	}


}
