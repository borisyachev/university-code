#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>



struct Company
{
	double revenue;
	short tw;  // teamwork value
	bool success;
};


void findMinMax(std::vector<Company> data, short& minTw, short& maxTw, double& minRevenue, double& maxRevenue) {
	minTw = maxTw = data[0].tw;
	minRevenue = maxRevenue = data[0].revenue;
	int dataSize = data.size();
	for (int i = 0; i < dataSize; ++i) {
		if (data[i].revenue > maxRevenue) maxRevenue = data[i].revenue;
		if (data[i].revenue < minRevenue) minRevenue = data[i].revenue;
		if (data[i].tw > maxTw) maxTw = data[i].tw;
		if (data[i].tw < minTw) minTw = data[i].tw;
	}
}

void kNN(Company query, std::vector<Company> data,int k) {

	std::vector<double> distances;

	int size = data.size();

	double maxRevenue, minRevenue; 
	short maxTw, minTw; // teamwork value

	findMinMax(data, minTw, maxTw, minRevenue, maxRevenue);
	if (query.revenue > maxRevenue) maxRevenue = query.revenue;    // checking if the 
	if (query.revenue < minRevenue) minRevenue = query.revenue;    // new query has
	if (query.tw < minTw) minTw = query.tw;                        // bigger values than the existing ones
	if (query.tw > maxTw) maxTw = query.tw;                        // so we can save it as max value

	double normalizedQueryRev = (query.revenue - minRevenue) / (maxRevenue - minRevenue);
	double normalizedQueryTw = (query.tw - minTw) / maxTw;

	std::unordered_map<double, Company> sumsSet;

	for (int i = 0; i < size; ++i) {
	
		double sum = 0;
		double normalizedCurrDataRevenue = (data[i].revenue - minRevenue) / (maxRevenue - minRevenue);
		double diffInRevenues = normalizedQueryRev - normalizedCurrDataRevenue;
		sum += diffInRevenues * diffInRevenues;

		
		short diffInTw = query.tw - data[i].tw;
		sum += diffInTw * diffInTw;

		sum = sqrt(sum);                                            // euclidean distance for the (query - i th company) found
		distances.push_back(sum);
		sumsSet.insert(std::make_pair(sum, data[i]));
	}

	std::sort(distances.begin(),distances.end());
	
	short successful = 0;
	short unsuccessful = 0;
	for (int i = 0; i < k; ++i) {                                   // checking the K nearest companies 
		auto it = sumsSet.find(distances[i]);
		(it->second.success) ? ++successful : ++unsuccessful; 
	}
	(successful > unsuccessful) ? std::cout << "SUCCESS\n" : std::cout << "NO SUCCESS\n";
}



int main() {


	std::ifstream toRead("data.txt");                              // "data.txt" can be changed with another text file with info for companies
																   // not implemented dynamically cuz of the specific file structure
	if (!toRead) {                                                 
		std::cerr << "Can't open text file.\n";
		return -1;
	}

	std::vector<Company> data;

	while (!toRead.eof()) {
		Company temp;
		toRead >> temp.revenue >> temp.tw >> temp.success;
		data.push_back(temp);
	}
	toRead.close();

	Company query = { 850, 3 , 0 };
	kNN(query, data, 5);
	return 0;
}