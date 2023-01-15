// Markus Buchholz
//g++ sin_cos_algorithm_function.cpp -o t -I/usr/include/python3.8 -lpython3.8

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <math.h>
#include <map>
#include <algorithm>


#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


float xmin = -5.0;
float xmax = 5.0;
float ymin = -5.0;
float ymax = 5.0;
int AGENTS = 30;
int EVOLUTIONS = 100;
float A = 2.0;


//--------------------------------------------------------------------------------

struct Pos
{

    float x;
    float y;
};

//--------------------------------------------------------------------------------

float generateRandom()
{

    std::random_device engine;
    std::mt19937 generator(engine());
    std::uniform_real_distribution<float> distrib(0.0, 1.0);
    return distrib(generator);
}

//--------------------------------------------------------------------------------
float generateRandomPI()
{

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<float> distribution(0, 2 * M_PI);
    return distribution(generator);
}

//--------------------------------------------------------------------------------

float valueGenerator(float low, float high)
{

    return low + generateRandom() * (high - low);
}

//--------------------------------------------------------------------------------

Pos positionUpdateCheck(Pos actualPos)
{

    Pos Pnew = actualPos;

    if (Pnew.x < xmin)
    {
        Pnew.x = xmin;
    }

    if (Pnew.x > xmax)
    {
        Pnew.x = xmax;
    }

    if (Pnew.y < ymin)
    {
        Pnew.y = ymin;
    }

    if (Pnew.y > ymax)
    {
        Pnew.y = ymax;
    }

    return Pnew;
}
//--------------------------------------------------------------------------------

std::vector<Pos> initPosXY()
{

    std::vector<Pos> pos;

    for (int ii = 0; ii < AGENTS; ii++)
    {

        pos.push_back({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {

        funcValue.push_back(ii.x * ii.y);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

float func(Pos pos)
{

    return (pos.x * pos.y);
}

//--------------------------------------------------------------------------------

bool compareMin(std::pair<Pos, float> a, std::pair<Pos, float> b)
{

    return a.second < b.second;
}
//--------------------------------------------------------------------------------

bool compareMax(std::pair<Pos, float> a, std::pair<Pos, float> b)
{

    return a.second > b.second;
}

//-------------------------------------------------------------------------------

std::pair<Pos, float> findBestValue(std::vector<Pos> pos, std::vector<float> func)
{
    std::vector<std::pair<Pos, float>> temp;

    for (int ii = 0; ii < func.size(); ii++)
    {

        temp.push_back(std::pair<Pos, float>(pos[ii], func[ii]));
    }

    std::sort(temp.begin(), temp.end(), compareMin);

    return temp[0];
}
//--------------------------------------------------------------------------------
Pos positionUpdate(Pos best, Pos actual, float r1)
{
    Pos newPos;

    float r2 = generateRandomPI();
    float r3 = 2 * generateRandom();
    float r4 = generateRandom();

    if (r4 < 0.5)
    {
        newPos.x = actual.x + r1 * std::sin(r2) * std::abs(r3 * best.x - actual.x);
        newPos.y = actual.y + r1 * std::sin(r2) * std::abs(r3 * best.y - actual.y);
    }

    if (r4 >= 0.5)
    {
        newPos.x = actual.x + r1 * std::cos(r2) * std::abs(r3 * best.x - actual.x);
        newPos.y = actual.y + r1 * std::cos(r2) * std::abs(r3 * best.y - actual.y);
    }
    return positionUpdateCheck(newPos);
}

//--------------------------------------------------------------------------------

void runSCA()
{

    std::vector<Pos> currentPos = initPosXY();
    std::vector<float> currentfuncValue = function(currentPos);

    for (int jj = 0; jj < EVOLUTIONS; jj++)
    {
        float r1 = A - jj * (float)(A / EVOLUTIONS);

        std::pair<Pos, float> best = findBestValue(currentPos, currentfuncValue);
        for (int ii = 0; ii < AGENTS; ii++)
        {

            auto bestPos = best.first;
            auto bestValue = best.second;

            Pos newPos = positionUpdate(bestPos, currentPos[ii], r1);
            float newFunc = func(newPos);

            if (newFunc < currentfuncValue[ii])
            {
                currentPos[ii] = newPos;
                currentfuncValue[ii] = newFunc;
            }
        }
    }

    for (auto &ii : currentfuncValue)
    {
        std::cout << ii << "\n";
    }
}

//--------------------------------------------------------------------------------
void plot2D2D()
{

    std::vector<float> time; 
    std::vector<float> y1; 
    std::vector<float> y2;
    float fi = M_PI/3;

    float r1 = A;
    int jj = 0;
    while (r1 > 0.01){

        r1 = A - jj * (float)(A / EVOLUTIONS);
        jj++;
        time.push_back(jj);
        y1.push_back(r1 * std::sin (fi*jj));
        y2.push_back(r1 * std::cos (fi*jj));

    }


    plt::title("Decreasing pattern for range of sine and cosine ");
    plt::named_plot("sine", time, y1);
    plt::named_plot("cosine", time, y2);
    plt::xlabel("iteration");
    plt::ylabel("Y");
    plt::legend();
    plt::show();
}

//-------------------------------------------------------------------------------

int main()
{

    runSCA();
    plot2D2D();
}
