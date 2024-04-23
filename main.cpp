#include <iostream>
#include <raylib.h>

using namespace std;
Color color_bg = {255,255,0,70};
Color color_food = {0,0,0,255};
const int cellsize = 30;
const int cellcount = 25;
class Food
{
    private:
        /* data */
        Vector2 location_f = {5, 6};
    public:
        void Draw(){
            DrawRectangle(location_f.x*cellsize, location_f.y*cellsize, cellsize, cellsize, color_food);
        }
};


int main () {
    cout << "Start..." << endl;

    InitWindow(cellsize*cellcount, cellsize*cellcount, "Snack!");
    SetTargetFPS(60);
    Food food = Food();
    while (WindowShouldClose() == false){
        BeginDrawing();

        ClearBackground(color_bg);

        food.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}