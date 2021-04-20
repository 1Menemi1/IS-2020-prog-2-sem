#include <fstream>
#include <set>
#include <map>
#include "pugixml.hpp"
#include "analysis.h"

using namespace std;

int main() {
    ofstream fout("output.txt");
    setlocale(0, "");
    pugi::xml_document xml;
    xml.load_file("..\\data\\data-20210322T085457-structure-20190416T084545.xml");

    vector<Analysis> stations;

    for (auto &station: xml.child("dataset").children("transport_station")) {
        Analysis tempStation;

        tempStation.setID(atoi(station.child_value("number")));
        tempStation.setType(station.child_value("type_of_vehicle"));
        tempStation.setStreets(station.child_value("location"));
        tempStation.setRoutes(station.child_value("routes"));
        tempStation.setCoords(station.child_value("coordinates"));

        stations.push_back(tempStation);
    }

    set<string> transportType;
    map<string, unsigned> stopsStreet;

    for (const Analysis &s: stations) {
        transportType.insert(s.getType());

        for (const string &street: s.getStreets()) {
            if (stopsStreet.find(street) != stopsStreet.end())
                stopsStreet[street] += 1;
            else
                stopsStreet[street] = 1;
        }
    }


    map<string, pair<string, unsigned>> maxStopsRoute;
    for (const auto &type: transportType) {
        maxStopsRoute[type].first = "";
        maxStopsRoute[type].second = 0;

        map<string, unsigned> stopsRoute;
        for (const auto& s: stations) {
            if (s.getType() != type)
                continue;

            for (const auto& route: s.getRoutes()) {
                if (stopsRoute.find(route) != stopsRoute.end())
                    stopsRoute[route]++;
                else
                    stopsRoute[route] = 1;
            }
        }

        for (auto[route, num_of_stops]: stopsRoute)
            if (num_of_stops > maxStopsRoute[type].second) {
                maxStopsRoute[type].second = num_of_stops;
                maxStopsRoute[type].first = route;
            }
    }


    map<string, pair<string, double>> maxRoute;
    for (const auto &type: transportType) {
        maxRoute[type].first = "";
        maxRoute[type].second = 0;
        map<string, vector<pair<double, double>>> stopsRoute;

        for (const auto &s: stations) {
            if (s.getType() != type)
                continue;
            for (const auto &route: s.getRoutes()) {
                if (stopsRoute.find(route) != stopsRoute.end())
                    stopsRoute[route].push_back(s.getCoords());
                else
                    stopsRoute[route] = vector<pair<double, double>>({s.getCoords()});
            }
        }

        for (auto[route, stops_coors]: stopsRoute) {
            double length = Analysis::length(stops_coors);
            if (length > maxRoute[type].second) {
                maxRoute[type].second = length;
                maxRoute[type].first = route;
            }
        }
    }


    string streetMaxRoutes;
    unsigned maxStops = 0;

    for (auto[street, stopsNum]: stopsStreet)
        if (stopsNum > maxStops) {
            maxStops = stopsNum;
            streetMaxRoutes = street;
        }


    fout << "=-=-=Маршрут с наибольшим кол-ом остановок по отдельными видам транспорта=-=-=\n\n";
    for (const auto &i: transportType)
        fout << maxStopsRoute[i].first << "ый " << i << " - " << maxStopsRoute[i].second << " остановки(-ок)\n";

    fout << "\n=-=-=-=-=-=-Наиболее длинные маршруты по отдельным видам транпорта-=-=-=-=-=-=\n\n";
    for (const auto &i: transportType)
        fout << maxRoute[i].first << "ый " << i << " - " << maxRoute[i].second << "км\n";

    fout << "\n=-=-=-=-=-=-=-=-=-=-=Улица с наибольшим числом остановок=-=-=-=-=-=-=-=-=-=-=\n\n";
    fout << streetMaxRoutes << " - " << maxStops << " остановки(-ок)";
}