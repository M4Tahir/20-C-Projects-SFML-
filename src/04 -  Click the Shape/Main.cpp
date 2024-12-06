#include <iostream>
#include <SFML/Graphics.hpp>



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
};

int main()
{
	sf::Vector2u winSize{ 600,600 };
	sf::RenderWindow window(sf::VideoMode(winSize.x, winSize.y), "Click The Shape");
	sf::Clock clock;
	std::vector<Ball>balls;
	balls.emplace_back(Ball(10, { 12,12 }, sf::Color::Green));

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
				winSize = { event.size.width,event.size.height };
				window.setView(view);
				for (auto& ball : balls) {
					ball.onWinResize({ event.size.width,event.size.height });
				}

				std::cout << "Resized: " << event.size.width << " X " << event.size.height << "\n";
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2f mousePos{ static_cast<float>(event.mouseButton.x),static_cast<float>(event.mouseButton.y) };
				balls.emplace_back(20, mousePos, sf::Color::Blue);
				if (event.mouseButton.button == sf::Mouse::Left) {
					//balls.emplace_back(Ball(20, mousePos, sf::Color::Blue));
					balls.erase(std::remove_if(balls.begin(), balls.end(), [&](const Ball& ball)
					{
						return ball.getGlobalBounds().contains(mousePos);
					}), balls.end());
				}
			}
		}
		window.clear();

		balls.erase(std::remove_if(balls.begin(), balls.end(), [&](const Ball& ball) {
			auto [bx, by] = ball.getPosition();
			return (bx <= 0 || bx >= winSize.x || by <= 0 || by >= winSize.y);
		}), balls.end());

		for (auto& ball : balls) {
			window.draw(ball);
			ball.update(dt);
		}

		window.display();
	}
}