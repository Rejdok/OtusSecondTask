#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>

using IPv4Decooded_t = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;
using IPAddresContainer = std::vector<IPv4Decooded_t>;

template<size_t index, typename ToExecute, typename... Args>
struct iterate_tuple
{
	static void next(std::tuple<Args...>& tuple, ToExecute function) {
		iterate_tuple<index - 1, ToExecute, Args...>::next(tuple, function);
		function(index, std::get<index>(tuple));
	}
};
template<typename ToExecute, typename... Args>
struct iterate_tuple<0, ToExecute, Args...>
{
	static void next(std::tuple<Args...>& tuple, ToExecute function)
	{
		function(0, std::get<0>(tuple));
	}
};
template<typename Callback, typename... Args>
void for_each(std::tuple<Args...>& t, Callback callback)
{
	int const tuple_size = std::tuple_size<std::tuple<Args...>>::value;
	iterate_tuple<tuple_size - 1, Callback, Args...>::next(t, callback);
}
std::ostream& operator<<(std::ostream & ostr, IPv4Decooded_t const& ip) {
	for_each(const_cast<IPv4Decooded_t &>(ip), [&ostr](size_t const& index, auto const& element) {
		ostr << int(element);
		if (index != 3) {
			ostr << ".";
		}
	});
	return ostr;
}

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ') {
	std::stringstream ss{ str };
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}

template<typename T, typename ... Args>
void filter(T const& ip, Args... args) {
	static_assert (sizeof...(args) <= 4 && sizeof...(args) >= 1, "Count of args shoud be 1 > && <=4");
	bool shouldPrint = true;
	std::array< int, sizeof...(args) > argsUnpacked = { args... };
	std::array<uint8_t, 4> ipIntChuncks;
	std::tie(ipIntChuncks[0], ipIntChuncks[1], ipIntChuncks[2], ipIntChuncks[3]) = ip;
	for (auto i = 0; i < sizeof...(args); i++) {
		if (argsUnpacked[i] != ipIntChuncks[i]) {
			shouldPrint = false;
			break;
		}
	}
	if (shouldPrint)
		std::cout << ip << std::endl;
}
void filter_any(uint8_t const& val, IPAddresContainer & IPVector) {
	for (auto& i : IPVector) {
		bool shouldPrint = false;
		for_each(i, [&shouldPrint,&val](size_t const&& index, const auto& valInTuple) {
			if (val == valInTuple)
				shouldPrint = true;
		});

		if (shouldPrint) {
			std::cout << i << std::endl;
		}
	};
}


IPv4Decooded_t decodeIP(std::string const& rawIP) {
	std::vector<std::string> stringChunks;
	std::array<int8_t, 4> intChunks;
	split(rawIP, stringChunks, '.');
	for (size_t i = 0; i < 4; i++) {
		intChunks[i] = std::stoi(stringChunks[i]);
	}
	IPv4Decooded_t rezult;
	for_each(rezult, [&intChunks](const size_t& index, auto&& element) {
		element = intChunks[index];
	});
	return rezult;
}
int main(int argc, char const *argv[]) {
	try
	{
		IPAddresContainer ip_pool;

		for (std::string line; std::getline(std::cin, line);) {
			ip_pool.push_back(decodeIP(std::string(line.begin(), std::find(line.begin(), line.end(), '\t'))));
		}
		std::sort(ip_pool.begin(), ip_pool.end(), [](IPv4Decooded_t const & f, IPv4Decooded_t const & s) {
			return f > s;
		});
		for (const auto& i : ip_pool) {
			std::cout << i << std::endl;
		}
		for (auto ip : ip_pool) {
			filter(ip, 1);
			
		}
		for (auto ip : ip_pool) {
			filter(ip, 46, 70);
		}
		filter_any(46, ip_pool);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
