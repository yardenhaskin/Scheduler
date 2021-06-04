#include "main.h"

//Global Vars
double total_weight = 0;
int packets_count = 0;
int curr_time = 0;
double curr_round = 0;
vector<string> line_vector;
string packet_connection;
int packet_time;
packet curr_packet;
vector<connection> connection_vector;
vector<connection> connection_vector_after_departure;
int connection_vector_capacity;
list<packet> *curr_packet_list;
connection *curr_connection;
connection *scheduled_connection;
int scheduled_connection_number;
packet *scheduled_packet;
int finish_time;
bool no_more_line_to_read = false;
list<departure_data> departures_list;

int main(int argc, char* argv[])
{
	if (!get_line())// no more lines to read
		return 0;
	while (true)
	{
		// no packets in the queue - read lines with closest time
		if ((packets_count == 0))
		{
			if (no_more_line_to_read)
				break;
			update_round(packet_time);
			curr_time = packet_time;
			push_to_packet_list();
			push_packets_until_time(curr_time);
		}

		//choose packet to schedule
		double min_last = INFINITY;

		for (int i = 0; i < connection_vector_capacity; i++)
			if (!connection_vector[i].packet_list.empty())
				if (connection_vector[i].packet_list.front().last < min_last)
				{
					scheduled_connection = &connection_vector[i];
					min_last = scheduled_connection->packet_list.front().last;
					scheduled_connection_number = i;
				}
		
		scheduled_packet = &scheduled_connection->packet_list.front();
		finish_time = curr_time + scheduled_connection->packet_list.front().len;
		
		// Print the scheduled packet
		cout << curr_time << ": " << scheduled_packet->time << " " << scheduled_connection->conn_id << " " << scheduled_packet->len;
		if (scheduled_packet->weight_provided)
			cout << " " << scheduled_packet->weight_as_string;
		cout << "\n";

		// read lines until the finish time - FIXME: need to include departures
		if ((curr_packet.time <= finish_time) & !no_more_line_to_read)
		{
			update_weights_until_time(packet_time);
			push_to_packet_list();
			push_packets_until_time(finish_time);
		}
		else
			update_weights_until_time(finish_time);

		//finish time arrived, update round and curr_time
		scheduled_connection = &connection_vector[scheduled_connection_number];
		scheduled_packet = &scheduled_connection->packet_list.front();
		scheduled_connection->packet_list.pop_front();
		packets_count -= 1;
		if (no_more_line_to_read & (packets_count == 0))
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

bool get_line()
{
	string line;
	if (getline(cin, line))
	{
		line_vector = parse_line(line);
		packet_time = stoi(line_vector[0]);
		packet_connection = line_vector[1] + " " + line_vector[2] + " " + line_vector[3] + " " + line_vector[4];
		curr_packet.len = stoi(line_vector[5]);
		curr_packet.weight_as_string = line_vector.size() == 7 ? line_vector[6] : "-1.0";
		curr_packet.weight = line_vector.size() == 7 ? stod(line_vector[6]) : -1.0;
		curr_packet.weight_provided = line_vector.size() == 7;
		curr_packet.time = packet_time;
		return true;
	}
	else
	{
		no_more_line_to_read = true;
		return false;
	}
}

void update_round(int t_plus_x_time)
{
	int x_time = t_plus_x_time - curr_time;
	if (total_weight <= 0.000001)
		total_weight = 0;
	else
		curr_round = curr_round + (x_time / total_weight);
}

void find_packet_place()
{
	connection temp_connection;
	for(int i=0; i<connection_vector_capacity; i++)
	{
		if (connection_vector[i].conn_id == packet_connection)
		{
			curr_connection = &connection_vector[i];
			curr_packet_list = &connection_vector[i].packet_list;
			return;
		}
	}
	// First time this connection arrives
	connection_vector.push_back(temp_connection);
	connection_vector[connection_vector_capacity].conn_id = packet_connection;
	connection_vector_capacity += 1;
	curr_connection = &connection_vector[connection_vector_capacity - 1];
	curr_packet_list = &connection_vector[connection_vector_capacity - 1].packet_list;
	return;
}

void push_to_packet_list()
{
	departure_data temp_departure;
	double prev_last;

	find_packet_place();
	if (curr_packet.weight == -1)
	{
		curr_packet.weight = curr_connection->connection_weight;
		curr_packet.weight_as_string = curr_connection->connection_weight_as_string;
	}
	else
	{
		curr_connection->connection_weight = curr_packet.weight;
		curr_connection->connection_weight_as_string = curr_packet.weight_as_string;
	}

	temp_departure.weight = curr_packet.weight;
	temp_departure.connection_id = curr_connection->conn_id;

	if (departures_list.empty())
	{
		total_weight += curr_packet.weight;
		total_weight = round(1000000*total_weight)/ 1000000;
		curr_packet.last = curr_round + (curr_packet.len / curr_packet.weight);
		temp_departure.last = curr_packet.last;
		departures_list.push_back(temp_departure);
	}
	else
	{
		prev_last = get_last_with_same_id_and_update_next_weight(temp_departure);
		curr_packet.last = max(prev_last, curr_round) + (curr_packet.len / curr_packet.weight);
		temp_departure.last = curr_packet.last;
		departures_list.push_back(temp_departure);
		departures_list.sort();
	}
	
	curr_packet_list->push_back(curr_packet);
	packets_count += 1;
}

void push_packets_until_time(int time)
{
	while (true)
	{
		if (!get_line())// no more lines to read
		{
			update_weights_until_time(finish_time);
			return;
		}
		if (packet_time <= time)
		{
			update_weights_until_time(packet_time);
			push_to_packet_list();
		}
		else
		{
			update_weights_until_time(finish_time);
			return;
		}
	}
}


double get_last_with_same_id_and_update_next_weight(departure_data departure)
{
	for (list<departure_data>::reverse_iterator it = departures_list.rbegin(); it != departures_list.rend(); ++it)
	{
		if (it->connection_id == departure.connection_id)
		{
			it->next_weight = departure.weight;
			return it->last;
		}
	}
	total_weight += departure.weight;
	total_weight = round(1000000 * total_weight) / 1000000;
	return 0;
}

void update_weights_until_time(int time)
{
	while (curr_time < time)
	{
		update_round(curr_time + 1);
		curr_time++;
		if (!departures_list.empty())
		{
			while (departures_list.front().last <= (curr_round + 0.000001))
			{
				total_weight += departures_list.front().next_weight - departures_list.front().weight;
				departures_list.pop_front();
				if (departures_list.empty())
					break;
			}
		}
	}
}