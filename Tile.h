#include <SFML/Graphics.hpp>

class Tile
{
    public:
        Tile();
        void set(int,int,int,sf::Texture*);
        void draw(sf::RenderWindow*);
        bool reveal(Tile**,int,int);
        bool flag();
        void show_bomb();

        bool bomb, covered, flagged;
        int b_around;

    private:
        sf::RectangleShape body;
        sf::Vector2i co;
};
