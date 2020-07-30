/**  ************************************************************
 * Traveling Salesman Problem
 *
 * Author: Rosemary A. Akoh
 *
 * Compile: g++ -std=c++11 tsp.cpp -o TSP
 *
 * Usage: ./TSP <allcities.txt> <visitedcities.txt>
***************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <queue>
#include <string>
#include <climits>
#include <math.h>
#include <unordered_map>
#include <vector>
#include <string.h>
#include <string>
#include <ctime>
using namespace std;

// class to get route
class Route
{
public:
	string path;
	double dist;

	Route(string pathIn, double distIn)
		: path(pathIn), dist(distIn) {}
};

// Graph class
class Graph
{
public:
	double lat;
	double lon;
	string city;
	string county;
	string state;

	Graph(double latIn, double lonIn, string cityIn, string countyIn, string stateIn)
		: lat(latIn), lon(lonIn), city(cityIn), county(countyIn), state(stateIn) {}
};

// class to get distance from one location to another
class PairMST
{
public:
	Graph fromGraph;
	Graph toGraph;
	double dist;
	PairMST(Graph fromGraphIn, Graph toGraphIn, double distIn)
		: fromGraph(fromGraphIn), toGraph(toGraphIn), dist(distIn) {}
};

// function to calculate distance
double calDistance(Graph G1, Graph G2)
{
	double lat1 = G1.lat;
	double lat2 = G2.lat;
	double lon1 = G1.lon;
	double lon2 = G2.lon;
	double dlon = lon2 - lon1;
	double dlat = lat2 - lat1;
	double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = 6373 * c;
	return d;
}

// function to get the complete path and distance
Route getDist(class vector <Graph> myGraph)
{
	int size = myGraph.size();
	double dist = 0.0;
	string path = myGraph[0].city + "|" + myGraph[0].county + "|" + myGraph[0].state + " --> ";
	int i = 0;
	for (i = 0; i < size - 1; i++)
	{
		path += myGraph[i + 1].city + "|" + myGraph[i + 1].county + "|" + myGraph[i + 1].state + " --> ";
		dist += calDistance(myGraph[i], myGraph[i + 1]);
	}
	class Route myRoute(path, dist);
	return myRoute;
}

// function to read file
vector <Graph> readFile(string fileName, bool isFullFile)
{
	ifstream in;
	in.open(fileName, ios::in);
	string str = "";
	int position;
	class vector <Graph> myGraph;
	double lat = 0.0;
	double lon = 0.0;

	// Read the file
	while (getline(in, str))
	{
		str = str + "\n";
		position = str.find('|');
		if (isFullFile) {
			lat = stod(str.substr(0, position));

			// Change input file to continue reading file from longtitude
			str = str.substr(position + 1, str.length());
			position = str.find('|');
			lon = stod((str.substr(0, position)));

			// Change input file to continue reading file from City name
			str = str.substr(position + 1, str.length());
			position = str.find('|');
		}
		string city = str.substr(0, position);

		// Change input file to continue reading file from County
		str = str.substr(position + 1, str.length());
		position = str.find('|');
		string county = str.substr(0, position);

		// Change input file to continue reading file from State
		str = str.substr(position + 1, str.length());
		position = str.find('|');
		string state = str.substr(0, position);

		class Graph myGraph1(lat, lon, city, county, state);
		myGraph.push_back(myGraph1);
	}
	in.close();
	return myGraph;
}

// function to switch contents of Graph
vector <Route> getTempGraph(vector <Route> fullRoute, vector <Graph> GraphIn, int intArray[])
{
	class vector <Graph> tempGraph = GraphIn;
	tempGraph.push_back(GraphIn[0]);

	for (int i = 0; i < GraphIn.size() - 1; i++)
	{
		tempGraph[i + 1] = GraphIn[intArray[i]];
	}

	fullRoute.push_back(getDist(tempGraph));
	tempGraph.clear();
	return fullRoute;
}

// function to permute Graph contents
vector <Route> permute(vector <Route> fullRoute, vector <Graph> GraphIn, int intArray[], int size, int numOfCom)
{
	// base case
	if (size == 1)
	{
		fullRoute = getTempGraph(fullRoute, GraphIn, intArray);
	}

	for (int i = 0; i < size; i++)
	{
		// recursive call to permute function
		fullRoute = permute(fullRoute, GraphIn, intArray, size - 1, numOfCom);
		if (size % 2 == 1)
			swap(intArray[0], intArray[size - 1]);

		else
			swap(intArray[i], intArray[size - 1]);
	}
	return fullRoute;
}

// function to handle command line argument
bool fileNotFound(string fileName)
{
	ifstream in;
	in.open(fileName, ios::in);
	if (!in)
	{
		cout << "Unable to open file " << fileName;
		return true;
	}
	in.close();
	return false;
}

// function to perform brute force algorithm
void bruteForce(vector <Graph> myGraph2)
{
	int num = myGraph2.size();
	int* cityArray = new int[num];
	for (int i = 0; i < num - 1; i++)
	{
		cityArray[i] = i + 1;
	}
	cout << endl;
	class vector <Route> fullRoute;
	fullRoute = permute(fullRoute, myGraph2, cityArray, num - 1, num - 1);

	double minDist;
	double minIndex = 0;
	for (int i = 0; i < fullRoute.size(); i++)
	{
		if (i == 0)
			minDist = fullRoute[i].dist;
		else if (fullRoute[i].dist < minDist)
		{
			minDist = fullRoute[i].dist;
			minIndex = i;
		}
	}
	cout << "Brute Force:" << endl;
	cout << "Minimum distance path is " << endl;
	cout << fullRoute[minIndex].path;
	cout << "Distance is ";
	cout << minDist << endl;
	cout << "===============" << endl;

}

// function to get the minimum spanning tree
void MST(vector <Graph> myGraph2)
{
	cout << endl;
	cout << "MST:" << endl;
	cout << "Minimum distance path is " << endl;
	class vector <Graph> graphMST = myGraph2;
	class vector <PairMST> pair;
	double minDistMST = 0;
	int minIndexMST = 1;
	int totalDistMST = 0;
	cout << graphMST[0].city << "|" << graphMST[0].county << "|" << graphMST[0].state;
	while (graphMST.size() > 1)
	{
		minDistMST = 0;
		int minIndexMST = 1;
		for (int i = 1; i < graphMST.size(); i++)
		{
			if (i == 1)
				minDistMST = calDistance(graphMST[0], graphMST[i]);
			else if (calDistance(graphMST[0], graphMST[i]) < minDistMST)
			{
				minDistMST = calDistance(graphMST[0], graphMST[i]);
				minIndexMST = i;
			}
		}

		class PairMST pairTemp(graphMST[0], graphMST[minIndexMST], minDistMST);
		pair.push_back(pairTemp);
		totalDistMST += minDistMST;
		cout << " --> " << pairTemp.toGraph.city << "|" << pairTemp.toGraph.county << "|" << pairTemp.toGraph.state;
		graphMST[0] = graphMST[minIndexMST];
		graphMST.erase(graphMST.begin() + minIndexMST);
	}
	class PairMST pairTemp(graphMST[0], myGraph2[0], calDistance(graphMST[0], myGraph2[0]));
	pair.push_back(pairTemp);
	totalDistMST += calDistance(graphMST[0], myGraph2[0]);
	cout << " --> " << pairTemp.toGraph.city << "|" << pairTemp.toGraph.county << "|" << pairTemp.toGraph.state;
	cout << " --> " << "Distance is " << totalDistMST << endl;

	cout << "===============" << endl;
}

int main(int argc, char const *argv[])
{

	if (!(argc > 2 && argv[2] != nullptr && argv[2][0] != 0))
	{
		cout << "Invalid parameters";
		return 1;
	}

	class vector <Graph> myGraph;
	class vector <Graph> myGraph2;
	class vector <Graph> temp;
	const char* fileName = argv[1];
	const char* fileName2 = argv[2];

	if (fileNotFound(fileName) || fileNotFound(fileName2))
	{
		return 1;
	}

	myGraph = readFile(fileName, true);
	myGraph2 = readFile(fileName2, false);
	if (myGraph2.size() < 3)
	{
		cout << "Number of visited cities cannot be less than 3" << "\n";
		return 1;
	}

	for (int i = 0; i < myGraph2.size(); i++)
	{
		for (int j = 0; j < myGraph.size(); j++)
		{
			if (myGraph2[i].city == myGraph[j].city && myGraph2[i].county == myGraph[j].county &&
				myGraph2[i].state == myGraph[j].state)
			{
				myGraph2[i] = myGraph[j];
			}
		}
	}
	for (int i = 0; i < myGraph2.size(); i++)
	{
		if (myGraph2[i].lat == 0.0 && myGraph2[i].lon == 0.0)
		{
			cout << "One or more city not found in all cities file" << "\n";
			return 1;
		}
	}

	cout << "Cities to visit:" << endl;
	for (int i = 0; i < myGraph2.size(); i++)
	{
		cout << myGraph2[i].city << "|" << myGraph2[i].county << "|" << myGraph2[i].state;
	}
	cout << "Enter the city at the beginning of the tour: ";
	string startLocation;
	getline(cin, startLocation);
	bool cityMatch = false;
	for (int i = 0; i < myGraph2.size(); i++)
	{
		if ((startLocation == myGraph2[i].city + "|" + myGraph2[i].county + "|" + (myGraph2[i].state).substr(0, 2)))
		{
			temp.push_back(myGraph2[0]);
			myGraph2[0] = myGraph2[i];
			myGraph2[i] = temp[0];
			cityMatch = true;
			break;
		}
	}
	if (!cityMatch)
	{
		cout << "===============" << endl;
		cout << "Invalid input " + startLocation + ". Using " + myGraph2[0].city + "|" + myGraph2[0].county + "|" + (myGraph2[0].state).substr(0, 2) + " as the first city " + "\n";
		cout << "===============" << endl;
	}

	int choice;

	cout << "Enter your choice: 1 for Brute Force and 2 for MST" << endl;
	cin >> choice;

	if (choice == 1)
	{
		float c1 = clock();
		bruteForce(myGraph2);
		float c2 = clock();
		float totaltime = c2 - c1;
		cout << "Brute Force time: " << (totaltime / (CLOCKS_PER_SEC)) << " sec\n";

	}
	else if (choice == 2)
	{
		float c11 = clock();
		MST(myGraph2);
		float c22 = clock();
		float totaltime = c22 - c11;
		cout << "MST time: " << (totaltime / (CLOCKS_PER_SEC)) << " sec\n";
	}
	else
	{
		cout << "Invalid choice" << endl;
	}
	return 0;
}
