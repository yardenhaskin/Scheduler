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

//structs

struct packet {
	int time;
	int len;
	double weight;
	double last;
	bool weight_provided;
	string weight_as_string;
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


#endif // __main_h__