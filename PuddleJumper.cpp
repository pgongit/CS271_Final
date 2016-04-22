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


class Node
{
public:
	bool visited;
	double searchDistance;
	string id;
	int index;
	int time;
	double price;
	int distance;
	Node* next;

	Node(string _id, int _time, double _price, int _distance, Node * _next)
	{
		id = _id;
		time = _time;
		price = _price;
		distance = _distance;
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

	Graph()
	{
		for (int i = 0; i < numberOfAirports; ++i)
		{
			Node *n = new Node(airportNameList[i], 0, 0, 0, NULL);
			n->index = i;
			headNodeList[i] = n;
		}
	}

	void AddEdge(string srcId, string destId, int time, double price, int distance);
	Node* GetHeadNode(string id);
	int GetNodeIndex(string id);
};


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

int TimeDifferenceToMinutes(string deptTime, string arivalTime)
{
	int dminutes = 0;
	int aminutes = 0;

	switch (deptTime.length())
	{
	case 2:
		dminutes = stoi(deptTime);
		break;
	case 3:
		dminutes = (stoi(deptTime.substr(0, 1)) * 60) + stoi(deptTime.substr(1, 3));
		break;
	case 4:
		dminutes = (stoi(deptTime.substr(0, 2)) * 60) + stoi(deptTime.substr(2, 4));
		break;
	}

	switch (arivalTime.length())
	{
	case 2:
		aminutes = stoi(arivalTime);
		break;
	case 3:
		aminutes = (stoi(arivalTime.substr(0, 1)) * 60) + stoi(arivalTime.substr(1, 3));
		break;
	case 4:
		aminutes = (stoi(arivalTime.substr(0, 2)) * 60) + stoi(arivalTime.substr(2, 4));
		break;
	}
	return (aminutes - dminutes);
}



int main()
{

	Graph * graph = new Graph();

	ifstream in;
	//in.open("testData.csv");
	in.open("project1.csv");
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

			int timeDifference = TimeDifferenceToMinutes(dep, arriv);

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


	system("pause");
	return -1;
}
