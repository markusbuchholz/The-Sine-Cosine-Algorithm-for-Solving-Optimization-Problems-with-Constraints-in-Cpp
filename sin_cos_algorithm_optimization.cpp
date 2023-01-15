// Markus Buchholz

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <math.h>
#include <algorithm>

//--------------------------------------------------------------------------------

float xmin = 0.05;
float xmax = 2.0;
float ymin = 0.25;
float ymax = 1.3;
float zmin = 2.0;
float zmax = 15.0;

//--------------------------------------------------------------------------------

int AGENTS = 30;
int EVOLUTIONS = 2000;
float A = 2.0;
float R = 0.045;

//--------------------------------------------------------------------------------

struct Pos
{

    float x;
    float y;
    float z;
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

    if (Pnew.z < zmin)
    {
        Pnew.z = zmin;
    }

    if (Pnew.z > zmax)
    {
        Pnew.z = zmax;
    }

    return Pnew;
}
//--------------------------------------------------------------------------------

std::vector<Pos> initPosXY()
{

    std::vector<Pos> pos;

    for (int ii = 0; ii < AGENTS; ii++)
    {

        pos.push_back({valueGenerator(xmin, xmax), valueGenerator(ymin, ymax), valueGenerator(zmin, zmax)});
    }

    return pos;
}

//--------------------------------------------------------------------------------

std::vector<float> function(std::vector<Pos> pos)
{
    std::vector<float> funcValue;

    for (auto &ii : pos)
    {
        float p1{0}, p2{0}, p3{0}, p4{0};
        auto f1 = 1 - (float)(std::pow(ii.y, 2) * ii.z) / (float)(71785.0 * std::pow(ii.x, 4));

        auto f2 = (float)(4.0 * std::pow(ii.y, 2) - ii.x * ii.y) / (float)(12556.0 * (ii.y * std::pow(ii.x, 3) - std::pow(ii.x, 4))) + 1.0 / (float)(5108.0 * std::pow(ii.x, 2));

        auto f3 = 1.0 - (float)(140.45 * ii.x) / (float)(std::pow(ii.y, 2) * ii.z);

        auto f4 = ((float)(ii.x + ii.y) / (float)1.5) - 1.0;

        if (f1 > 0)
        {
            p1 = std::pow(f1, 2);
        }
        if (f2 > 0)
        {
            p2 = std::pow(f2, 2);
        }
        if (f3 > 0)
        {
            p3 = std::pow(f3, 2);
        }

        if (f4 > 0)
        {
            p4 = std::pow(f4, 2);
        }

        auto f = (ii.z + 2) * ii.y * std::pow(ii.x, 2) + R * p1 + R * p2 + R * p3 + R * p4;

        funcValue.push_back(f);
    }

    return funcValue;
}

//--------------------------------------------------------------------------------

float func(Pos pos)
{
    float p1{0}, p2{0}, p3{0}, p4{0};
    auto f1 = 1 - (float)(std::pow(pos.y, 2) * pos.z) / (float)(71785.0 * std::pow(pos.x, 4));

    auto f2 = (float)(4.0 * std::pow(pos.y, 2) - pos.x * pos.y) / (float)(12556.0 * (pos.y * std::pow(pos.x, 3) - std::pow(pos.x, 4))) + 1.0 / (float)(5108.0 * std::pow(pos.x, 2));

    auto f3 = 1.0 - (float)(140.45 * pos.x) / (float)(std::pow(pos.y, 2) * pos.z);

    auto f4 = ((float)(pos.x + pos.y) / (float)1.5) - 1.0;

    if (f1 > 0)
    {
        p1 = std::pow(f1, 2);
    }
    if (f2 > 0)
    {
        p2 = std::pow(f2, 2);
    }
    if (f3 > 0)
    {
        p3 = std::pow(f3, 2);
    }

    if (f4 > 0)
    {
        p4 = std::pow(f4, 2);
    }

    return (pos.z + 2) * pos.y * std::pow(pos.x, 2) + R * p1 + R * p2 + R * p3 + R * p4;


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
        newPos.z = actual.z + r1 * std::sin(r2) * std::abs(r3 * best.z - actual.z);
    }

    if (r4 >= 0.5)
    {
        newPos.x = actual.x + r1 * std::cos(r2) * std::abs(r3 * best.x - actual.x);
        newPos.y = actual.y + r1 * std::cos(r2) * std::abs(r3 * best.y - actual.y);
        newPos.z = actual.z + r1 * std::cos(r2) * std::abs(r3 * best.z - actual.z);
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

    auto bestPosValueSCA = findBestValue(currentPos, currentfuncValue);

    std::cout << "-------Optimization problem: tension/compression spring ----------\n";
    std::cout<< "min weight= " << std::get<1>(bestPosValueSCA)<< "\n";   
    std::cout<< "values   d= " <<std::get<0>(bestPosValueSCA).x << " D= " <<std::get<0>(bestPosValueSCA).y << " N= " <<std::get<0>(bestPosValueSCA).z << "\n";   


}
//--------------------------------------------------------------------------------

int main()
{

    runSCA();
}
