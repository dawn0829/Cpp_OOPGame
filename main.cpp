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

bool trigger_event(double interval){
        double currentTime = GetTime(); 
        if(currentTime - lastUpdateTime >= interval){
            lastUpdateTime = currentTime;
            return true;
        }
        return false;  
};
bool checkele_indequee(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(element, deque[i])){
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
    private:
        Vector2 direction = {1, 0};
    public:
        deque<Vector2> body = {Vector2{4,7},Vector2{5,7},Vector2{6,7}};
        
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
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
        void player_mov(){

            if(IsKeyPressed(KEY_W) && direction.y != 1){
                direction = {0, -1};
            }
            if(IsKeyPressed(KEY_A) && direction.x != 1){
                direction = {-1, 0};
            }
            if(IsKeyPressed(KEY_S) && direction.y != -1){
                direction = {0, 1};
            }
            if(IsKeyPressed(KEY_D) && direction.x != -1){
                direction = {1, 0};
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
        if(trigger_event(0.2)){
            snake.body_update();
            CheckCollision();
        }
        snake.player_mov();
        
    }
    void CheckCollision(){
        if (Vector2Equals(snake.body[0], food.location)){
            cout << "EATSHIT" << endl;
            food.location = food.gen_pos();
            while(checkele_indequee(food.location, snake.body)){
                food.location = food.gen_pos();
            }
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