
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


template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ') {
	std::stringstream ss{ str };
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}

struct IPAdress
{
	std::string IPAddressRaw;
	IPv4Decooded_t IPAddressDecoded;
	
	IPAdress() = default;
	IPAdress(IPAdress const&) = default;
	IPAdress(IPAdress &&) = default;
	IPAdress& operator=(IPAdress &&) = default;
	IPAdress& operator=(IPAdress const &) = default;
	~IPAdress() = default;
	IPAdress(std::string&& ipAdress) {
		IPAddressRaw = ipAdress;
		IPAddressDecoded = decodeIP(ipAdress);
	}
	
	IPv4Decooded_t decodeIP(std::string const& rawIP) {
		IPv4Decooded_t decodedIP;
		std::vector<std::string> ips;
		split(rawIP, ips, '.');
		for (size_t i = 0; i < 4; i++) {
			decodedIP[i] = std::stoi(ips[i]);
		}
		return decodedIP;
	}
};
std::ostream& operator<<(std::ostream& ostr, const IPAdress& ip) {
	ostr << ip.IPAddressRaw;
	return ostr;
}

void filter(const uint8_t& val, const IPAddresContainer& IPVector) {
	for (const auto &i : IPVector) {
		if (i.IPAddressDecoded[0] == val) {
			std::cout << i.IPAddressRaw << std::endl;
		}
	};
}
void filter(const uint8_t &val1, const uint8_t &val2, const IPAddresContainer& IPVector) {
	for (const auto &i : IPVector) {
		if ((i.IPAddressDecoded[0] == val1) &&
			(i.IPAddressDecoded[1] == val2)) {
			std::cout << i.IPAddressRaw << std::endl;
		}
	};
}
void filter_any(const uint8_t& val, const IPAddresContainer& IPVector) {
	for (const auto &i : IPVector) {
		if (std::find(i.IPAddressDecoded.begin(), i.IPAddressDecoded.end(), val) != i.IPAddressDecoded.end()) {
			std::cout << i.IPAddressRaw << std::endl;
		}
	};
}
void lexicographicalSort(IPAddresContainer &ip_pool) {
	std::sort(ip_pool.begin(), ip_pool.end(), [](const IPAdress& f, const IPAdress& s) {
		for (size_t i = 0; i < 4; i++) {
			if (f.IPAddressDecoded[i] > s.IPAddressDecoded[i]) {
				return true;
			}
			else if (f.IPAddressDecoded[i] < s.IPAddressDecoded[i])
			{
				return false;
			}
		}
		return false;
	});
}
int main(int argc, char const *argv[]) {
	try
	{
		IPAddresContainer ip_pool;

		for (std::string line; std::getline(std::cin, line);) {
			ip_pool.push_back(IPAdress{ std::string(line.begin(), std::find(line.begin(), line.end(), '\t')) });
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
