#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>


struct Node {

	std::vector<std::vector<int>> State;

	int level;

	Node* rightChild;
	Node* leftChild;
	Node* upChild;
	Node* downChild;

	Node(std::vector<std::vector<int>> State, int lvl) : State(State), level(lvl), leftChild(nullptr), upChild(nullptr), rightChild(nullptr), downChild(nullptr) {}
};

int getHeuristic(std::vector<std::vector<int>> GivenVec, int level);

struct cîmp
{
	bool operator()(Node* a, Node* b)
	{
		int heurA = getHeuristic(a->State, a->level);
		int heurB = getHeuristic(b->State, b->level);
		return heurA > heurB;
	}
};



std::vector<std::vector<int>> GoalVec = { {1,2,3}, {4,5,6}, {7,8,0} };
std::set<std::vector<std::vector<int>>> visited;
std::unordered_map<Node*, Node*> ParentsNode;
std::priority_queue<Node*, std::vector<Node*>, cîmp> Q;

std::vector<std::vector<int>> Right;
std::vector<std::vector<int>> Left;
std::vector<std::vector<int>> Down;
std::vector<std::vector<int>> Up;



std::vector<std::vector<int>> getRight(std::vector<std::vector<int>> GivenVec, int NullPosX, int NullPosY) {

	if (NullPosY == 2)
	{
		return GivenVec;
	}

	std::swap(GivenVec[NullPosX][NullPosY], GivenVec[NullPosX][NullPosY + 1]);
	return GivenVec;
}

std::vector<std::vector<int>> getLeft(std::vector<std::vector<int>> GivenVec, int NullPosX, int NullPosY) {

	if (NullPosY == 0)
	{
		return GivenVec;
	}

	std::swap(GivenVec[NullPosX][NullPosY], GivenVec[NullPosX][NullPosY - 1]);
	return GivenVec;
}

std::vector<std::vector<int>> getUp(std::vector<std::vector<int>> GivenVec, int NullPosX, int NullPosY) {

	if (NullPosX == 0)
	{
		return GivenVec;
	}

	std::swap(GivenVec[NullPosX][NullPosY], GivenVec[NullPosX - 1][NullPosY]);
	return GivenVec;
}

std::vector<std::vector<int>> getDown(std::vector<std::vector<int>> GivenVec, int NullPosX, int NullPosY) {

	if (NullPosX == 2)
	{
		return GivenVec;
	}

	std::swap(GivenVec[NullPosX][NullPosY], GivenVec[NullPosX + 1][NullPosY]);
	return GivenVec;
}

void findEmptyPiece(std::vector<std::vector<int>>& GivenVec, int& NullPosX, int& NullPosY) {

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (GivenVec[i][j] == 0) {
				NullPosX = i;
				NullPosY = j;
				return;
			}
		}
	}
}

int getHeuristic(std::vector<std::vector<int>> GivenVec, int level) {

	int sum = level;


	for (int k = 0; k < 9; ++k) {       // finding the position of every plate (0-8) in the starting 
                                        // puzzle state and in the current puzzle state,
                                        // and evaluating the Manhattan distance

		int goalX, goalY, currX, currY;

		bool found1 = false;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (GoalVec[i][j] == k) {
					goalX = i;
					goalY = j;
					found1 = true;
					break;
				}
			}
			if (found1) break;
		}

		bool found2 = false;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (GivenVec[i][j] == k) {
					currX = i;
					currY = j;
					found2 = true;
					break;
				}
			}
			if (found2) break;
		}

		sum += (abs(goalX - currX) + abs(goalY - currY));
	}

	return sum;
}


void print(std::vector<std::vector<int>>& toPrint) {

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cout << toPrint[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void Solve(std::vector<std::vector<int>>& StartVec) {

	Node* root = new Node(StartVec, 0);

	Node* finalNode = nullptr;

	Q.push(root);

	while (!Q.empty()) {

		Node* currNode = Q.top();
		Q.pop();

		visited.insert(currNode->State);

		if (currNode->State == GoalVec) {
			finalNode = currNode;
			break;
		}

		int NullPosX = -1, NullPosY = -1;
		findEmptyPiece(currNode->State, NullPosX, NullPosY);

		Right = getRight(currNode->State, NullPosX, NullPosY);
		if (visited.find(Right) == visited.end())
		{
			currNode->rightChild = new Node(Right, currNode->level + 1);
			ParentsNode.insert({ currNode->rightChild, currNode });
			Q.push(currNode->rightChild);
			visited.insert(Right);

		}

		Left = getLeft(currNode->State, NullPosX, NullPosY);
		if (visited.find(Left) == visited.end())
		{
			currNode->leftChild = new Node(Left, currNode->level + 1);
			ParentsNode.insert({ currNode->leftChild, currNode });
			Q.push(currNode->leftChild);
			visited.insert(Left);
		}

		Up = getUp(currNode->State, NullPosX, NullPosY);
		if (visited.find(Up) == visited.end())
		{
			currNode->upChild = new Node(Up, currNode->level + 1);
			ParentsNode.insert({ currNode->upChild, currNode });
			Q.push(currNode->upChild);
			visited.insert(Up);
		}

		Down = getDown(currNode->State, NullPosX, NullPosY);
		if (visited.find(Down) == visited.end())
		{
			currNode->downChild = new Node(Down, currNode->level + 1);
			ParentsNode.insert({ currNode->downChild, currNode });
			Q.push(currNode->downChild);
			visited.insert(Down);
		}

	}

	std::vector<Node*> Path;
	Node* curr = finalNode;
	Path.push_back(curr);
	while (curr != root) {
		auto it = ParentsNode.find(curr);
		if (it != ParentsNode.end()) {
			Path.push_back(it->second);
			curr = it->second;
		}
	}

	int PathSize = Path.size();
	for (int i = PathSize - 1; i >= 0; --i) {
		std::cout << "State[" << PathSize - i << "]: " << "\n\n";
		print(Path[i]->State);
		std::cout << "\n\n";
	}
	auto it = ParentsNode.begin();
	while (it != ParentsNode.end()) {
		delete it->first;
		++it;
	}
	delete root;
}

int main() {

	std::cout << "GOAL: \n\n";
	print(GoalVec);
	std::cout << "\n\n";
	std::vector<std::vector<int>> StartVec = { {0,8,7}, {1,2,3}, {5,4,6} };   // values can be changed for the starting puzzle
	std::cout << "START: \n\n";                                               // don't forget that not every puzzle has a solution
	print(StartVec);                                                          // if there's no solution the program loops infinitely
	std::cout << "\n\n";
	Solve(StartVec);

	return 0;
}