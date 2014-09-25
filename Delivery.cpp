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
	Problem(Location start, Location goal) {
		s = start;
		g = goal;
	}

	Location getStart() {
		return s;
	}

	Location getGoal() {
		return g;
	}

private:
	Location s;
	Location g;
}


class Node {
public:
	Node() {
		s = 0;
		pc = 0;
	}
	
	Node(Location state, int pathCost) {
		s = state;
		pc = pathCost;
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

private:
	Location s;
	int pc;
};


std::vector<std::pair<int,int>> aStar(Problem problem) {
	Node startNode(problem.getStart(), 0);

	std::priority_queue<Node, std::vector<Node>, Node> frontier;
	frontier.push(startNode);

	std::set<Location> explored;

	while(true) {
		if (frontier.empty()) {
			return NULL;
		}
		
		Node node = frontier.top();
		frontier.pop();

		if (node.getState() == problem.getGoal()) {
			return solution(node); // TBI
		}

		explored.insert(node.getState);
		
		// Find children of node
		// Iterate over them
			// If the child is not in explored or frontier, insert the child into the frontier
			// Else if the child is in frontier with higher path cost, replace that node with the child
	}
}




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





