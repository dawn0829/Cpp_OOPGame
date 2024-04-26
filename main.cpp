#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;
Color color_bg = {255,255,0,70};
Color color_snack = {0,0,0,255};
const int cellsize = 30;
const int cellcount = 25;

double lastUpdateTime = 0;
bool running = true;

static int score;

bool trigger_event(double interval){
        double currentTime = GetTime(); 
        if (currentTime - lastUpdateTime >= interval){
            lastUpdateTime = currentTime;
            return true;
        }
        return false;  
};
bool checkele_indequee(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if (Vector2Equals(element, deque[i])){
            return true;
        }
    }
    return false;
}
class Food
{
    private:
        Texture2D texture;

    public:
        Vector2 location;
        Food(){
            Image food_img = LoadImage("./Graphic/food.png");
            texture = LoadTextureFromImage(food_img);
            UnloadImage(food_img);
            location = gen_pos();
        }
        ~Food(){
            UnloadTexture(texture);
        }
        void draw_obj(){
            DrawTexture(texture, location.x*cellsize, location.y*cellsize, WHITE);
        }
        Vector2 gen_pos(){
            float x = GetRandomValue(0, cellcount - 1);
            float y = GetRandomValue(0, cellcount - 1);
            return Vector2{x , y};
        }
};
class Snake
{
    public:
        Vector2 direction = {1, 0};
        deque<Vector2> body = {Vector2{4,7},Vector2{5,7},Vector2{6,7}};
        bool addsegment = false;
        Snake(/* args */){
            
        };
        ~Snake(){

        };
        void draw_obj(){
            for(unsigned int i = 0; i < body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{x*cellsize, y*cellsize, (float)cellsize, (float)cellsize};
                DrawRectangleRounded(segment, 0.5, 6, color_snack); 
            }
            Rectangle head = Rectangle{body[0].x*cellsize, body[0].y*cellsize, (float)cellsize, (float)cellsize};
            DrawRectangleRounded(head, 0.5, 6, GREEN); 

        }
        void body_update(){
            body.push_front(Vector2Add(body[0], direction));
            if (addsegment){
                addsegment = false;
            }
            else
            body.pop_back();
            
        }
        void player_mov(){

            if (IsKeyPressed(KEY_W) && direction.y != 1){
                direction = {0, -1};
                running = true;
            }
            if (IsKeyPressed(KEY_A) && direction.x != 1){
                direction = {-1, 0};
                running = true;
            }
            if (IsKeyPressed(KEY_S) && direction.y != -1){
                direction = {0, 1};
                running = true;
            }
            if (IsKeyPressed(KEY_D) && direction.x != -1){
                direction = {1, 0};
                running = true;
            }
        }
        
};
class Game{
    private:
        Food food;
        Snake snake;
        
    public:
    Game():food(),snake(){};
     

    void Draw()
    {
        food.draw_obj();
        snake.draw_obj();
    }
    void Update()
    {   
        if (running)
        {
            if  (trigger_event(0.2))
            {
                snake.body_update();
                CheckCollisionFood();
                CheckCollisionEdge();  
                CheckCollisionTail();
            }  
        }
        snake.player_mov();
    }

    void CheckCollisionFood(){
        if (Vector2Equals(snake.body[0], food.location)){
            cout << "Eating" << endl;
            score ++;
            snake.addsegment = true;
            food.location = food.gen_pos();
            while(checkele_indequee(food.location, snake.body)){
                food.location = food.gen_pos();
            }
        }
    }
    void CheckCollisionEdge(){
        if (snake.body[0].x == -1 || snake.body[0].x == cellcount || snake.body[0].y == -1 || snake.body[0].y == cellcount){
            cout << "GameOver" << endl;
            GameOver();
        }
    }
    void Reset(){
        snake.direction = {1, 0};
        snake.body = {Vector2{6,7},Vector2{5,7},Vector2{4,7}};
        score = 0;
    }
    void GameOver(){
        cout << "You got " << score << " score! Move to restart" << endl;
        Reset();
        running = false;

    }
    void CheckCollisionTail(){
        deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if (checkele_indequee(snake.body[0], headlessbody)){
            GameOver();
        }
    }

};



int main () {
    cout << "Start..." << endl;
    InitWindow(cellsize*cellcount, cellsize*cellcount, "Snake Game!");
    SetTargetFPS(60);
    Game game = Game();
    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(color_bg);

        game.Update();
        game.Draw();
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}