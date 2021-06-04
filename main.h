#ifndef __main_h__
#define __main_h__


//includes
#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <vector>
using namespace std;
#include <stdlib.h>
#include <iostream>
#include <list>
#include <algorithm>

//structs

struct packet {
	int time;
	int len;
	double weight;
	double last;
	bool weight_provided;
	string weight_as_string;
};

struct departure_data {
	double last;
	double weight;
	double next_weight = 0;
	string connection_id;

	bool operator<(const departure_data& x)
	{
		return (last < x.last);
	}
};

struct connection {
	string conn_id;
	list<packet> packet_list;
	double connection_weight = 1;
	string connection_weight_as_string = "1";
};

size_t split(const std::string& txt, std::vector<std::string>& strs, char ch);
vector<string> parse_line(string line);
bool get_line();
void update_round(int t_plus_x_time);
void find_packet_place();
void push_to_packet_list();
void push_packets_until_time(int time);
bool compareByLength(const departure_data& a, const departure_data& b);
double get_last_with_same_id_and_update_next_weight(departure_data departure);
void update_weights_until_time(int time);

#endif // __main_h__