#include "main.h"

//Global Vars
int total_weight = 0;
int packets_count = 0;
int curr_time = 0;
float curr_round = 0;
vector<string> line_vector;
string packet_connection;
int packet_time;
packet curr_packet;
vector<connection> connection_vector;
int connection_vector_capacity;


int main(int argc, char* argv[])
{

	while (true)
	{
		if (packets_count == 0)
		{
			get_line();
			update_round(packet_time);
			curr_time = packet_time;


		}

		break;
	}
}

size_t split(const std::string& txt, std::vector<std::string>& strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

vector<string> parse_line(string line)
{
	vector<string> v;

	split(line, v, ' ');
	return v;
}

void get_line()
{
	string line;
	getline(cin, line);
	cout << line << '\n';
	line_vector = parse_line(line);
	packet_time = stoi(line_vector[0]);
	packet_connection = line_vector[1] + " " + line_vector[2] + " " + line_vector[3] + " " + line_vector[4];
	curr_packet.len = stoi(line_vector[5]);
	curr_packet.weight = line_vector.size() == 7 ? stof(line_vector[6]) : -1.0;
	cout << packet_time << "\n" << packet_connection << "\n" << curr_packet.len << "\n" << curr_packet.weight << "\n\n";
}

void update_round(int t_plus_x_time)
{
	int x_time = t_plus_x_time - curr_time;
	curr_round = curr_round + x_time / total_weight;
}

list<packet> find_packet_place()
{
	for(int i=0; i<connection_vector_capacity; i++)
	{
		if (connection_vector[i].conn_id == packet_connection)
		{
			return connection_vector[i].packet_list;
		}
	}
	// First time this connection arrives
	connection_vector[connection_vector_capacity].conn_id = packet_connection;
	connection_vector_capacity += 1;
	return connection_vector[connection_vector_capacity - 1].packet_list;
}