#include "Tile.h"

Tile::Tile()
{

}

void Tile::set(int x, int y, int s, sf::Texture* tx)
{
    covered = true;
    bomb = false;
    flagged = false;
    b_around = 0;

    co = sf::Vector2i(x,y);
    body.setSize(sf::Vector2f(s,s));
    body.setTexture(tx);
    body.setTextureRect(sf::IntRect(630,0,70,70));
    body.setPosition(sf::Vector2f(s*(x+0.5), s*(y+2.5)));
}

void Tile::draw(sf::RenderWindow* w)
{
    w->draw(body);
}

bool Tile::reveal(Tile** b, int width, int height)
{
    covered = false;
    if (bomb)
    {
        body.setTextureRect(sf::IntRect(910,0,70,70));
        return true;
    }
    else if (!flagged)
    {
        body.setTextureRect(sf::IntRect(b_around*70,0,70,70));
        if (b_around == 0)
        {
            for (int i=-1; i<=1; i++)
            {
                for (int j=-1; j<=1; j++)
                {
                    if (co.x+i<width && co.x+i>=0 && co.y+j<height && co.y+j>=0 && b[co.x+i][co.y+j].covered) b[co.x+i][co.y+j].reveal(b, width, height);
                }
            }
        }
        return false;
    }
}

bool Tile::flag()
{
    if (flagged)
    {
        flagged = false;
        body.setTextureRect(sf::IntRect(630,0,70,70));
        return false;
    }
    else if (covered)
    {
        flagged = true;
        body.setTextureRect(sf::IntRect(700,0,70,70));
        return true;
    }
}

void Tile::show_bomb()
{
    if (bomb && !flagged && body.getTextureRect().left != 910) body.setTextureRect(sf::IntRect(840,0,70,70));
    if (flagged && !bomb) body.setTextureRect(sf::IntRect(770,0,70,70));
}
