#include "address.h"
#include "socket.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {
	auto addrs = Address::find_by_name("www.google.com", "80", SocketType::TCP, AF_INET6);
	std::vector<Address> sorted_addrs(addrs.begin(), addrs.end());
	std::sort(sorted_addrs.begin(), sorted_addrs.end());

	for (const auto& addr : sorted_addrs) {
		std::cout << addr << std::endl;
	}
	
	getchar();

	return 0;
}