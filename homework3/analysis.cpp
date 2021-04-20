#include <cmath>
#include "analysis.h"

using namespace std;

const int MERIDIAN_DEGREE = 110;
const int LATITUDE = 58;

void Analysis::setID(int id) {
    this->id = id;
}

void Analysis::setType(const string &type) {
    this->type = type;
}

void Analysis::setStreets(const string &str) {
    if (str.length() > 0)
        streets = split(str, ',');
}

void Analysis::setRoutes(const string &str) {
    if (str.length() > 0)
        routes = split(replace(str, ".", ","), ',');
}

void Analysis::setCoords(const string &str) {
    vector<string> tokens = split(str, ',');
    cord.first = atof(tokens[0].c_str());
    cord.second = atof(tokens[1].c_str());
}

string Analysis::getType() const {
    return type;
}

const vector<string> &Analysis::getStreets() const {
    return streets;
}

const vector<string> &Analysis::getRoutes() const {
    return routes;
}

pair<double, double> Analysis::getCoords() const {
    return cord;
}

vector<string> Analysis::split(const string &str, char ch1) {
    vector<string> result;
    int prev = 0;

    for (int i = 0; i < str.length() - 1; i++)
        if (str[i] == ch1) {
            result.push_back(str.substr(prev, i - prev));
            prev = i + 1;
        }

    result.push_back(str.substr(prev));
    return result;
}

string Analysis::replace(const string &str, const string &s1, const string &s2) {
    string temp = str;
    int i;

    while ((i = temp.find(s1)) != string::npos)
        temp.replace(i, s1.length(), s2);

    return temp;
}

double distance(pair<double, double> p1, pair<double, double> p2) {
    return sqrt(pow((p1.first - p2.first) * MERIDIAN_DEGREE, 2) +
                pow((p1.second - p2.second) * LATITUDE, 2));
}

double Analysis::length(const vector<pair<double, double>> &stops) {
    double length = 0;
    int next = 0;
    vector<bool> visited(stops.size(), false);

    while (next != stops.size()) {
        int cur = next;
        visited[cur] = true;
        double min_dist = INT64_MAX;
        next = stops.size();

        for (int i = 0; i < stops.size(); i++) {
            if (visited[i])
                continue;

            double dist;

            if ((dist = distance(stops[cur], stops[i])) < min_dist) {
                min_dist = dist;
                next = i;
            }
        }

        if (next < stops.size()) {
            for (int i = 0; i < stops.size(); i++) {
                if (!visited[i])
                    continue;

                double dist = distance(stops[next], stops[i]);
                if (dist < min_dist)
                    min_dist = dist;
            }

            length += min_dist;
        }
    }
    return length;
}