#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string>
#include <vector> 
#include <fstream>
#include <queue>
#include <iostream>

using namespace std;

class Weight
{
public:
	double price;
	int time;
	int distance;
};

class Node
{
public:
	bool visited;
	double searchDistance;
	string id;
	int index;
	Weight w;
	Node* next;

	Node(string _id, int _time, double _price, int _distance, Node * _next)
	{
		id = _id;
		w.time = _time;
		w.price = _price;
		w.distance = _distance;
		next = _next;
		visited = false;
		searchDistance = DBL_MAX;
	}
};

class Graph
{
public:
	string airportNameList[18] = {
		"ATL","BOS","BWI","DCA","DEN","DFW","HSV","IAD","IAH","IND",
		"JAN","JFK","LAX","MIA","ORD","SEA","SFO","TPA" };

	enum  search { time, price, distance };
	int numberOfAirports = 18;
	Node* headNodeList[18];
	queue <Node*> searchQueue;
	Weight weightMatrix[18][18];

	Graph()
	{
		for (int i = 0; i < numberOfAirports; ++i)
		{
			Node *n = new Node(airportNameList[i], 0, 0, 0, NULL);
			n->index = i;
			headNodeList[i] = n;

			for (int j = 0; j < numberOfAirports; j++)
			{
				Weight w;
				w.distance = INT_MAX;
				w.price = DBL_MAX;
				w.time = INT_MAX;
				weightMatrix[i][j] = w;
			}
		}
	}

	void AddEdge(string srcId, string destId, int time, double price, int distance);
	Node* GetHeadNode(string id);
	int GetNodeIndex(string id);
	string SearchDirectFlights(string from, string to);
};



string Graph::SearchDirectFlights(string from, string to)
{
	int distance = INT_MAX;
	double price = DBL_MAX;
	int time = INT_MAX;
	Node * startNode = GetHeadNode(from);


	Node* node = startNode;
	while (node != NULL)
	{
		if (node->id == to)
		{
			if (node->w.distance < distance)
			{
				distance = node->w.distance;
			}
			if (node->w.time < time)
			{
				time = node->w.time;
			}
			if (node->w.price < price)
			{
				price = node->w.price;
			}
		}
		node = node->next;
	}

	string res = "";

	if (distance < INT_MAX)
	{
		res += "\nShortest Distance: ";
		res += to_string(distance) + " miles";
	}

	if (price < DBL_MAX)
	{
		res += "\nLowest Price: ";
		res += "$" + to_string(price);
	}

	if (time < INT_MAX)
	{
		res += "\nShortest Time: ";
		res += to_string(time) + " min";
	}
	return res == "" ? "NOT - FOUND" : res;
}

Node* Graph::GetHeadNode(string id)
{
	Node * node = NULL;

	for (int i = 0; i < numberOfAirports; i++)
	{
		if (headNodeList[i]->id == id)
		{
			node = headNodeList[i];
			break;
		}
	}

	return node;
}

int Graph::GetNodeIndex(string id)
{
	int index = -1;
	for (int i = 0; i < numberOfAirports; i++)
	{
		if (headNodeList[i]->id == id)
		{
			index = i;
			break;
		}
	}

	return index;
}

void Graph::AddEdge(string srcId, string destId, int time, double price, int distance)
{
	Node *head = NULL;

	for (int i = 0; i < numberOfAirports; i++)
	{
		if (srcId == headNodeList[i]->id)
		{
			head = headNodeList[i];
			break;
		}
	}

	int index = head->index;
	Node * temp = head;
	while (1)
	{
		if (temp->next == NULL)
		{
			Node * newNode = new Node(destId, time, price, distance, NULL);
			newNode->index = GetNodeIndex(destId);
			temp->next = newNode;
			return;
		}
		temp = temp->next;
	}
};


int TimeDifferenceToMinutes(int deptTime, int arivalTime)
{
	int dh = deptTime / 100;
	int dm = deptTime % 100;
	int dminutes = dh * 60 + dm;

	int ah = arivalTime / 100;
	int am = arivalTime % 100;
	int aminutes = ah * 60 + am;

	if (dminutes > aminutes)
	{
		return (dminutes - aminutes);
	}
	else
	{
		return (aminutes - dminutes);
	}
}

int main()
{
	Graph * graph = new Graph();

	ifstream in;
	in.open("testData.csv");
	//in.open("project1.csv");
	string flights;
	if (in.is_open() == true)
	{
		while (in.peek() != EOF)
		{
			in >> flights;
			int pos = 0;
			int count = 0;
			string src, dest, dep, arriv, price, dist = "";
			int delimiterPos[5];
			for (int i = 0; i < flights.length(); i++)
			{
				if (flights[i] == ',')
				{
					delimiterPos[count++] = pos;
				}
				pos++;
			}

			src = flights.substr(0, delimiterPos[0]);
			dest = flights.substr(delimiterPos[0] + 1, (delimiterPos[1] - delimiterPos[0] - 1));
			dep = flights.substr(delimiterPos[1] + 1, (delimiterPos[2] - delimiterPos[1] - 1));
			arriv = flights.substr(delimiterPos[2] + 1, (delimiterPos[3] - delimiterPos[2] - 1));
			dist = flights.substr(delimiterPos[3] + 1, (delimiterPos[4] - delimiterPos[3] - 1));
			price = flights.substr(delimiterPos[4] + 1, (delimiterPos[5] - delimiterPos[4] - 1));

			int timeDifference = TimeDifferenceToMinutes(stoi(dep), stoi(arriv));

			graph->AddEdge(src, dest, timeDifference, stod(price), stoi(dist));

		}
	}

	in.clear();
	in.close();

	string from;
	string to;

	cout << "Available Destinations: ";
	for (int i = 0; i < graph->numberOfAirports; i++)
	{
		cout << graph->airportNameList[i] << ", ";
	}

	cout << endl;
	cout << endl;
	cout << "Enter a departure airport: ";
	cin >> from;
	cout << "Enter a destination airport: ";
	cin >> to;

	cout << endl;
	cout << endl;
	cout << "Direct Flight Between " << from << " and " << to << endl;
	cout << graph->SearchDirectFlights(from, to) << endl;
		

	system("pause");
	return -1;
}

