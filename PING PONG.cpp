// CT-063 | CT-060 | CT-064 | CT-070

#include <iostream>
#include <raylib.h>
using namespace std;

Color Dblue = Color{0, 82, 172, 255};
Color Lblue = Color{102, 191, 255, 255};
Color White = Color{255, 255, 255, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int player2_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth())
        {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }
    void Update2()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }

        if (x - radius <= 0)
        {
            player2_score++;
            ResetBall();
        }
        if (x + radius >= GetScreenWidth())
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{

protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    virtual void Update(int) = 0;
    virtual void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }
};

class PlayerPaddle : public Paddle
{
public:
    void Update(int a = 0)
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};
class Player2Paddle : public Paddle
{
public:
    void Update(int a = 0)
    {
        if (IsKeyDown(KEY_W))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_S))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class Timer
{
private:
    float startTime;
    float duration;

public:
    Timer(float timeDuration = 0)
    {
        startTime = GetTime();
        duration = timeDuration;
    }

    float GetElapsedTime()
    {
        return GetTime() - startTime;
    }

    bool IsTimerComplete()
    {
        return GetElapsedTime() >= duration;
    }
};

class pingpong
{
protected:
    Ball ball;
    PlayerPaddle player;
    Player2Paddle player2;
    CpuPaddle cpu;

    const int screen_width = 1280;
    const int screen_height = 800;

public:
    bool two_player()
    {
        Timer gameTimer(60.0f);
        player.width = 25;
        player.height = 120;
        player.x = screen_width - player.width - 10;
        player.y = screen_height / 2 - player.height / 2;
        player.speed = 6;

        player2.height = 120;
        player2.width = 25;
        player2.x = 10;
        player2.y = screen_height / 2 - cpu.height / 2;
        player2.speed = 6;
        ball.radius = 20;
        ball.x = screen_width / 2;
        ball.y = screen_height / 2;
        ball.speed_x = 7;
        ball.speed_y = 7;

        while (WindowShouldClose() == false && !gameTimer.IsTimerComplete())
        {
            BeginDrawing();
            // Updating
            ball.Update2();
            player.Update();
            player2.Update();

            // Checking for collisions
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.height}))
            {
                ball.speed_x *= -1;
            }

            // Drawing
            ClearBackground(Dblue);
            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Dblue);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Lblue);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

            ball.Draw();
            player2.Draw();
            player.Draw();

            DrawText(TextFormat("%i", player2_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("Time: %.1f", gameTimer.GetElapsedTime()), 10, 10, 20, WHITE);

            EndDrawing();
        }
        CloseWindow();
        InitWindow(screen_width, screen_height, "Score");
        while (!IsKeyPressed(KEY_Y))
        {
            BeginDrawing();
            ClearBackground(Dblue);
            DrawText(TextFormat("Player 2 \n%i", player2_score), 3 * screen_width / 4 - 100, 80, 50, WHITE);
            DrawText(TextFormat("Player 1 \n%i", player_score), screen_width / 4 - 100, 80, 50, WHITE);

            if (player2_score > player_score)
            {
                DrawText(TextFormat("%s", "PLAYER 2 WINS!"), screen_width / 2 - 200, 400, 50, WHITE);
            }
            else if (player2_score < player_score)
            {
                DrawText(TextFormat("%s", "PLAYER 1 WINS!"), screen_width / 2 - 200, 400, 50, WHITE);
            }
            else
            {
                DrawText(TextFormat("%s", "GAME TIES!"), screen_width / 2 - 180, 400, 50, WHITE);
            }
            DrawText(TextFormat("%s", "Do you want to play again? \n If yes (Press Y)\n If No (Press E)"), screen_width / 2 - 180, 500, 50, WHITE);

            if (IsKeyPressed(KEY_E))
            {
                return true;
                break;
            }

            EndDrawing();
        }
        player2_score = 0;
        player_score = 0;
        CloseWindow();
    }
    bool one_player()
    {
        Timer gameTimer(60.0f);
        ball.radius = 20;
        ball.x = screen_width / 2;
        ball.y = screen_height / 2;
        ball.speed_x = 7;
        ball.speed_y = 7;

        player.width = 25;
        player.height = 120;
        player.x = screen_width - player.width - 10;
        player.y = screen_height / 2 - player.height / 2;
        player.speed = 6;

        cpu.height = 120;
        cpu.width = 25;
        cpu.x = 10;
        cpu.y = screen_height / 2 - cpu.height / 2;
        cpu.speed = 6;

        while (WindowShouldClose() == false && !gameTimer.IsTimerComplete())
        {
            BeginDrawing();
            // Updating
            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            // Checking for collisions
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
            {
                ball.speed_x *= -1;
            }

            // Drawing
            ClearBackground(Dblue);
            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Dblue);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Lblue);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball.Draw();
            cpu.Draw();
            player.Draw();
            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("Time: %.1f", gameTimer.GetElapsedTime()), 10, 10, 20, WHITE);

            EndDrawing();
        }
        CloseWindow();

        InitWindow(screen_width, screen_height, "Score");

        while (!IsKeyPressed(KEY_Y))
        {
            BeginDrawing();
            ClearBackground(Dblue);
            DrawText(TextFormat("CPU \n%i", cpu_score), screen_width / 4 - 80, 80, 50, WHITE);
            DrawText(TextFormat("Player \n%i", player_score), 3 * screen_width / 4 - 80, 80, 50, WHITE);
            if (cpu_score > player_score)
            {
                DrawText(TextFormat("%s", "CPU WINS!"), screen_width / 2 - 170, 400, 50, WHITE);
            }
            else if (cpu_score < player_score)
            {
                DrawText(TextFormat("%s", "PLAYER WINS!"), screen_width / 2 - 170, 400, 50, WHITE);
            }
            else
            {
                DrawText(TextFormat("%s", "GAME TIES!"), screen_width / 2 - 170, 400, 50, WHITE);
            }
            DrawText(TextFormat("%s", "Do you want to play again? \n If yes (Press Y)\n If No (Press E)"), screen_width / 2 - 180, 500, 50, WHITE);

            if (IsKeyPressed(KEY_E))
            {
                return true;
                break;
            }
            EndDrawing();
        }
        cpu_score = 0;
        player_score = 0;
        CloseWindow();
    }
};
bool gameplay()
{
    pingpong p1;
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Ping Pong");

    InitWindow(screen_width, screen_height, "My Pong Game!");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        bool escape = false;
        BeginDrawing();
        ClearBackground(Dblue);
        DrawText("GAME STARTS!", screen_width / 2 - 20 - MeasureText("GAME START", 60) / 2, 125, 60, Yellow);
        DrawText("Select Gameplay Type:", screen_width / 2 - 20 - MeasureText("Select No. of players", 60) / 2, 275, 60, Yellow);
        DrawText("CPU/Player : \n(PRESS A)", screen_width / 3 * 0.35 - MeasureText("Player 1", 60) / 2, 450, 40, Yellow);
        DrawText("2 Players : \n(PRESS B) ", (screen_width / 4) * 2 - MeasureText("Player 2", 60) / 2, 450, 40, Yellow);
        DrawText("Exit : \n(PRESS E) ", (screen_width / 4) * 3.5 - MeasureText("Player 2", 60) / 2, 450, 40, Yellow);

        if (IsKeyPressed(KEY_A))
        {
            escape = p1.one_player();
            break;
        }
        else if (IsKeyPressed(KEY_B))
        {
            escape = p1.two_player();
            break;
        }
        else if (IsKeyPressed(KEY_E) || escape)
        {
            return false;
        }
        EndDrawing();
    }
}

int main()
{
    bool ans = true;

    while (!IsKeyPressed(KEY_E))
    {
        ans = gameplay();
    }

    CloseWindow();
    return 0;
}