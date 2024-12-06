
#include<iostream>
#include "SFML/Graphics.hpp"


class Paddle : public sf::RectangleShape
{
public:
	Paddle(const sf::Vector2f& pos, const sf::Vector2f& dim, const sf::Color& color)
	{
		this->setSize(dim);
		this->setPosition(pos);
		this->setFillColor(color);
	}

	void onWinResize(const sf::Vector2u& newWinSize)
	{
		auto [x, y] = this->getPosition();
		if (x == 0.f) // left paddle
			this->setPosition({ x,newWinSize.y / 2 - this->getSize().y / 2 });
		else // right paddle
			this->setPosition({ static_cast<float>(newWinSize.x) - this->getSize().x, newWinSize.y / 2 - this->getSize().y / 2 });
	}
};

class Ball : public sf::CircleShape
{
public:
	Ball(float radius, const sf::Vector2f& pos, const sf::Color& color)
	{
		this->setRadius(radius);
		this->setOrigin({ radius,radius });
		this->setPosition(pos);
		this->setFillColor(color);
	}
	void update(float dt)
	{
		this->move(m_Velocity * dt);
	}

	void onWinResize(const sf::Vector2u& newWinSize)
	{
		m_WinDim = newWinSize;
		auto [x, y] = this->getPosition();
		//float relativeX = x / static_cast<float>(newWinSize.x);
		//float relativeY = y / static_cast<float>(newWinSize.y);
		// As to ensure that after resizing the ball remain the correct bounds lets clamp the x, and y. clamp(value, min, max) if value is < min return min, if grater than max then return max else return value.
		x = std::clamp(x, this->getRadius(), static_cast<float>(newWinSize.x) - this->getRadius());
		y = std::clamp(y, this->getRadius(), static_cast<float>(newWinSize.y) - this->getRadius());

		this->setPosition({ x,y });
	}


	void setVelocity(const sf::Vector2f& dv)
	{
		m_Velocity = dv;
	}
	const sf::Vector2f& getVelocity() const { return m_Velocity; }
private:
	int dirX = rand() % 2 == 0 ? 1 : -1;
	int dirY = rand() % 2 == 0 ? 1 : -1;
	sf::Vector2f m_Velocity{ static_cast<float>(50 * dirX),static_cast<float>(20 * dirY) };
	sf::Vector2u m_WinDim;
}

class GameLogic
{
public:
	GameLogic() = default;

	static void handleBallCollision(Ball& ball, const sf::Vector2u& windowSize)
	{
		auto [bx, by] = ball.getPosition();
		auto [vx, vy] = ball.getVelocity();
		if (by + ball.getRadius() >= static_cast<float>(windowSize.y))
			ball.setVelocity({ vx, -std::abs(vy) });

		if (by - ball.getRadius() <= 0.f)
			ball.setVelocity({ vx, std::abs(vy) });

		if (bx - ball.getRadius() <= 0 || bx + ball.getRadius() >= windowSize.x) {
			ball.setPosition(windowSize.x / 2, windowSize.y / 2);
			int dirX = rand() % 2 == 0 ? 1 : -1;
			int dirY = rand() % 2 == 0 ? 1 : -1;
			ball.setVelocity({ static_cast<float>(20 * dirX),static_cast<float>(20 * dirY) });
		}


	}

	static void ballCollisionWithPaddle(Ball& ball, const Paddle& paddle)
	{
		if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
		{
			auto [vx, vy] = ball.getVelocity();
			ball.setVelocity({ -vx, vy });
		}
	}

	static void handlePaddleMovement(Paddle& paddle, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, float speed, float dt, const sf::Vector2u& winSize)
	{
		auto [_, py] = paddle.getPosition();

		if (py > 0.f && sf::Keyboard::isKeyPressed(upKey))
			paddle.move({ 0, -speed * dt });
		if (py + paddle.getSize().y < winSize.y && sf::Keyboard::isKeyPressed(downKey))
			paddle.move({ 0, speed * dt });
	}

	static void increaseVelocity(Ball& ball, const float dt)
	{
		auto [vx, vy] = ball.getVelocity();
		if (vx > 300)
			return;
		if (vy > 300)
			return;

		float acceleration = 10.f;
		vx += (vx > 0.f ? 1.f : -1.f) * acceleration * dt;
		vy += (vy > 0.f ? 1.f : -1.f) * acceleration * dt;
		ball.setVelocity({ vx,vy });
	}
};

int main()
{
	// No option to enable antialiasing for individual shape but can be enabled globally.
	sf::ContextSettings setting;
	setting.antialiasingLevel = 8; // 8x antialiasing.

	sf::Vector2u winSize{ 720,480 };
	auto window = sf::RenderWindow(sf::VideoMode(winSize.x, winSize.y), "Ball and Paddle", sf::Style::Default, setting);
	window.setFramerateLimit(120);

	// Line that we draw in the middle of the screen.
	sf::Vertex line[] = {
		sf::Vertex({static_cast<float>(winSize.x) / 2, 0}), // starting points to draw line.
		sf::Vertex({static_cast<float>(winSize.x) / 2,static_cast<float>(winSize.y)}),

		sf::Vertex({0,static_cast<float>(winSize.y) / 2}),
		sf::Vertex({static_cast<float>(winSize.x),static_cast<float>(winSize.y) / 2}),
	};
	std::cout << "";

	sf::Clock clock;

	sf::Vector2f paddleDim{ 20,80 };
	Paddle paddle1({ 0,static_cast<float>(winSize.y) / 2 - 40 }, paddleDim, sf::Color::Green);
	Paddle paddle2({ static_cast<float>(winSize.x) - 20,static_cast<float>(winSize.y) / 2 - 40 }, paddleDim, sf::Color::Green);
	Ball ball(20, { static_cast<float>(winSize.x - 10) / 2 ,static_cast<float>(winSize.y - 10) / 2 }, sf::Color::Blue);

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::View view(sf::FloatRect(0, 0, static_cast<float>(event.size.width),
					static_cast<float>(event.size.height)));
				window.setView(view);
				winSize = { event.size.width,event.size.height };
				line[0].position = { static_cast<float>(winSize.x) / 2, 0 };
				line[1].position = { static_cast<float>(winSize.x) / 2,static_cast<float>(winSize.y) };
				line[2].position = { 0,static_cast<float>(winSize.y) / 2 };
				line[3].position = { static_cast<float>(winSize.x),static_cast<float>(winSize.y) / 2 };

				paddle1.onWinResize(winSize);
				paddle2.onWinResize(winSize);
				ball.onWinResize(winSize);

				std::cout << "Resized: " << event.size.width << " X " << event.size.height << "\n";
			}
		}
		GameLogic::handlePaddleMovement(paddle1, sf::Keyboard::W, sf::Keyboard::S, 400, dt, winSize);
		GameLogic::handlePaddleMovement(paddle2, sf::Keyboard::Up, sf::Keyboard::Down, 400, dt, winSize);

		ball.update(dt);

		GameLogic::handleBallCollision(ball, winSize);
		GameLogic::ballCollisionWithPaddle(ball, paddle1);
		GameLogic::ballCollisionWithPaddle(ball, paddle2);
		GameLogic::increaseVelocity(ball, dt);

		window.clear();
		window.draw(line, 4, sf::Lines);
		window.draw(paddle1);
		window.draw(paddle2);
		window.draw(ball);
		window.display();
	}
}