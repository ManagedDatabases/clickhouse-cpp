#include <clickhouse/client.h>
#include <iostream>
using namespace clickhouse;
using namespace std;
int main() {
/// Initialize client connection.
Client client(ClientOptions()
		.SetEndpoints({
			{"localhost", 9881},
                        {"localhost", 9891} /// port is ClientOptions.port
              }));


client.Select("select path from system.disks", [](const Block& block) { 
	//	std::cout << "size of blocks is " << block.GetRowCount() << std::endl;
                if (block.GetRowCount() > 0) std::cout << block[0]->As<ColumnString>()->At(0) << std::endl; 
		});

}
