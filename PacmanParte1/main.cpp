#include "ProgramInfo.h"


/// <summary>
/// Sets the needed variables
/// </summary>
void Setup();
/// <summary>
/// Handles the inputs
/// </summary>
void Input();
/// <summary>
/// Handles the logic of the game
/// </summary>
void Logic();
/// <summary>
/// Draws the screen
/// </summary>
void Draw();

ProgramInfo info;

int main()
{
    Setup();
    while (info.run)
    {
        Input();
        Logic();
        Draw();
    }
}

void Setup()
{
    std::cout.sync_with_stdio(false);
    srand(time(NULL));

    unsigned short enemyNumber = 0;
    std::cout << "DO YOU FEEL LUCKY?\n HOW MANY ENEMIES CAN YOU HANDLE?";
    std::cin >> enemyNumber;
    for (size_t i = 0; i < enemyNumber; i++)
    {
        info.enemigos.push_back(Enemy(info.pacman_map.spawn_enemy));
    }
}

void Input()
{
    info.input = USER_INPUTS::NONE;
    if (ConsoleUtils::KeyPressed(VK_UP) || ConsoleUtils::KeyPressed('W'))
    {
        info.input = USER_INPUTS::UP;
    }
    if (ConsoleUtils::KeyPressed(VK_DOWN) || ConsoleUtils::KeyPressed('S'))
    {
        info.input = USER_INPUTS::DOWN;
    }
    if (ConsoleUtils::KeyPressed(VK_RIGHT) || ConsoleUtils::KeyPressed('D'))
    {
        info.input = USER_INPUTS::RIGHT;
    }
    if (ConsoleUtils::KeyPressed(VK_LEFT) || ConsoleUtils::KeyPressed('A'))
    {
        info.input = USER_INPUTS::LEFT;
    }
    if (ConsoleUtils::KeyPressed(VK_ESCAPE) || ConsoleUtils::KeyPressed('Q'))
    {
        info.input = USER_INPUTS::QUIT;
    }
}

void Logic()
{
    if (info.win || info.lose)
    {
        switch (info.input)
        {
        case QUIT:
            info.run = false;
            break;
        }
    }
    else
    {
        if (info.input == USER_INPUTS::QUIT)
            info.run = false;

        info.player.Update(&info.pacman_map, info.input, &info.enemigos);

        for (size_t i = 0; i < info.enemigos.size(); i++)
        {
            Enemy::ENEMY_STATE enemystate = info.enemigos[i].Update(&info.pacman_map, info.player.position);
            switch (enemystate)
            {
            case Enemy::ENEMY_KILLED:
                info.player.points += 50;
                break;
            case Enemy::ENEMY_DEAD:
                info.player.position = info.pacman_map.spawn_player;
                info.player.vidas--;
                break;
            }
        }
        if (info.player.vidas <= 0)
        {
            info.lose = true;
        }
        if (info.pacman_map.points <= 0)
        {
            info.win = true;
        }
    }
}

void Draw()
{
    ConsoleUtils::Console_SetPos(0, 0);
    info.pacman_map.Draw();
    //ConsoleUtils::Console_SetPos(player_x, player_y);
    //ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::DARK_YELLOW);
    //std::cout << player_char;
    info.player.Draw();

    for (size_t i = 0; i < info.enemigos.size(); i++)
    {
        info.enemigos[i].Draw();
    }

    ConsoleUtils::Console_ClearCharacter({ 0,(short)info.pacman_map.Height });
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::CYAN);
    std::cout << "POINTS: " << info.player.points << " REMAINING: " << info.pacman_map.points << std::endl;
    if (info.win)
    {
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::GREEN);
        std::cout << "YOU WIN!" << std::endl;
    }
    if (info.lose)
    {
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::RED);
        std::cout << "YOU LOSE\nGAME OVER" << std::endl;
    }
    std::cout << "FPS: " << TimeManager::getInstance().frameCount << std::endl;
    std::cout << "Time: " << TimeManager::getInstance().time << std::endl;
    std::cout << "DeltaTime: " << TimeManager::getInstance().deltaTime << std::endl;
    TimeManager::getInstance().NextFrame();
}