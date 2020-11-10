#include <SFML/Graphics.hpp>
#include <time.h>
#include <math.h>

#include "Tile.h"

int main()
{
    const int
    TILE_SIZE = 40;

    srand(time(NULL));

    bool game=false, ready=true;
    int board_width = 9, board_height = 9, bombs = 10, b_count = bombs, t_count = 0;

    sf::Vector2i curr;
    sf::Vector2i bomb_co;

    Tile** board = new Tile*[board_width];
    for (int i=0; i<board_width; i++)
        board[i] = new Tile[board_height];

    sf::Texture tx;
    tx.loadFromFile("img/tile.png");

    sf::Texture num_tx;
    num_tx.loadFromFile("img/num.png");

    sf::Texture smile_tx;
    smile_tx.loadFromFile("img/smile.png");

    sf::RectangleShape counter[3], time[3], smile;
    for (int i=0; i<3; i++)
    {
        counter[i].setSize(sf::Vector2f(38,70));
        counter[i].setPosition(sf::Vector2f(0.5*TILE_SIZE+i*38, 0.5*TILE_SIZE));
        counter[i].setTexture(&num_tx);
        counter[i].setTextureRect(sf::IntRect(b_count%((int)(std::pow(10,(3-i)))) / (int)(std::pow(10,(2-i))) * 76, 0, 76, 140));
        time[i].setSize(sf::Vector2f(38,70));
        time[i].setPosition(sf::Vector2f((board_width+0.5)*TILE_SIZE-(i+1)*38, 0.5*TILE_SIZE));
        time[i].setTexture(&num_tx);
        time[i].setTextureRect(sf::IntRect(t_count%((int)(std::pow(10,(i+1)))) / (int)(std::pow(10,i)) * 76, 0, 76, 140));
    }
    smile.setSize(sf::Vector2f(70,70));
    smile.setPosition(sf::Vector2f((board_width+1)*TILE_SIZE*0.5 - 35, 0.5*TILE_SIZE));
    smile.setTexture(&smile_tx);
    smile.setTextureRect(sf::IntRect(0,0,70,70));

    for(int i=0; i<board_width; i++)
    {
        for(int j=0; j<board_height; j++)
            board[i][j].set(i,j,TILE_SIZE,&tx);
    }

    sf::RenderWindow window(sf::VideoMode((board_width+1)*TILE_SIZE,(board_height+3)*TILE_SIZE), "Minesweeper", sf::Style::Close | sf::Style::Titlebar);

    window.clear(sf::Color(220,220,220));
    for (int i=0; i<board_width; i++)
    {
        for (int j=0; j<board_height; j++)
        {
            board[i][j].draw(&window);
        }
    }
    for (int i=0; i<3; i++)
    {
        window.draw(time[i]);
        window.draw(counter[i]);
    }
    window.draw(smile);
    window.display();


    sf::Clock clock;
    float deltaT, totalT=0;

    //game loop
    while(window.isOpen())
    {
        deltaT = clock.restart().asSeconds();

        //event loop
        sf::Event ev;
        while(window.pollEvent(ev))
        {
            switch(ev.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonReleased:
                if (ev.mouseButton.x > TILE_SIZE*0.5 && ev.mouseButton.x < window.getSize().x - TILE_SIZE*0.5
                    && ev.mouseButton.y > TILE_SIZE*2.5 && ev.mouseButton.y < window.getSize().y - TILE_SIZE*0.5
                    && ready)
                {
                    curr = sf::Vector2i((ev.mouseButton.x - TILE_SIZE*0.5)/TILE_SIZE, (ev.mouseButton.y - TILE_SIZE*2.5)/TILE_SIZE);

                    if (ev.mouseButton.button == sf::Mouse::Left)
                    {
                        //game start
                        if (!game)
                        {
                            game = true;
                            while (b_count > 0)
                            {
                                bomb_co.x = rand()%board_width;
                                bomb_co.y = rand()%board_height;
                                if ((!board[bomb_co.x][bomb_co.y].bomb) && !(bomb_co.x >= curr.x-1 && bomb_co.x <= curr.x+1 && bomb_co.y >= curr.y-1 && bomb_co.y <= curr.y+1))
                                {
                                    board[bomb_co.x][bomb_co.y].bomb = true;
                                    for (int i=bomb_co.x-1; i<=bomb_co.x+1; i++)
                                    {
                                        for (int j=bomb_co.y-1; j<=bomb_co.y+1; j++)
                                        {
                                            if ((i != bomb_co.x || j != bomb_co.y) && i<board_width && i>=0 && j<board_height && j>=0) board[i][j].b_around++;
                                        }
                                    }
                                    b_count--;
                                }
                            }
                            b_count = bombs;
                        }
                        if (!board[curr.x][curr.y].flagged && board[curr.x][curr.y].reveal(board, board_width, board_height))
                        {
                            game=false;
                            ready=false;
                            smile.setTextureRect(sf::IntRect(70,0,70,70));
                            for (int i=0; i<board_width; i++)
                            {
                                for (int j=0; j<board_height; j++)
                                    board[i][j].show_bomb();
                            }
                        }
                    }
                    else if (ev.mouseButton.button == sf::Mouse::Right && game)
                    {
                        if (board[curr.x][curr.y].flag()) b_count--;
                        else b_count++;

                        for (int i=0; i<3; i++)
                            counter[i].setTextureRect(sf::IntRect(b_count%((int)(std::pow(10,(3-i)))) / (int)(std::pow(10,(2-i))) * 76, 0, 76, 140));
                    }

                    window.clear(sf::Color(220,220,220));
                    for (int i=0; i<board_width; i++)
                    {
                        for (int j=0; j<board_height; j++)
                        {
                            board[i][j].draw(&window);
                        }
                    }
                    for (int i=0; i<3; i++)
                    {
                        window.draw(time[i]);
                        window.draw(counter[i]);
                    }
                    window.draw(smile);
                    window.display();
                }
                else if (ev.mouseButton.x > (board_width+1)*TILE_SIZE*0.5 - 35 && ev.mouseButton.x < (board_width+1)*TILE_SIZE*0.5 + 35 &&
                         ev.mouseButton.y > 0.5*TILE_SIZE && ev.mouseButton.y < 0.5*TILE_SIZE + 70 && ev.mouseButton.button == sf::Mouse::Left)
                {
                    game = false;
                    ready = true;

                    deltaT=0; totalT=0;

                    t_count = 0;
                    b_count = bombs;
                    for (int i=0; i<3; i++)
                    {
                        time[i].setTextureRect(sf::IntRect(t_count%((int)(std::pow(10,(i+1)))) / (int)(std::pow(10,i)) * 76, 0, 76, 140));
                        counter[i].setTextureRect(sf::IntRect(b_count%((int)(std::pow(10,(3-i)))) / (int)(std::pow(10,(2-i))) * 76, 0, 76, 140));
                    }

                    smile.setTextureRect(sf::IntRect(0,0,70,70));

                    window.clear(sf::Color(220,220,220));
                    for (int i=0; i<board_width; i++)
                    {
                        for (int j=0; j<board_height; j++)
                        {
                            board[i][j].set(i,j,TILE_SIZE,&tx);
                            board[i][j].draw(&window);
                        }
                    }
                    for (int i=0; i<3; i++)
                    {
                        window.draw(time[i]);
                        window.draw(counter[i]);
                    }
                    window.draw(smile);
                    window.display();
                }
                break;
            default:
                break;
            }
        }

        if (game)
        {
            totalT += deltaT;
            if (totalT >= 1.0)
            {
                totalT -= 1;
                t_count++;
                for (int i=0; i<3; i++)
                    time[i].setTextureRect(sf::IntRect(t_count%((int)(std::pow(10,(i+1)))) / (int)(std::pow(10,i)) * 76, 0, 76, 140));

                window.clear(sf::Color(220,220,220));
                for (int i=0; i<board_width; i++)
                {
                    for (int j=0; j<board_height; j++)
                    {
                        board[i][j].draw(&window);
                    }
                }
                for (int i=0; i<3; i++)
                {
                    window.draw(time[i]);
                    window.draw(counter[i]);
                }
                window.draw(smile);
                window.display();
            }
        }

    }

    for (int i=0; i<board_width; i++)
        delete [] board[i];
    delete [] board;
}
