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
  int len;
  float weight;
  float last;
} ;

struct connection {
	string conn_id;
	list<packet> packet_list;
};

size_t split(const std::string& txt, std::vector<std::string>& strs, char ch);
vector<string> parse_line(string line);


#endif // __main_h__