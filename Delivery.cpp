#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <list>
#include <queue>
#include <set>
#include "DeliveryManClient.h"
#pragma comment (lib, "DeliveryManClient")


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
		pc = 0;
		p = NULL;
	}
	
	Node(Location state, int pathCost, Node *parent) {
		s = state;
		pc = pathCost;
		p = parent;
	}
	
	bool operator() (const Node& n1, const Node& n2) {
		return n1.pc < n2.pc;
	}
	
	Location getState() {
		return s;
	}
	
	int getPathCost() {
		return pc;
	}

	Node *getParent() {
		return p;
	}

private:
	Location s;
	int pc;
	Node *p;
};


std::vector<std::pair<int, int>> solution(Node node) {
	std::vector<std::pair<int, int>> solutionPath;
	Node *currentNode = &node;
	while (currentNode != NULL) {
		std::pair<int, int> currentLocation = (*currentNode).getState();
		solutionPath.insert(solutionPath.begin(), currentLocation);
		currentNode = (*currentNode).getParent();
	}
	return solutionPath;
}


//returns the edge between node1 and node2
Location nodesToEdge(Location node1, Location node2){
    int y1 = node1.first;
    int x1 = node1.second;
    int y2 = node2.first;
    int x2 = node2.second;

    Location edge;

    if(y1==y2){
        edge.first = 2*y1; 
        edge.second = std::min(x1,x2);
    }
    else if(x1==x2){
        edge.first = y1+y2;
        edge.second = x1;
    }
    else{
        //nodes not adjecent
    }
    return edge;
}


//makes a path of nodes into a path of edges
std::vector<Location> nodeToEdgePath(std::vector<Location> nodes){
    int length = nodes.size() - 1;
    std::vector<Location> edges;
    for(int i = 0; i < length; i++){
        edges[i] = nodesToEdge(nodes[i],nodes[i+1]); 
    }
    return edges;
}




//std::vector<std::pair<int, int>> aStar(Problem problem) {
//	Node startNode(problem.getStart(), 0, NULL);
//
//	std::set<Node> frontier;
//	frontier.insert(startNode);
//
//	std::set<Location> explored;
//
//	while(true) {
//		if (frontier.empty()) {
//			std::vector<std::pair<int,int>> emptySolution;
//			return emptySolution;
//		}
//		
//		Node node = frontier.top();
//		frontier.pop();
//
//		if (node.getState() == problem.getGoal()) {
//			return solution(node);
//		}
//
//		explored.insert(node.getState());
//
//		Location state = node.getState();
//		int yCoordinate = state.first;
//		int xCoordinate = state.second;
//		if (yCoordinate != 0) {
//			Location northLocation(yCoordinate - 1, xCoordinate);
//			if (!explored.count(northLocation) && !frontier.count(northLocation)) {
//				// DO SOMETHING
//
//			}
//		}
//		
//		// Find children of node
//		// Iterate over them
//			// If the child is not in explored or frontier, insert the child into the frontier
//			// Else if the child is in frontier with higher path cost, replace that node with the child
//	}
//}


// Martin's test version of main. Comment out if not needed.
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



std::map<int,std::vector<std::pair<int,int>>> vanInstructions;
bool onceXSSCleaned= true;
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
            for(int i=0;i<10;i++) {
                trip.first = 40;
                trip.second = 20+i;
                path.push_back(trip);
            }
        }
        if(vanNo==1) {
            for(int i=0;i<10;i++) {
                trip.first = 40;
                trip.second = 20-(i+1);
                path.push_back(trip);
            }
        }
        if(vanNo==2) {
            for(int i=0;i<10;i++) {
                trip.first = 40-(2*i+1);
                trip.second = 20;
                path.push_back(trip);
            }
        }
        if(vanNo==3) {
            for(int i=0;i<10;i++) {
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
        if(!vans[i].instructions.empty()) {   //check later
            //A*
            if((package.first-vans[i].location.first)+(package.second-vans[i].location.second)<minDistance) {
                minDistance = package.first-vans[i].location.first+package.second-vans[i].location.second;
                ret = i;
            }
        }
    }
    return ret;
}
void pickUpPackage(int vanNumber, Location package, std::vector<VanInfo> vans) {
    std::vector<std::pair<int,int>> path;
    std::pair<int,int> trip;
    int vanLocFirst = vans[vanNumber].location.first;
    int vanLocSecond = vans[vanNumber].location.second;
    int packageFirst = package.first;
    int packageSecond = package.second;
    if(packageSecond-vanLocSecond>0) {
        for(int i=0;i<packageSecond-vanLocSecond;i++) {
            trip.first = vanLocFirst*2+i;
            trip.second = vanLocSecond;
            path.push_back(trip);
        }
    } else {
            for(int i=0;i<-(packageSecond-vanLocSecond);i++) {
            trip.first = vanLocFirst*2-i;
            trip.second = vanLocSecond;
            path.push_back(trip);
        }
    }
    if(packageFirst-vanLocFirst>0) {
        for(int i=0;i<packageFirst-vanLocFirst;i++) {
            trip.first = packageFirst;
            trip.second = vanLocSecond;
            path.push_back(trip);
        }
    } else {
            for(int i=0;i<-(packageFirst-vanLocFirst);i++) {
            trip.first = packageFirst;
            trip.second = vanLocSecond;
            path.push_back(trip);
        }
    }
}
 
int _other_tmain(int argc, _TCHAR* argv[])
{
    std::wcout << L"Hello and welcome!\n";
    // Create client
    std::wstring name = L"This will be our group name";
    bool OK;
    DM_Client client(name, OK);
    // Start game and print information
    std::vector<std::vector<std::wstring>> nodes;
    std::wstring startOutput;
    client.startGame(nodes, startOutput);
    std::wcout << L"Node type at (10,10): " << nodes[40][40] << L"\n";
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
    while(true) {
        for(int j=0;j<40;j++) {
            waitingDeliveries.clear();
            activeDeliveries.clear();
            completedDeliveries.clear();
            vans.clear();
            edges.clear();
            client.getInformation(time, edges, vans, waitingDeliveries, activeDeliveries, completedDeliveries, informationOutput);
            std::wcout << L"Time: " << time << "\n";
            std::wcout << L"Cargo in van 0: " << vans[0].cargo << L"\n";
            std::wcout << L"Cargo in van 1: " << vans[1].cargo << L"\n";
            std::wcout << L"Cargo in van 2: " << vans[2].cargo << L"\n";
            std::wcout << L"Cargo in van 3: " << vans[3].cargo << L"\n";
            std::wcout << L"Cargo in van 4: " << vans[4].cargo << L"\n";
            std::wcout << L"Van 0 location: " << vans[0].location.first << ", " << vans[0].location.second << L"\n";
            std::wcout << L"Van 1 location: " << vans[1].location.first << ", " << vans[1].location.second << L"\n";
            std::wcout << L"Van 2 location: " << vans[2].location.first << ", " << vans[2].location.second << L"\n";
            std::wcout << L"Van 3 location: " << vans[3].location.first << ", " << vans[3].location.second << L"\n";
            std::wcout << L"Van 4 location: " << vans[4].location.first << ", " << vans[4].location.second << L"\n";
            std::wcout << L"Number of packages: " << waitingDeliveries.size() << L"\n";
            for(int i=0;i<waitingDeliveries.size();i++) {
                std::wcout << L"Package " << i << " location: " << waitingDeliveries[i].pickUp.first << ", " << waitingDeliveries[i].pickUp.second << L"\n";
            }
            if(once) {
            spread();
            client.sendInstructions(vanInstructions,uselessString);
            once=false;
            }
            waitingDeliveries.clear();
            activeDeliveries.clear();
            completedDeliveries.clear();
            edges.clear();
            vans.clear();
        }
    }
    return 0;
}
 



int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout << L"Hello and welcome!\n";

	// Create client
	std::wstring name = L"Group1";
	bool OK;
	DM_Client client(name, OK);

	// Start game and print information
	std::vector<std::vector<std::wstring>> nodes;
	std::wstring startOutput;
	client.startGame(nodes, startOutput);
	std::wcout << L"Node type at (10,10): " << nodes[40][40] << L"\n";
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

	// spread out cars


	//main loop
	while(completedDeliveries.size() < 20){
		edges.clear();
		vans.clear();
		waitingDeliveries.clear();
		activeDeliveries.clear();
		completedDeliveries.clear();
		client.getInformation(time, edges, vans, waitingDeliveries, activeDeliveries, completedDeliveries, informationOutput);

		std::list<VanInfo> availableVans;
		for(int i = 0; i < vans.size(); i++){
			VanInfo currentVan = vans[i];
			if (currentVan.cargo != -1){									//vans that have a delivery
				DeliveryInfo  delivery = activeDeliveries[currentVan.cargo];
				if (currentVan.instructions.size() == 0){					//stranded vans and vans that just picked up a delivery
					//calculate route from van to dropoff point
					//sendInstructions to van
				}
				else if (delivery.dropOff != currentVan.instructions[currentVan.instructions.size()]){	//vans that have accidently picked up delivery
					//calculate route from van to dropoff point
					//sendInstructions to van
				}
			}
			else{
				availableVans.push_back(currentVan);	//add van to list of available vans
			}
		}

		int i = 0;
		while(!availableVans.empty() &&  i < waitingDeliveries.size()){
			//find closest van in available list
			//calculate route from closest van to pickup point
			//send Instructions to closest van
			i++;
		}
	}


	return 0;
}