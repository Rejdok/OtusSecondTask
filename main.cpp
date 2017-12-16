#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>

using IPv4Decooded_t = std::array<uint8_t, 4>;
struct IPAdress;
using IPAddresContainer = std::vector<IPAdress>;
struct IPAdress
{
	std::string IPAddresRaw;
	IPv4Decooded_t IPAddresDecoded;

};
std::ostream& operator<<(std::ostream& ostr, const IPAdress& ip) {
	ostr << ip.IPAddresRaw;
	return ostr;
}
template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
	std::stringstream ss{ str };
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}
IPv4Decooded_t decodeIP(std::string& rawIP) {
	IPv4Decooded_t decodedIP;
	std::vector<std::string> ips;
	split(rawIP, ips, '.');
	for (size_t i = 0; i < 4; i++) {
		decodedIP[i] = std::stoi(ips[i]);
	}
	return decodedIP;
}
void filter(const uint8_t& val, const IPAddresContainer& IPVector) {
	for (const auto &i : IPVector) {
		if (i.IPAddresDecoded[0] == val) {
			std::cout << i.IPAddresRaw << std::endl;
		}
	};
}
void filter(const uint8_t &val1, const uint8_t &val2, const IPAddresContainer& IPVector) {
	for (const auto &i : IPVector) {
		if ( (i.IPAddresDecoded[0] == val1 )&&
			(i.IPAddresDecoded[1] == val2)) {
			std::cout << i.IPAddresRaw << std::endl;
		}
	};
}
void filter_any(const uint8_t& val, const IPAddresContainer& IPVector) {
	for (const auto &i : IPVector) {
		if (std::find(i.IPAddresDecoded.begin(), i.IPAddresDecoded.end(), val) != i.IPAddresDecoded.end()) {
			std::cout << i.IPAddresRaw << std::endl;
		}
	};
}
void lexicographicalSort(IPAddresContainer &ip_pool){
	std::sort(ip_pool.begin(), ip_pool.end(), [](IPAdress& f, IPAdress& s) {
			for (size_t i = 0; i < 4; i++) {
				if (f.IPAddresDecoded[i] > s.IPAddresDecoded[i]) {
					return true;
				}
				else if (f.IPAddresDecoded[i] < s.IPAddresDecoded[i])
				{
					return false;
				}
			}
			return false;
		});
}
int main(int argc, char const *argv[])
{
	try
	{
		IPAddresContainer ip_pool;

		for (std::string line; std::getline(std::cin, line);)
		{	
			auto g = std::string(line.begin(), std::find(line.begin(), line.end(), '\t'));
			ip_pool.push_back({g, decodeIP(g) });
		}
		lexicographicalSort(ip_pool);
		for (const auto& i : ip_pool) {
			std::cout << i << std::endl;
		}
		filter(1, ip_pool);

		filter(46, 70, ip_pool);

		filter_any(46, ip_pool);

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
