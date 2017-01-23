#include <iostream>
#include <cstdio>
#include "prime1/Prime1JsonDumper.hpp"
#include "prime1/Prime1ConsoleDumper.hpp"

using namespace std;
using namespace nlohmann;

char *memory;
size_t memory_size;

int main() {
  FILE *f = fopen("ram_crashed.raw", "rb");
  fseek(f, 0, SEEK_END);
  memory_size = static_cast<size_t>(ftell(f));
  fseek(f, 0, SEEK_SET);

  memory = new char[memory_size];
  fread(memory, 1, memory_size, f);

//  cout << Prime1JsonDumper::parseHeap().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePlayer().dump(2) << endl;
//  cout << Prime1JsonDumper::parseWorld().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePool().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePoolBasic().dump(2) << endl;
  cout << Prime1JsonDumper::parsePoolSummary().dump(2) << endl;
  cout << Prime1JsonDumper::parseHeapStats().dump(2) << endl;

//  MemoryBuffer buff(0x1000);
//  Prime1ConsoleDumper::dump(buff);
//  cout << buff.getBuff() << endl;

//  json json_message;
//  //Prime 1
////        json_message["heap"] = Prime1JsonDumper::parseHeap();
//  json_message["camera"] = Prime1JsonDumper::parseCamera();
////        json_message["player"] = Prime1JsonDumper::parsePlayer();
//  json_message["player_raw"] = Prime1JsonDumper::parsePlayerRaw();
//  json_message["world"] = Prime1JsonDumper::parseWorld();
//  json_message["pool_summary"] = Prime1JsonDumper::parsePoolSummary();
//
//  cout << json_message.dump(2) << endl;

  return 0;
}