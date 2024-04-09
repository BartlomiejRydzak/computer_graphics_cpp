//Uwaga! Co najmniej C++17!!!
//Project-> ... Properties->Configuration Properties->General->C++ Language Standard == ISO C++ 17 Standard (/std:c++17)

#include "SFML/Graphics.hpp"
#include "ShareTechMono-Regular.h"
#include <numeric>
#include <iostream>

//Tak � dobrze pa�stwo widz� � TO jest zmienna globalna! Czytanie i przetwarzanie font�w w SFML jest bardzo kosztowne. Dlatego zrobimy to raz. 
//Co nie zmienia faktu, �e mo�na by to zrobi� bez zmiennej globalnej i to ca�kiem �adnie. Jak? To ju� dla Pa�stwa :-)
std::shared_ptr<sf::Font> font;
float slider = 1;
float hue2rgb(float p, float q, float t) {

    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if (t < 1. / 6)
        return p + (q - p) * 6 * t;
    if (t < 1. / 2)
        return q;
    if (t < 2. / 3)
        return p + (q - p) * (2. / 3 - t) * 6;

    return p;

}

sf::Color hsl2rgb(float h, float s, float l) {

    sf::Color result;

    if (0 == s) {
        result.r = result.g = result.b = l;
    }
    else {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        result.r = hue2rgb(p, q, h + 1. / 3) * 255;
        result.g = hue2rgb(p, q, h) * 255;
        result.b = hue2rgb(p, q, h - 1. / 3) * 255;
    }

    return result;

}
sf::Color HSBtoRGB(float H, float S, float V) {
    float R, G, B;

    if (S == 0)
    {
        R = V * 255;
        G = V * 255;
        B = V * 255;
    }
    else
    {
        float var_h = H * 6;
        if (var_h == 6) var_h = 0;
        float var_i = int(var_h);
        float var_1 = V * (1 - S);
        float var_2 = V * (1 - S * (var_h - var_i));
        float var_3 = V * (1 - S * (1 - (var_h - var_i)));

        float var_r, var_g, var_b;
        if (var_i == 0) {
            var_r = V;
            var_g = var_3;
            var_b = var_1;
        }
        else if (var_i == 1) { var_r = var_2; var_g = V; var_b = var_1; }
        else if (var_i == 2) { var_r = var_1; var_g = V; var_b = var_3; }
        else if (var_i == 3) { var_r = var_1; var_g = var_2; var_b = V; }
        else if (var_i == 4) { var_r = var_3; var_g = var_1; var_b = V; }
        else { var_r = V; var_g = var_1; var_b = var_2; }

        R = var_r * 255;
        G = var_g * 255;
        B = var_b * 255;
    }

    return sf::Color(R, G, B);
}

class hexagon : public sf::Drawable
{
protected:
    sf::Vector2f left_top, right_bottom, center;
    sf::Vector2f p[6]; // Kolejno�� punkt�w opisana w pliku PDF do laboratorium.
public:
    void Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom);
    void Draw_Border(sf::RenderTarget& target, sf::RenderStates states, sf::String name) const;
    bool rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha, float& beta) const;
private:
    float d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const;
};

//Troch� matematyki jeszcze nikomu nie zaszkodzi�o. Wi�c dwie kolejne metody to czysta matematyka.
float hexagon::d_coefficient(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p) const
{
    float A = p0.y - p1.y;
    float B = p1.x - p0.x;
    float C = p0.x * (p1.y - p0.y) - p0.y * (p1.x - p0.x);

    return 2 * ((A * p.x + B * p.y + C) / sqrt(A * A + B * B)) / sqrt(3.0f);
}

bool hexagon::rhombus(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p, float& alpha, float& beta) const
{
    float L = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
    sf::Vector2f p3(0.5f * (p0.x - p1.x) + 0.5f * sqrt(3.0f) * (p1.y - p0.y) + p0.x, 0.5f * sqrt(3.0f) * (p0.x - p1.x) - 0.5f * (p1.y - p0.y) + p0.y);

    beta = -d_coefficient(p0, p1, p) / L;
    alpha = d_coefficient(p0, p3, p) / L;

    if ((alpha > 1.0f) || (beta > 1.0f) || (alpha < 0.0f) || (beta < 0.0f)) return false; else return true;
}

void hexagon::Set_Borders(sf::Vector2f _left_top, sf::Vector2f _right_bottom)
{
    left_top = _left_top;
    right_bottom = _right_bottom;
    float a = ((right_bottom.y - left_top.y) / 2.0f + 0.5f) - 20.0f;
    center = left_top + sf::Vector2f((right_bottom.x - left_top.x) / 2.0f + 0.5f, (right_bottom.y - left_top.y) / 2.0f + 0.5f + 10);

    p[0] = center - sf::Vector2f(0.0f, a);
    p[1] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[2] = center - sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
    p[3] = center + sf::Vector2f(0.0f, a);
    p[4] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, 0.5f * a);
    p[5] = center + sf::Vector2f(0.5f * sqrt(3.0f) * a, -0.5f * a);
}

void hexagon::Draw_Border(sf::RenderTarget& target, sf::RenderStates states, sf::String name) const
{
    // Tu trzeba narysowa� ramk�. I napisy.
    sf::VertexArray line(sf::LinesStrip, 7);

    for (int i = 0; i < 6; i++) {
        line[i].position = p[i];
        line[i].color = sf::Color(0, 0, 0);
    }
    line[6].position = p[0];
    line[6].color = sf::Color(0, 0, 0);

    target.draw(line, states);

    sf::VertexArray ramka(sf::LinesStrip, 5);

    ramka[0].position = sf::Vector2f(left_top.x + 10, left_top.y + 10);
    ramka[1] = sf::Vector2f(left_top.x + 10, right_bottom.y);
    ramka[2] = sf::Vector2f(right_bottom.x - 10, right_bottom.y);
    ramka[3] = sf::Vector2f(right_bottom.x - 10, left_top.y + 10);
    ramka[4] = sf::Vector2f(left_top.x + 10, left_top.y + 10);

    ramka[0].color = sf::Color(0, 0, 0);
    ramka[1].color = sf::Color(0, 0, 0);
    ramka[2].color = sf::Color(0, 0, 0);
    ramka[3].color = sf::Color(0, 0, 0);
    ramka[4].color = sf::Color(0, 0, 0);

    target.draw(ramka, states);

    sf::Font font;
    sf::Text text;

    font.loadFromFile("MONTSERRATALTERNATES-BLACK.otf");

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(left_top.x + 20, left_top.y + 20);
    text.setString(name);
    target.draw(text, states);

    text.setPosition(p[0].x - 40, p[0].y - 10);
    text.setString(name[0]);
    target.draw(text, states);

    text.setPosition(p[2].x - 25, p[2].y - 10);
    text.setString(name[1]);
    target.draw(text, states);

    text.setPosition(p[4].x - 10, p[4].y + 10);
    text.setString(name[2]);
    target.draw(text, states);
}

class hexagon_RGB : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

void hexagon_RGB::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysowa� sze�ciok�t RGB.

    sf::VertexArray triangleStrip(sf::TriangleStrip, 4);

    triangleStrip[0].position = p[0];
    triangleStrip[0].color = sf::Color(255 * slider, 0, 0);

    triangleStrip[1].position = p[1];
    triangleStrip[1].color = sf::Color(255 * slider, 255 * slider, 0);

    triangleStrip[3].position = center;
    triangleStrip[3].color = sf::Color(255 * slider, 255 * slider, 255 * slider);

    triangleStrip[2].position = p[5];
    triangleStrip[2].color = sf::Color(255 * slider, 0, 255 * slider);

    target.draw(triangleStrip);

    triangleStrip[0].position = p[1];
    triangleStrip[0].color = sf::Color(255 * slider, 255 * slider, 0);

    triangleStrip[1].position = p[2];
    triangleStrip[1].color = sf::Color(0, 255 * slider, 0);

    triangleStrip[2].position = center;
    triangleStrip[2].color = sf::Color(255 * slider, 255 * slider, 255 * slider);

    triangleStrip[3].position = p[3];
    triangleStrip[3].color = sf::Color(0, 255 * slider, 255 * slider);

    target.draw(triangleStrip);

    triangleStrip[0].position = p[3];
    triangleStrip[0].color = sf::Color(0, 255 * slider, 255 * slider);

    triangleStrip[2].position = p[4];
    triangleStrip[2].color = sf::Color(0, 0, 255 * slider);

    triangleStrip[1].position = center;
    triangleStrip[1].color = sf::Color(255 * slider, 255 * slider, 255 * slider);

    triangleStrip[3].position = p[5];
    triangleStrip[3].color = sf::Color(255 * slider, 0, 255 * slider);

    target.draw(triangleStrip);

    Draw_Border(target, states, "RGB");

}

class hexagon_CMY : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

void hexagon_CMY::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysowa� sze�ciok�t CMY.
    sf::VertexArray triangleStrip(sf::TriangleStrip, 4);

    triangleStrip[0].position = p[0];
    triangleStrip[0].color = sf::Color((1 - slider) * 225, 255, 255);

    triangleStrip[1].position = p[1];
    triangleStrip[1].color = sf::Color((1 - slider) * 225, (1 - slider) * 225, 255);

    triangleStrip[3].position = center;
    triangleStrip[3].color = sf::Color((1 - slider) * 225, (1 - slider) * 225, (1 - slider) * 225);

    triangleStrip[2].position = p[5];
    triangleStrip[2].color = sf::Color((1 - slider) * 225, 255, (1 - slider) * 225);

    target.draw(triangleStrip);

    triangleStrip[0].position = p[1];
    triangleStrip[0].color = sf::Color((1 - slider) * 225, (1 - slider) * 225, 255);

    triangleStrip[1].position = p[2];
    triangleStrip[1].color = sf::Color(255, (1 - slider) * 225, 255);

    triangleStrip[2].position = center;
    triangleStrip[2].color = sf::Color((1 - slider) * 225, (1 - slider) * 225, (1 - slider) * 225);

    triangleStrip[3].position = p[3];
    triangleStrip[3].color = sf::Color(255, (1 - slider) * 225, (1 - slider) * 225);

    target.draw(triangleStrip);

    triangleStrip[0].position = p[3];
    triangleStrip[0].color = sf::Color(255, (1 - slider) * 225, (1 - slider) * 225);

    triangleStrip[2].position = p[4];
    triangleStrip[2].color = sf::Color(255, 255, (1 - slider) * 225);

    triangleStrip[1].position = center;
    triangleStrip[1].color = sf::Color((1 - slider) * 225, (1 - slider) * 225, (1 - slider) * 225);

    triangleStrip[3].position = p[5];
    triangleStrip[3].color = sf::Color((1 - slider) * 225, 255, (1 - slider) * 225);

    target.draw(triangleStrip);

    Draw_Border(target, states, "CMY");
}

class hexagon_HSL : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

void hexagon_HSL::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysowa� sze�ciok�t HSL.
    sf::VertexArray triangles(sf::Points);

    for (float y = p[0].y; y < p[3].y; y++) {
        for (float x = p[1].x; x < p[5].x; x++) {
            float alpha, beta;
            sf::Vector2f point(x, y);
            if (rhombus(p[0], p[1], point, alpha, beta)) {
                float H = 0;
                float S = 1 * (alpha);
                float L = 1 * (beta)*slider;

                sf::Color color = hsl2rgb(H, S, L);
                triangles.append(sf::Vertex(point, color));
            }
            if (rhombus(p[2], p[3], point, alpha, beta)) {
                float H = 0 + (beta);
                float S = 1;
                float L = 1 * (alpha)*slider;

                sf::Color color = hsl2rgb(H, S, L);
                triangles.append(sf::Vertex(point, color));
            }
            if (rhombus(p[4], p[5], point, alpha, beta)) {
                float H = 0 + (alpha);
                float S = 1 * (beta);
                float L = 1 * slider;

                sf::Color color = hsl2rgb(H, S, L);
                triangles.append(sf::Vertex(point, color));
            }
        }
    }

    target.draw(triangles, states);

    Draw_Border(target, states, "HSL");
}

class hexagon_HSB : public hexagon
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

void hexagon_HSB::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Tu trzeba narysowa� sze�ciok�t HSB.
    sf::VertexArray triangles(sf::Points);

    for (float y = p[0].y; y < p[3].y; y++) {
        for (float x = p[1].x; x < p[5].x; x++) {
            float alpha, beta;
            sf::Vector2f point(x, y);
            if (rhombus(p[0], p[1], point, alpha, beta)) {
                float H = 0;
                float S = 1 * (alpha);
                float L = 1 * (beta)*slider;

                sf::Color color = HSBtoRGB(H, S, L);
                triangles.append(sf::Vertex(point, color));
            }
            if (rhombus(p[2], p[3], point, alpha, beta)) {
                float H = 0 + (beta);
                float S = 1;
                float L = 1 * (alpha)*slider;

                sf::Color color = HSBtoRGB(H, S, L);
                triangles.append(sf::Vertex(point, color));
            }
            if (rhombus(p[4], p[5], point, alpha, beta)) {
                float H = 0 + (alpha);
                float S = 1 * (beta);
                float L = 1 * slider;

                sf::Color color = HSBtoRGB(H, S, L);
                triangles.append(sf::Vertex(point, color));
            }
        }
    }

    target.draw(triangles, states);
    Draw_Border(target, states, "HSB");
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

    hexagon_RGB h_RGB;
    hexagon_CMY h_CMY;
    hexagon_HSL h_HSL;
    hexagon_HSB h_HSB;

    sf::Clock frame_clock, around_half_secound_clock;
    sf::Int64 mean_frames_time = 0;
    std::vector<sf::Int64> frame_times;
    sf::Text text;

    font = std::make_shared<sf::Font>();
    font->loadFromMemory(&(ShareTechMono_Regular_ttf[0]), ShareTechMono_Regular_ttf.size());
    text.setFont(*font);
    text.setCharacterSize(21);
    text.setFillColor(sf::Color::Black);

    //Nie mog� unikn�� tych "magic numbers" bo bym zdradzi� w ten spos�b co ma by� w sf::Event::Resize. A tego by�my nie chcieli - prawda? :-D
    text.setPosition(10, 10);
    h_RGB.Set_Borders(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(300.0f, 294.0f));
    h_CMY.Set_Borders(sf::Vector2f(300.0f, 0.0f), sf::Vector2f(600.0f, 294.0f));
    h_HSL.Set_Borders(sf::Vector2f(0.0f, 294.0f), sf::Vector2f(300.0f, 588.0f));
    h_HSB.Set_Borders(sf::Vector2f(300.0f, 294.0f), sf::Vector2f(600.0f, 588.0f));

    around_half_secound_clock.restart();
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(window.getSize().x / 15, window.getSize().y / 5));
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y * 0.7));

    sf::RectangleShape line;
    line.setSize(sf::Vector2f(10.f, window.getSize().y * 0.8));
    line.setFillColor(sf::Color::Green);
    line.setOutlineColor(sf::Color::Green);
    line.setOutlineThickness(1.0f);
    line.setPosition(sf::Vector2f(window.getSize().x * 5 / 6 + rectangle.getSize().x / 2 - 5.f, window.getSize().y * 0.1));

    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);


        frame_clock.restart(); // Start pomiaru czasu.

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            // Tu trzeba obs�u�y� zdarzenia: zmian� rozmiaru okna oraz klikanie w �suwaczek�. 
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                float y = window.getSize().y / 2;
                float x = window.getSize().x / 3;
                h_RGB.Set_Borders(sf::Vector2f(0.0, 0.0f), sf::Vector2f(x, y));
                h_CMY.Set_Borders(sf::Vector2f(x, 0.0f), sf::Vector2f(2 * x, y));
                h_HSL.Set_Borders(sf::Vector2f(0.0f, y), sf::Vector2f(x, 2 * y));
                h_HSB.Set_Borders(sf::Vector2f(x, y), sf::Vector2f(2 * x, 2 * y));

                rectangle.setSize(sf::Vector2f(window.getSize().x / 15, window.getSize().y / 5));
                rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y / 2 - 100.f));
                line.setSize(sf::Vector2f(10.f, window.getSize().y * 0.8));
                line.setPosition(sf::Vector2f(window.getSize().x * 5 / 6 + rectangle.getSize().x / 2 - 5.f, window.getSize().y * 0.1));
            }

            if (event.type == sf::Event::MouseButtonPressed) {

                float y = event.mouseButton.y;
                if (y > window.getSize().y * 0.7)
                    rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y * 0.7));
                else if (y < window.getSize().y * 0.1)
                    rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y * 0.1));

                else
                    rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, y));

                slider = (rectangle.getPosition().y - line.getPosition().y) / (line.getSize().y - rectangle.getSize().y);

            }

            else if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    float y = event.mouseMove.y - rectangle.getSize().y / 2;
                    if (y > window.getSize().y * 0.7)
                        rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y * 0.7));
                    else if (y < window.getSize().y * 0.1)
                        rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y * 0.1));

                    else
                        rectangle.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, y));

                    slider = (rectangle.getPosition().y - line.getPosition().y) / (line.getSize().y - rectangle.getSize().y);
                }
            }

        }


        window.draw(h_RGB);
        window.draw(h_CMY);
        window.draw(h_HSL);
        window.draw(h_HSB);
        // Pewnie tu gdzie� wypada�o by dorysowa� jeszcze suwaczek... 

        window.draw(line);
        window.draw(rectangle);


        text.setString(std::to_string(mean_frames_time) + "us");
        text.setPosition(sf::Vector2f(window.getSize().x * 5 / 6, window.getSize().y * 0.95));
        window.draw(text);

        // Pomiar czasu u�rednimy w okresie oko�o 1/2 sekundy.
        frame_times.push_back(frame_clock.getElapsedTime().asMicroseconds());
        if (around_half_secound_clock.getElapsedTime().asSeconds() >= 0.5f && frame_times.size() >= 1)
        {
            mean_frames_time = (sf::Int64)((float)std::reduce(frame_times.begin(), frame_times.end()) / (float)frame_times.size());
            frame_times.clear();
            around_half_secound_clock.restart();
        }

        frame_clock.restart(); // Stop pomiaru czasu.
        window.display();
    }

    //Hmmm ... :-/
    font.reset();

    return 0;
}