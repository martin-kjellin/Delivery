#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <list>
#include <queue>
#include <set>
#include <utility>
#include <time.h> // Added for randomness
#include <stdlib.h> // Added for randomness
#include "DeliveryManClient.h"
#pragma comment (lib, "DeliveryManClient")


std::map<int,std::vector<std::pair<int,int>>> vanInstructions;  //Variable that keeps track of instructions that should be sent to vans. Cleaned regularly.

std::map<int,int> assignedDeliveries;  //Maps van numbers to the delivery ID of their cargo.

std::map<int,Location> cargoDestination;  //Maps cargo ID to the destination of the cargo.

bool once = true; //Temp variable to make sure the vans only spread out once.

bool idle[] = {true,true,true,true,true};  //Keeps track of the vans are idle.

bool returningPackage[] = {false,false,false,false,false}; //Keeps track of if the vans are returning a package.


Location nodesToEdge(Location node1, Location node2) {
	int y1 = node1.first;
	int x1 = node1.second;
	int y2 = node2.first;
	int x2 = node2.second;
	Location edge;
	if (y1 == y2) { // East--west edge
		edge.first = 2 * y1;
		edge.second = min(x1, x2);
	} else if (x1 == x2){ // North--south edge
		edge.first = y1 + y2;
		edge.second = x1;
	}
	return edge;
}


class Problem {
public:
	Problem(Location start, Location goal, std::vector<std::vector<int>> edges) {
		s = start;
		g = goal;
		e = edges;
	}
	Location getStart() {
		return s;
	}
	Location getGoal() {
		return g;
	}
	std::vector<std::vector<int>> getEdges() {
		return e;
	}
	int getEdgeCost(Location node, Location adjacentNode) { // Only to be used for adjacent nodes
		Location edge = nodesToEdge(node, adjacentNode);
		int yCoordinate = edge.first;
		int xCoordinate = edge.second;
		int edgeCost = e[yCoordinate][xCoordinate];
		return edgeCost;
	}
	// Estimate the path cost to be the average of two of the possible paths
	int getEstimatedCostToGoal(Location from) {
		int estimatedPathCost;
		if (from.first <= g.first && from.second <= g.second) { // from node is northwest of goal node
			// Cost of going first east, then south to reach the goal
			int eastSouthCost = 0;
			for (int x = from.second; x < g.second; x++) {
				Location edge = nodesToEdge(Location(from.first, x), Location(from.first, x + 1));
				eastSouthCost += e[edge.first][edge.second];
			}
			for (int y = from.first; y < g.first; y++) {
				Location edge = nodesToEdge(Location(y, g.second), Location(y + 1, g.second));
				eastSouthCost += e[edge.first][edge.second];
			}
			// Cost of going first south, then east to reach the goal
			int southEastCost = 0;
			for (int y = from.first; y < g.first; y++) {
				Location edge = nodesToEdge(Location(y, from.second), Location(y + 1, from.second));
				southEastCost += e[edge.first][edge.second];
			}
			for (int x = from.second; x < g.second; x++) {
				Location edge = nodesToEdge(Location(g.first, x), Location(g.first, x + 1));
				southEastCost += e[edge.first][edge.second];
			}
			estimatedPathCost = (eastSouthCost + southEastCost) / 2;
		}
		if (from.first >= g.first && from.second <= g.second) { // from node is southwest of goal node
			// Cost of going first east, then north to reach the goal
			int eastNorthCost = 0;
			for (int x = from.second; x < g.second; x++) {
				Location edge = nodesToEdge(Location(from.first, x), Location(from.first, x + 1));
				eastNorthCost += e[edge.first][edge.second];
			}
			for (int y = from.first; y > g.first; y--) {
				Location edge = nodesToEdge(Location(y, g.second), Location(y - 1, g.second));
				eastNorthCost += e[edge.first][edge.second];
			}
			// Cost of going first north, then east to reach the goal
			int northEastCost = 0;
			for (int y = from.first; y > g.first; y--) {
				Location edge = nodesToEdge(Location(y, from.second), Location(y - 1, from.second));
				northEastCost += e[edge.first][edge.second];
			}
			for (int x = from.second; x < g.second; x++) {
				Location edge = nodesToEdge(Location(g.first, x), Location(g.first, x + 1));
				northEastCost += e[edge.first][edge.second];
			}
			estimatedPathCost = (eastNorthCost + northEastCost) / 2;
		}
		if (from.first >= g.first && from.second >= g.second) { // from node is southeast of goal node
			// Cost of going first west, then north to reach the goal
			int westNorthCost = 0;
			for (int x = from.second; x > g.second; x--) {
				Location edge = nodesToEdge(Location(from.first, x), Location(from.first, x - 1));
				westNorthCost += e[edge.first][edge.second];
			}
			for (int y = from.first; y > g.first; y--) {
				Location edge = nodesToEdge(Location(y, g.second), Location(y - 1, g.second));
				westNorthCost += e[edge.first][edge.second];
			}
			// Cost of going first north, then west to reach the goal
			int northWestCost = 0;
			for (int y = from.first; y > g.first; y--) {
				Location edge = nodesToEdge(Location(y, from.second), Location(y - 1, from.second));
				northWestCost += e[edge.first][edge.second];
			}
			for (int x = from.second; x > g.second; x--) {
				Location edge = nodesToEdge(Location(g.first, x), Location(g.first, x - 1));
				northWestCost += e[edge.first][edge.second];
			}
			estimatedPathCost = (westNorthCost + northWestCost) / 2;
		}
		if (from.first <= g.first && from.second >= g.second) { // from node is northeast of goal node
			// Cost of going first west, then south to reach the goal
			int westSouthCost = 0;
			for (int x = from.second; x > g.second; x--) {
				Location edge = nodesToEdge(Location(from.first, x), Location(from.first, x - 1));
				westSouthCost += e[edge.first][edge.second];
			}
			for (int y = from.first; y < g.first; y++) {
				Location edge = nodesToEdge(Location(y, g.second), Location(y + 1, g.second));
				westSouthCost += e[edge.first][edge.second];
			}
			// Cost of going first south, then west to reach the goal
			int southWestCost = 0;
			for (int y = from.first; y < g.first; y++) {
				Location edge = nodesToEdge(Location(y, from.second), Location(y + 1, from.second));
				southWestCost += e[edge.first][edge.second];
			}
			for (int x = from.second; x > g.second; x--) {
				Location edge = nodesToEdge(Location(g.first, x), Location(g.first, x - 1));
				southWestCost += e[edge.first][edge.second];
			}
			estimatedPathCost = (westSouthCost + southWestCost) / 2;
		}
		// Older solution, where the path cost was estimated to be equal to the number of edges
		//int yDistance = abs(g.first - from.first);
		//int xDistance = abs(g.second - from.second);
		//int estimatedPathCost = yDistance + xDistance;
		return estimatedPathCost;
	}
private:
	Location s;
	Location g;
	std::vector<std::vector<int>> e;
};


class Node {
public:
	Node() {
		std::pair<int, int> startPair;
		s = (Location) startPair;
		tc = 0;
		pc = 0;
		p = NULL;
	}
	Node(Location state, int pathCostToNode, int pathCost, Node *parent) {
		s = state;
		tc = pathCostToNode;
		pc = pathCost;
		p = parent;
	}
	bool operator() (const Node& n1, const Node& n2) const {
		return n1.pc < n2.pc;
	}
	Location getState() {
		return s;
	}
	int getPathCostToNode() {
		return tc;
	}
	int getPathCost() {
		return pc;
	}
	Node *getParent() {
		return p;
	}
private:
	Location s;
	int tc;
	int pc;
	Node *p;
};


// Help function to aStar, returns the path to node
std::vector<std::pair<int, int>> solution(Node *node) {
	std::vector<std::pair<int, int>> solutionPath;
	while (node != NULL) {
		std::pair<int, int> currentLocation = (*node).getState();
		solutionPath.insert(solutionPath.begin(), currentLocation);
		Node *oldNode = node;
		node = (*node).getParent();
		delete oldNode;
	}
	return solutionPath;
}


// Help function to aStar, inserts a node into the frontier if the
// conditions of A* are fulfilled
void insertNodeIntoFrontier(Problem& problem, Node *previousNode, Location& insertLocation, std::multiset<Node, Node>& frontier, std::set<Location>& explored) {
	int pathCostToNode = (*previousNode).getPathCostToNode() + problem.getEdgeCost((*previousNode).getState(), insertLocation);
	int pathCost = pathCostToNode + problem.getEstimatedCostToGoal(insertLocation);
	Node insertNode(insertLocation, pathCostToNode, pathCost, previousNode);
	bool frontierHasInsertNode = false;
	for (std::multiset<Node, Node>::iterator it = frontier.begin(); it != frontier.end(); it++) {
		Node currentNode = *it;
		if (currentNode.getState() == insertNode.getState()) {
			frontierHasInsertNode = true;
		}
	}
	if (!frontierHasInsertNode) {
		if (!explored.count(insertNode.getState())) {
			frontier.insert(insertNode);
		}
	} else {
		std::multiset<Node, Node>::iterator findIterator = frontier.find(insertNode);
		if (findIterator != frontier.end()) {
			Node oldInsertNode = *findIterator;
			if (oldInsertNode.getPathCost() > insertNode.getPathCost()) {
				frontier.erase(findIterator);
				frontier.insert(insertNode);
			}
		}
	}
}


// The A* algorithm
std::vector<std::pair<int, int>> aStar(Problem problem) {
	Location startLocation = problem.getStart();
	Node startNode(problem.getStart(), 0, problem.getEstimatedCostToGoal(startLocation), NULL);
	// The frontier (works as both a priority queue and a set)
	std::multiset<Node, Node> frontier;
	frontier.insert(startNode);
	// The explored set
	std::set<Location> explored;
	std::vector<std::pair<int, int>> returnValue;
	while (true) {
		if (frontier.empty()) { // No solution to the problem was found (this should never happen)
			break;
		} else {
			// Get the lowest-cost node of the frontier
			std::multiset<Node, Node>::iterator lowestIterator = frontier.begin();
			Node *node = new Node;
			*node = *lowestIterator;
			frontier.erase(lowestIterator);
			if ((*node).getState() == problem.getGoal()) { // The goal is reached
				std::vector<std::pair<int, int>> solutionNodes = solution(node);
				for (std::vector<std::pair<int, int>>::iterator it = solutionNodes.begin(); (it + 1) != solutionNodes.end(); it++) {
					Location edge = nodesToEdge(*it, *(it + 1));
					returnValue.push_back(edge);
				}
				break;
			} else {
				Location location = (*node).getState();
				explored.insert(location);
				int problemSize = 41;
				int yCoordinate = location.first;
				int xCoordinate = location.second;
				if (yCoordinate > 0) { // Possible to move to the north
					Location northLocation(yCoordinate - 1, xCoordinate);
					insertNodeIntoFrontier(problem, node, northLocation, frontier, explored);
				}
				if (yCoordinate < problemSize - 1) { // Possible to move to the south
					Location southLocation(yCoordinate + 1, xCoordinate);
					insertNodeIntoFrontier(problem, node, southLocation, frontier, explored);
				}
				if (xCoordinate > 0) { // Possible to move to the west
					Location westLocation(yCoordinate, xCoordinate - 1);
					insertNodeIntoFrontier(problem, node, westLocation, frontier, explored);
				}
				if (xCoordinate < problemSize - 1) { // Possible to move to the east
					Location eastLocation(yCoordinate, xCoordinate + 1);
					insertNodeIntoFrontier(problem, node, eastLocation, frontier, explored);
				}
			}
		}
	}
	return returnValue;
}


int dist(Location start, Location end) {  //Calculates Manhattan distance between two locations.
	return ((start.first-end.first)+(start.second-end.second));
}


void spread() { //Spreads the vans close to the pickup locations of the first few deliveries.
	std::vector<std::pair<int,int>> path;
	std::pair<int,int> trip;
	for(int vanNo=0;vanNo<5;vanNo++) {
		if(vanNo==0) {
			for(int i=0;i<10;i++) {
				trip.first = 40+(2*i)+1;
				trip.second = 20;
				path.push_back(trip);
			}
			for(int i=0;i<16;i++) {
				trip.first=30*2;
				trip.second=20-(i+1);
				path.push_back(trip);
			}
		}

		if(vanNo==1) {
			for(int i=0;i<20;i++) {
				trip.first = 40;
				trip.second = 20+i;
				path.push_back(trip);
			}
			for(int i=0;i<13;i++) {
				trip.first=40-(2*i+1);
				trip.second=40;
				path.push_back(trip);
			}
			trip.first=14;
			trip.second=41;
			path.push_back(trip);
		}
		if(vanNo==2) {
			for(int i=0;i<15;i++) {
				trip.first = 40+(2*i)+1;
				trip.second = 20;
				path.push_back(trip);
			}
			for(int i=0;i<10;i++) {
				trip.first=35*2;
				trip.second=20-(i+1);
				path.push_back(trip);
			}
		}
		if(vanNo==3) {
			for(int i=0;i<2;i++) {
				trip.first = 40;
				trip.second = 20+i;
				path.push_back(trip);
			}
			for(int i=0;i<1;i++) {
				trip.first = 40-(2*i)-1;
				trip.second = 22;
				path.push_back(trip);
			}
		}
		if(vanNo==4) {
			for(int i=0;i<9;i++) {
				trip.first = 40;
				trip.second = 20+i;
				path.push_back(trip);
			}
		}
		vanInstructions[vanNo]=path;
		path.clear();
	}
}


int closestVan(Location package, std::vector<VanInfo> vans, int tid) { //Gives the ID-number of the closest van to a given package, paying not attention to traffic conditions.
	int ret = -1;
	int minDistance = 100;
	for(int i=0;i<5;i++) {
		if(idle[i]) {
			int tempDistance = (abs(package.first-vans[i].location.first)+abs(package.second-vans[i].location.second));
			if(tempDistance<minDistance) {
				minDistance = tempDistance;
				ret = i;
			}
		}
	}
	if(minDistance>35 && tid<1100) {return -1; } else
		return ret;
}


void movePackage(int vanNumber, Location package, std::vector<VanInfo> vans, std::vector<std::vector<int>> edges) {

	Problem problem(vans[vanNumber].location, package, edges);
	vanInstructions[vanNumber] = aStar(problem);
}


int mainMethod(int argc, _TCHAR* argv[]) {
	std::wcout << L"Hello and welcome!\n";

	// Create client
	std::wstring name = L"Sverrir, Sander, Martin och Malin";
	bool OK;
	DM_Client client(name, OK);

	// Start game and print information
	std::vector<std::vector<std::wstring>> nodes;
	std::wstring startOutput;
	client.startGame(nodes, startOutput);
	std::wcout << L"Output: " << startOutput << L"\n\n";

	// Get information about the environment state
	int time;
	std::vector<std::vector<int>> edges;
	std::vector<VanInfo> vans;
	std::vector<DeliveryInfo> waitingDeliveries;
	std::vector<DeliveryInfo> activeDeliveries;
	std::vector<std::pair<int,int>> completedDeliveries;
	std::wstring informationOutput;
	std::wstring uselessString;

	client.getInformation(time, edges, vans, waitingDeliveries, activeDeliveries, completedDeliveries, informationOutput);

	for(int deliveryID=0;deliveryID<min(waitingDeliveries.size(),5);deliveryID++) {
		int lowestDistance = 100;
		int bestVanID = 0;
		for(int vanID=0;vanID<5;vanID++) {
			int thisDistance = dist(waitingDeliveries[deliveryID].pickUp,vans[vanID].location);
			if (thisDistance<lowestDistance) {
				lowestDistance = thisDistance;
				bestVanID = vanID;
			}
		}
	}

	assignedDeliveries[0]=-1;
	assignedDeliveries[1]=-1;
	assignedDeliveries[2]=-1;
	assignedDeliveries[3]=-1;
	assignedDeliveries[4]=-1;

	int compDeliveries = 0;

	while(time<1440 && compDeliveries<20) { //The main loop, which runs continuously until the time runs out.
		waitingDeliveries.clear();
		activeDeliveries.clear();
		completedDeliveries.clear();
		vans.clear();
		edges.clear();
		client.getInformation(time, edges, vans, waitingDeliveries, activeDeliveries, completedDeliveries, informationOutput);
		std::wcout << "\n" << "\n";
		std::wcout << L"Time: " << time << "\n";

		//Only necessary for testing purposes:

		std::wcout << L"Van 0 location: (" << vans[0].location.first << ", " << vans[0].location.second << L"), cargo: " << vans[0].cargo << L"\n";
		std::wcout << L"Van 1 location: (" << vans[1].location.first << ", " << vans[1].location.second << L"), cargo: " << vans[1].cargo << L"\n";
		std::wcout << L"Van 2 location: (" << vans[2].location.first << ", " << vans[2].location.second << L"), cargo: " << vans[2].cargo << L"\n";
		std::wcout << L"Van 3 location: (" << vans[3].location.first << ", " << vans[3].location.second << L"), cargo: " << vans[3].cargo << L"\n";
		std::wcout << L"Van 4 location: (" << vans[4].location.first << ", " << vans[4].location.second << L"), cargo: " << vans[4].cargo << L"\n";

		std::wcout << L"Deliveries on grid: " << waitingDeliveries.size() + activeDeliveries.size() << L"\n";
		std::wcout << L"Completed deliveries: " << completedDeliveries.size() << L"\n";

		if(once) {
			spread();
			client.sendInstructions(vanInstructions,uselessString);
			once = false;
		}

		if(waitingDeliveries.empty() == false) {
			if(waitingDeliveries.back().Number>=cargoDestination.size()) {
				cargoDestination[waitingDeliveries.back().Number]=waitingDeliveries.back().dropOff;
			}
		}

		for(int vanNumber=0;vanNumber<5;vanNumber++) { //Checks if vans that were previously on a delivery are now free.
			if((vans[vanNumber].cargo==-1) && returningPackage[vanNumber]) {
				vans[vanNumber].instructions.clear();
				assignedDeliveries[vanNumber]=-1;
				idle[vanNumber]=true;
				returningPackage[vanNumber]=false;
			}
		}

		for(int vanNumber=0;vanNumber<5;vanNumber++) { //Prevents van from going to its destination if its delivery has already been picked up.
			if((vans[vanNumber].cargo==-1) && (assignedDeliveries[vanNumber]!=-1)) {
				bool flag = false;
				for(int i=0;i<waitingDeliveries.size();i++) {
					if(waitingDeliveries[i].Number==assignedDeliveries[vanNumber]) {
						flag = true;
						break;
					}
				}
				if(flag==false) {
					vans[vanNumber].instructions.clear();
					assignedDeliveries[vanNumber]=-1;
					idle[vanNumber]=true;
					returningPackage[vanNumber]=false;
				}
			}
		}

		for(int i=0;i<waitingDeliveries.size();i++) { //Sends those vans that have picked up deliveries on a mission to return their deliveries.
			bool flag = true;

			for(int vanInfo=0;vanInfo<5;vanInfo++) {

				if(assignedDeliveries[vanInfo]==waitingDeliveries[i].Number) {
					flag = false;
					break;
				}
			}
			if(flag) {
				int whichVan = closestVan(waitingDeliveries[i].pickUp,vans,time);
				if(whichVan!=-1) {
					vanInstructions.clear();
					movePackage(whichVan,waitingDeliveries[i].pickUp,vans,edges);
					client.sendInstructions(vanInstructions,uselessString);
					vanInstructions.clear();
					idle[whichVan]=false;
					assignedDeliveries[whichVan]=waitingDeliveries[i].Number;
				}
			}
		}

		for(int vanNumber=0;vanNumber<5;vanNumber++) {  //Sends free vans on a mission to collect deliveries.
			if(returningPackage[vanNumber]==false && (vans[vanNumber].cargo!=-1)) {
				vans[vanNumber].instructions.clear();
				movePackage(vanNumber,cargoDestination[vans[vanNumber].cargo],vans,edges);
				client.sendInstructions(vanInstructions,uselessString);
				vanInstructions.clear();
				returningPackage[vanNumber]=true;
			}
		}

		compDeliveries = completedDeliveries.size();

		waitingDeliveries.clear();
		activeDeliveries.clear();
		completedDeliveries.clear();
		edges.clear();
		vans.clear();
	}

	vanInstructions.clear();
	assignedDeliveries.clear();
	cargoDestination.clear();
	once = true;
	bool idle[] = {true,true,true,true,true};
	bool returningPackage[] = {false,false,false,false,false};

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	while(true) {
		mainMethod(argc, argv);
	}
}