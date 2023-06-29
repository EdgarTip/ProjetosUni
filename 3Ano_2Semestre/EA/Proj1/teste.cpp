#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <cstring>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
using namespace std;

int intersection(vector<string>& nums1, vector<string>& nums2) {
        unordered_set<string> m(nums1.begin(), nums1.end());
        vector<string> res;
        for (auto a : nums2)
            if (m.count(a)) {
                res.push_back(a);
                m.erase(a);
            }

        return res.size();
    }


int main() {

    vector<string> a = {"1", "2", "3", "4"};
    vector<string> b = {"4", "2", "2", "2"};

    int c = intersection(a, b);

    cout << "LEFT -" + to_string(c) << endl;


}