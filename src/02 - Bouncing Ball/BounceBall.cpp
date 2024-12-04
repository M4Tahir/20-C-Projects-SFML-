
#include <iostream>

#include "SFML/Graphics.hpp"
#include "./SFML/Graphics/CircleShape.hpp"


class Ball : public sf::CircleShape
{
public:
	Ball(float radius, const sf::Vector2f& pos, const sf::Color& color)
	{
		this->setRadius(radius);
		this->setFillColor(color);
		this->setPosition(pos);
	}
	void update(float dt)
	{
		//this->move(this->getPosition() + (m_Velocity * dt));
		this->move(m_Velocity * dt);
	}

	void setVelocity(const sf::Vector2f& dv)
	{
		m_Velocity = dv;
	}
	const sf::Vector2f& getVelocity() const { return m_Velocity; }
	// Provide access to the inherited draw method from sf::Drawable in case of private inheritance.
	//using sf::CircleShape::setRadius;
	//using sf::CircleShape::getRadius;
	//using sf::CircleShape::getPosition;
	//using sf::CircleShape::setPosition;
	//using sf::CircleShape::getGlobalBounds;
	//using sf::CircleShape::setFillColor;
	//using sf::CircleShape::draw;

private:
	sf::Vector2f m_Velocity{ 100,50 };
};

static void checkCollision(const sf::Vector2u& dim, Ball& ball, float dt)
{
	auto [bx, by] = ball.getPosition();
	auto [width, height] = static_cast<sf::Vector2f>(dim);
	auto [vx, vy] = ball.getVelocity();

	if (bx + ball.getRadius() * 2 >= width)
		ball.setVelocity({ -std::abs(vx), vy });
	if (bx <= 0)
		ball.setVelocity({ std::abs(vx), vy });
	if (by + ball.getRadius() * 2 >= height)
		ball.setVelocity({ vx, -std::abs(vy) });
	if (by <= 0)
		ball.setVelocity({ vx, std::abs(vy) });
}

static void increaseVelocity(Ball& ball, const float dt)
{
	auto [vx, vy] = ball.getVelocity();
	if (vx > 500)
		return;
	if (vy > 550)
		return;

	float acceleration = 10.f;
	vx += (vx > 0.f ? 1.f : -1.f) + acceleration * dt;
	vy += (vy > 0.f ? 1.f : -1.f) + acceleration * dt;
	ball.setVelocity({ vx,vy });
}

int main()
{
	sf::Vector2u dim{ 720,480 };
	auto window = sf::RenderWindow(sf::VideoMode(dim.x, dim.y), "Bouncing Ball");
	window.setFramerateLimit(60);
	int fps = 0;
	sf::Clock clock;
	std::vector<Ball> balls;
	balls.reserve(10);
	balls.emplace_back(balls.emplace_back(Ball(20, { 200, 300 }, sf::Color{ 100, 250, 100 })));

	while (window.isOpen())
	{
		sf::Event event;
		float dt = clock.restart().asSeconds();

		fps += 1;
		if (fps > 30) {
			std::cout << "FPS: " << 1.0f / dt << " Count: " << balls.size() << '\n';
			fps = 0;
		}

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				auto view = sf::View(sf::FloatRect(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
				window.setView(view);
				dim = window.getSize();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				balls.emplace_back(Ball(10 + rand() % 20,
					{ static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) },
					sf::Color{
						static_cast<uint8_t>(rand() % 255),
						static_cast<uint8_t>(rand() % 255),
						static_cast<uint8_t>(rand() % 255)
					}));
			}
		}

		window.clear();

		for (auto& ball : balls) {
			checkCollision(dim, ball, dt);
			increaseVelocity(ball, dt);
			ball.update(dt);
			window.draw(ball);
		}
		window.display();
	}

	return  0;
}