#include <iostream> 
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include "vec3d.h"
#include "sphere.h"
#include "light.h"
#include "BVH_Sphere.h"
#include "vecFunctions.h"

using namespace std;

void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

//создание проекций шаров на плоскость отображения
vector<Circle> projections(vector<sphere> Sp){
    vector<Circle> projection;
    for (int i = 0; i < Sp.size(); i++)
    {
        Circle someCircle(Sp.at(i));
        projection.push_back(someCircle);
    }
    return projection;
}

int main() 
{
    //окно и буфер

    double window_scale = 1.6;
	int width = 120 * window_scale;
	int height = 30 * window_scale;
    float scale = (float)width / height * (11.0f / 24.0f);

    //градиент света
    char gradient[] = " .:-=/r(14H9W8$@";
	int gradientSize = std::size(gradient) - 2;
    int gradientLen = std::size(gradient);



    int time = 20000;
    int speed = 1; //скорость анимации минимум 1

    vec3d camera(0);

    //вектор сфер
    vector<sphere> Sp;
    sphere Sp1(0, 0, 6, 1, 0.9);
    sphere Sp2(-2, -1.2, 5, 1.2, 1.0);
    sphere Sp3(2, -1.2, 4, 1.4, 0.8);
    
    Sp.push_back(Sp1);
    Sp.push_back(Sp2);
    Sp.push_back(Sp3);
    sphere Sp4(0, -5002, 100, 4999, 0.5);
    Sp.push_back(Sp4);

    float z = 0;

    //свет
    
    int ambienLight = 0;
    int pointLinght = 1;
    int directionalLight = 2;
    
    // light Difuse(5, 10, 20, 1);
    
    light Ambient(0, 0.2);
    light Point(1, 0.6, 2, 1, 0);
    light Directional(2, 0.2, 1, 4, 4);
    light lights[] = {Ambient, Point, Directional};

    //тест BVH
    AABB* rootBox = new AABB(0, 0, 2, 2);
    Node* rootNode = new Node(rootBox);
    BVH* tree = new BVH(rootNode);
    tree->createBVH(projections(Sp));
    cout << "BVH info:" << endl;
    tree->BVH_info(tree->root);
    sphere* test;

    cout << "Run animation? (Enter Y or N)" << endl;
    char run;
    cin >> run;

    if (run == 'Y' || run == 'y')
    {    
        SetWindow(width, height);
        char* screen = new char[width * height + 1];
        HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        DWORD dwBytesWritten = 0;
        for (int t = 0; t < time; t += speed)
        {
            Sp.at(0).center.x = 3*cos(z);  //первая сфера движется по кругу
            Sp.at(0).center.z = 6 + 3*sin(z);

            tree->createBVH(projections(Sp));

            z += 0.005;
            // vec3d camera(sp1.center.x + sp1.center.y * cos(t*0.01), sp1.center.z + sp1.center.y * sin(t*0.01), 0);
            // типо круится вокруг шара, пока не работает

            for (int i = 0; i < width; i++)
            {   
                for (int j = 0; j < height; j++)
                {
                    float x = ((float)i - (float)width /2) / (11.0 / 24.0);
                    float y = ((float)j - (float)height / 2);
                    float d = 1;

                    vec3d canvasXYZ(x / (float)width, -y / (float)height, d);
                    float tmp = 0;
                    test = tree->getSphere(canvasXYZ.x, canvasXYZ.y, tree->root);
                    if (test != nullptr){
                        tmp = traceRayV2(camera, canvasXYZ, d, 10, *test, lights);
                    }    

                    // tmp = traceRay(camera, canvasXYZ, d, 10, Sp, lights); //верися трасировки без BVH дерева
                    screen[i + j * width] = gradient[(int)(tmp * gradientSize)];
                }
            }
            screen[width * height] = '\0';
            WriteConsoleOutputCharacter(hConsole, (LPCSTR)screen, width * height, { 0, 0 }, &dwBytesWritten);
        }
        delete[] screen;
    }
    return 0;
}