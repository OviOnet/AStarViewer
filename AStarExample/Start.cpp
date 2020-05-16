#define OLC_PGE_APPLICATION
#include <iostream>
#include <random>
#include "olcPixelGameEngine.h"
#include "AStar.hpp"
using namespace std;

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "A* Example";
	}

public:

	AStar::CoordinateList m_Path;
	int i = 0;
	int UpdateInterval = 0;

	int GenRandomNumber(int max)
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(0, max);

		return dist(rng);
	}

	bool CheckIfWall(AStar::CoordinateList& walls, AStar::Vec2i& input)
	{
		for (int i = 0; i < walls.size(); i++)
		{
			if (walls[i] == input)
			{
				return true;
			}
		}

		return false;
	}

	AStar::Vec2i GenerateRandomPosition(int maxWidth, int maxHeight, AStar::CoordinateList& Walls)
	{
		bool PositionCorrect = false;
		AStar::Vec2i Coords;
		while (PositionCorrect == false)
		{
			int startingPointX = GenRandomNumber(maxWidth);
			int startingPointY = GenRandomNumber(maxHeight);
			Coords = { startingPointX, startingPointY };

			if (!CheckIfWall(Walls, Coords))
				PositionCorrect = true;
		}
		

		return Coords;
	}

	bool OnUserCreate() override
	{
	
		int WindowWidth = 128;
		int WindowHeight = 128;


		//A* Pathfinder generator
		AStar::Generator generator;
		generator.setWorldSize({ WindowWidth, WindowHeight });
		generator.setHeuristic(AStar::Heuristic::octagonal);
		generator.setDiagonalMovement(true);

	
		AStar::Vec2i wall;

		for (int i = 0; i < ScreenWidth(); i++)
		{
			for (int j = 0; j < ScreenHeight(); j++)
			{
				int RandomWall = GenRandomNumber(2);

				if (RandomWall == 1)
				{
					Draw(i, j, olc::BLACK);
					wall = { i, j };
					generator.addCollision(wall);
				}
				else
				{
					Draw(i, j, olc::WHITE);
				}
			}
		}
		
		auto Walls = generator.getWalls();

		
		AStar::Vec2i StartCoords = GenerateRandomPosition(WindowWidth, WindowHeight, Walls);
		AStar::Vec2i EndCoords = GenerateRandomPosition(WindowWidth, WindowHeight, Walls);

		Draw(StartCoords.x, StartCoords.y, olc::RED);
		Draw(EndCoords.x, EndCoords.y, olc::BLUE);

		auto path = generator.findPath({ StartCoords.x, StartCoords.y }, { EndCoords.x, EndCoords.y }, this);
		

		//Remove current position of the character from the vector
		path.pop_back();
		//Current position is the last one in the vector so we have to reverse it
		std::reverse(path.begin(), path.end());

		path.pop_back();
		
		m_Path = path;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (UpdateInterval++ % 40 == 0)
		{
		  Draw(m_Path[i].x, m_Path[i].y, olc::GREEN);
			i = i % (m_Path.size() - 1);
			i++;
		}
		
		string currentPos = "X :" + to_string(m_Path[i].x) + " Y: " + to_string(m_Path[i].y);

		return true;
	}
};


int main()
{

	Example demo;
	if (demo.Construct(128, 128, 7, 7))
		demo.Start();


	return 0;
}
