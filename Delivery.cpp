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


// Returns the edge between the adjacent nodes node1 and node2
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


// Test version of main, prints a path of nodes. Comment out if not needed.
//int _tmain(int argc, _TCHAR* argv[]) {
//
//	std::pair<int, int> firstPair(3, 6);
//	Location firstLocation = (Location) firstPair;
//	Node firstNode(firstLocation, 3, NULL);
//
//	std::pair<int, int> secondPair(1, 8);
//	Location secondLocation = (Location) secondPair;
//	Node secondNode(secondLocation, 4, &firstNode);
//
//	std::pair<int, int> thirdPair(7, 8);
//	Location thirdLocation = (Location) thirdPair;
//	Node thirdNode(thirdLocation, 4, &secondNode);
//
//	std::vector<std::pair<int, int>> path = solution(thirdNode);
//	for (unsigned int i = 0; i < path.size(); i++) {
//		std::cout << "Coordinate pair " << i << "\n" << "y: " << path[i].first << "\n" << "x: " << path[i].second << "\n";
//	}
//}



// === BELOW IS CODE THAT WORKS WITHOUT A* ===


std::map<int,std::vector<std::pair<int,int>>> vanInstructions;

std::map<int,int> assignedDeliveries;

std::map<int,Location> cargoDestination;

bool once = true;

bool idle[] = {true,true,true,true,true};

bool returningPackage[] = {false,false,false,false,false};

int dist(Location start, Location end) {
	return ((start.first-end.first)+(start.second-end.second));
}

void sendPathway(int vanNumber, Location start, Location end) {
	std::vector<std::pair<int,int>> path;
	std::pair<int,int> trip;
	trip.first = start.first*2;
	trip.second = start.second;
	path.push_back(trip);
	//path.reserve(1);
	vanInstructions[vanNumber]=path;
}

void spread() {
	std::vector<std::pair<int,int>> path;
	std::pair<int,int> trip;
	for(int vanNo=0;vanNo<4;vanNo++) {
		if(vanNo==0) {
			for(int i=0;i<15;i++) {
				trip.first = 40;
				trip.second = 20+i;
				path.push_back(trip);
			}
		}
		if(vanNo==1) {
			for(int i=0;i<15;i++) {
				trip.first = 40;
				trip.second = 20-(i+1);
				path.push_back(trip);
			}
		}
		if(vanNo==2) {
			for(int i=0;i<15;i++) {
				trip.first = 40-(2*i+1);
				trip.second = 20;
				path.push_back(trip);
			}
		}
		if(vanNo==3) {
			for(int i=0;i<15;i++) {
				trip.first = 40+(2*i+1);
				trip.second = 20;
				path.push_back(trip);
			}
		}
		vanInstructions[vanNo]=path;
		path.clear();
	}
}

int closestVan(Location package, std::vector<VanInfo> vans) {
	int ret = -1;
	int minDistance = 100;
	for(int i=0;i<5;i++) {
		if(idle[i]) {
			if((abs(package.first-vans[i].location.first)+abs(package.second-vans[i].location.second))<minDistance) {
				minDistance = (abs(package.first-vans[i].location.first)+abs(package.second-vans[i].location.second));
				ret = i;
			}
		}
	}
	return ret;
}

void movePackage(int vanNumber, Location package, std::vector<VanInfo> vans, std::vector<std::vector<int>> edges) {
	// Below is code that uses A*

	Problem problem(vans[vanNumber].location, package, edges);
	std::vector<std::pair<int,int>> path = aStar(problem);
	
	// End of code that uses A*


	// Below is code that works, but that doesn't use A* and isn't fast enough

	//std::vector<std::pair<int,int>> path;
	//std::pair<int,int> trip;
	//int vanLocFirst = vans[vanNumber].location.first;
	//int vanLocSecond = vans[vanNumber].location.second;
	//int packageFirst = package.first;
	//int packageSecond = package.second;

	//if(packageFirst-vanLocFirst>0) {
	//	for(int i=0;i<packageFirst-vanLocFirst;i++) {
	//		trip.first = vanLocFirst*2+(2*i+1);
	//		trip.second = vanLocSecond;
	//		path.push_back(trip);
	//	}
	//} else {
	//		for(int i=0;i<-(packageFirst-vanLocFirst);i++) {
	//		trip.first = vanLocFirst*2-(2*i+1);
	//		trip.second = vanLocSecond;
	//		path.push_back(trip);
	//	}
	//}
	//if(packageSecond-vanLocSecond>0) {
	//	for(int i=0;i<packageSecond-vanLocSecond;i++) {
	//		trip.first = packageFirst*2;
	//		trip.second = vanLocSecond+(i);
	//		path.push_back(trip);
	//	}
	//} else {
	//		for(int i=0;i<-(packageSecond-vanLocSecond);i++) {
	//		trip.first = packageFirst*2;
	//		trip.second = vanLocSecond-(i+1);
	//		path.push_back(trip);
	//	}
	//}

	// End of code that doesn't use A*

	
	vanInstructions[vanNumber]=path;
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL)); // Added for randomness
	std::wcout << L"Hello and welcome!\n";

	// Create client
	std::wstring name = L"Sverrir, Sander, Martin och Malin";
	bool OK;
	DM_Client client(name, OK);

	// Start game and print information
	std::vector<std::vector<std::wstring>> nodes;
	std::wstring startOutput;
	client.startGame(nodes, startOutput);

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

	for(int deliveryID=0;deliveryID<min(int(waitingDeliveries.size()),5);deliveryID++) {
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

	while (time < 1440 && compDeliveries < 20) {
			waitingDeliveries.clear();
			activeDeliveries.clear();
			completedDeliveries.clear();
			vans.clear();
			edges.clear();
			client.getInformation(time, edges, vans, waitingDeliveries, activeDeliveries, completedDeliveries, informationOutput);
			std::wcout << L"Time: " << time << "\n";

			//std::wcout << L"Van 0 location: (" << vans[0].location.first << ", " << vans[0].location.second << L"), cargo: " << vans[0].cargo;
			//if (vans[0].instructions.size() > 0) {
			//	std::wcout << ", instructions: (" << vans[0].instructions[0].first << ", " << vans[0].instructions[0].second << L") \n";
			//}
			//std::wcout << L"Van 1 location: (" << vans[1].location.first << ", " << vans[1].location.second << L"), cargo: " << vans[1].cargo;
			//if (vans[1].instructions.size() > 0) {
			//	std::wcout << ", instructions: (" << vans[1].instructions[0].first << ", " << vans[1].instructions[0].second << L") \n";
			//}
			//std::wcout << L"Van 2 location: (" << vans[2].location.first << ", " << vans[2].location.second << L"), cargo: " << vans[2].cargo;
			//if (vans[2].instructions.size() > 0) {
			//	std::wcout << ", instructions: (" << vans[2].instructions[0].first << ", " << vans[2].instructions[0].second << L") \n";
			//}
			//std::wcout << L"Van 3 location: (" << vans[3].location.first << ", " << vans[3].location.second << L"), cargo: " << vans[3].cargo;
			//if (vans[3].instructions.size() > 0) {
			//	std::wcout << ", instructions: (" << vans[3].instructions[0].first << ", " << vans[3].instructions[0].second << L") \n";
			//}
			//std::wcout << L"Van 4 location: (" << vans[4].location.first << ", " << vans[4].location.second << L"), cargo: " << vans[4].cargo;
			//if (vans[4].instructions.size() > 0) {
			//	std::wcout << ", instructions: (" << vans[4].instructions[0].first << ", " << vans[4].instructions[0].second << L") \n";
			//}

			std::wcout << L"Packages on board: " << waitingDeliveries.size() + activeDeliveries.size() << L"\n";
			std::wcout << L"Completed deliveries: " << completedDeliveries.size() << L"\n\n";

			//for(int i=0;i<waitingDeliveries.size();i++) {
			//	std::wcout << L"Package " << waitingDeliveries[i].Number << " location: " << waitingDeliveries[i].pickUp.first << ", " << waitingDeliveries[i].pickUp.second << L"\n";
			//}

			if(once) {
				spread();
				client.sendInstructions(vanInstructions,uselessString);
				once = false;
			}

			if(waitingDeliveries.empty() == false) {
				if(waitingDeliveries.back().Number>=int(cargoDestination.size())) {
					cargoDestination[waitingDeliveries.back().Number]=waitingDeliveries.back().dropOff;
				}
			}

			for(int vanNumber=0;vanNumber<5;vanNumber++) { //checks if vans are now free
				if((vans[vanNumber].cargo==-1) && (returningPackage[vanNumber]==true)) {
					vans[vanNumber].instructions.clear(); //possibly remove
					assignedDeliveries[vanNumber]=-1;
					idle[vanNumber]=true;
					returningPackage[vanNumber]=false;
				}
			}

			for(int vanNumber=0;vanNumber<5;vanNumber++) { //prevents van from going to its destination if its delivery has already been picked up. Does not
				//seem to make a difference as is.
				if((vans[vanNumber].cargo==-1) && (assignedDeliveries[vanNumber]!=-1)) {
					bool flag = false;
					for(int i=0;i<int(waitingDeliveries.size());i++) {
						if(waitingDeliveries[i].Number==assignedDeliveries[vanNumber]) {
							flag = true;
							break;
						}
					}
					if(flag==false) {
						vans[vanNumber].instructions.clear(); //possibly remove
						assignedDeliveries[vanNumber]=-1;
						idle[vanNumber]=true;
						returningPackage[vanNumber]=false;
					}
				}
			}

			for(int i=0;i<int(waitingDeliveries.size());i++) {
				bool flag = true;

				for(int vanInfo=0;vanInfo<5;vanInfo++) {

					if(assignedDeliveries[vanInfo]==waitingDeliveries[i].Number) {
						flag = false;
						break;
					}
				}
				if(flag) {
					int whichVan = closestVan(waitingDeliveries[i].pickUp,vans);
					if(whichVan!=-1) {
						movePackage(whichVan,waitingDeliveries[i].pickUp,vans,edges);
						client.sendInstructions(vanInstructions,uselessString);
						vanInstructions.clear();
						idle[whichVan]=false;
						assignedDeliveries[whichVan]=waitingDeliveries[i].Number;
						flag=false;
					}
				}
			}

			for(int vanNumber=0;vanNumber<5;vanNumber++) {
				if(returningPackage[vanNumber]==false && (vans[vanNumber].cargo!=-1)) {
					vans[vanNumber].instructions.clear();
					movePackage(vanNumber,cargoDestination[vans[vanNumber].cargo],vans,edges);
					client.sendInstructions(vanInstructions,uselessString);
					vanInstructions.clear(); //possibly remove
					returningPackage[vanNumber]=true;
				}
			}

			//for(int i=0;i<cargoDestination.size();i++) {
			//	std::wcout << L"Cargo " << i << L" destination = (" << cargoDestination[i].first << ", " <<
			//		cargoDestination[i].second << L")" << L"\n";
			//}

			//for(int i=0;i<waitingDeliveries.size();i++) {
			//	std::wcout << L"Waiting Delivery: " << waitingDeliveries[i].Number << L" dropOff = (" 
			//		<< waitingDeliveries[i].dropOff.first << ", " << waitingDeliveries[i].dropOff.second << ")" << L"\n";
			//}

			waitingDeliveries.clear();
			activeDeliveries.clear();
			compDeliveries = completedDeliveries.size();
			completedDeliveries.clear();
			edges.clear();
			vans.clear();
		}
	return 0;
}