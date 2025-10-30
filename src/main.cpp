#include <SFML/Graphics.hpp>

int main()
{
    // Tạo cửa sổ 800x600
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

    // Tạo một hình tròn
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Vòng lặp chính của chương trình
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}