#include <iostream>
#include <cstdio>
#include <prime1/CGameState.hpp>
#include <prime1/CStateManager.hpp>
#include <prime1/CGameGlobalObjects.hpp>
#include "prime1/Prime1JsonDumper.hpp"
#include "prime1/Prime1ConsoleDumper.hpp"

#include "CRandom16Impl.hpp"

using namespace std;
using namespace nlohmann;

char *memory;
size_t memory_size;

void readObjectList();

void testRandom();

void readAreas();

int main() {
//  FILE *f = fopen("ram.raw", "rb");
  FILE *f = fopen("ram_borked79.raw", "rb");
  fseek(f, 0, SEEK_END);
  memory_size = static_cast<size_t>(ftell(f));
  fseek(f, 0, SEEK_SET);

  memory = new char[memory_size];
  fread(memory, 1, memory_size, f);

  readAreas();
  //  testRandom();
//  readObjectList();


//  cout << Prime1JsonDumper::parseHeap().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePlayerRaw().dump(2) << endl;
//  cout << Prime1JsonDumper::parseWorld().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePool().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePoolBasic().dump(2) << endl;
//  cout << Prime1JsonDumper::parsePoolSummary().dump(2) << endl;
//  cout << Prime1JsonDumper::parseHeapStats().dump(2) << endl;

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

void readAreas() {
  CStateManager manager(CStateManager::LOCATION);

  CWorld world = manager.world.deref();
  int areaCount = world.areas.size.read();

  for (int i = 0; i < areaCount; i++) {
    auto autoPtr = world.areas[i];
//    cout << hex << " autoptr offset: " << autoPtr.ptr();
//    cout << hex << " autoptr second value: " << autoPtr.dataPtr.read();
//    cout << endl;
    CGameArea area = autoPtr.dataPtr.deref();
    cout << hex << "Area " << area.selfIDX.read() << "|" << area.mrea.read() << endl;
    cout << (area.postConstructedBool.read() ? "C" : " ");
    cout << (area.active.read() ? "A" : " ");
    cout << (area.tokensReady.read() ? "T" : " ");
    cout << (area.loadPaused.read() ? "P" : " ");
    cout << (area.validated.read() ? "V" : " ");
    cout << " phase: ";
    switch (area.phase.read()) {
      case EPhase::LoadHeader:
        cout << "Load header";
        break;
      case EPhase::LoadSecSizes:
        cout << "Load section sizes";
        break;
      case EPhase::ReserveSections:
        cout << "Reserve Sections";
        break;
      case EPhase::LoadDataSections:
        cout << "Load data sections";
        break;
      case EPhase::WaitForFinish:
        cout << "Wait for finish";
        break;
      default:
        cout << "Unknown";
    }
    cout << " chain: ";
    switch (area.curChain.read()) {
      case EChain::Invalid:
        cout << "Invalid";
        break;
      case EChain::ToDeallocate:
        cout << "ToDeallocate";
        break;
      case EChain::Deallocated:
        cout << "Deallocated";
        break;
      case EChain::Loading:
        cout << "Loading";
        break;
      case EChain::Alive:
        cout << "Alive";
        break;
      case EChain::AliveJudgement:
        cout << "Alive Judgement";
        break;
      default:
        cout << "Unknown";
    }

    if (area.postConstructed.read() != 0) {
      CPostConstructed pc = area.postConstructed.read();
      cout << " Occlusion state: ";
      switch (pc.occlusionState.read()) {
        case EOcclusionState::Occluded:
          cout << "Occluded";
          break;
        case EOcclusionState::Visible:
          cout << "Visible";
          break;
        default:
          cout << "Unknown";
      }
    }

    cout << endl;


//    cout << "Post constructed auto_ptr: " << (int) area.postConstructed.unknown.read() << " | " << area.postConstructed.dataPtr.read() << endl;

    cout << "Load transaction offset: " << area.loadTransactions.ptr() << endl;

  }
}

void testRandom() {
//  CStateManager manager(CStateManager::LOCATION);
//  CRandom16Impl impl(manager.random.seed.read());
  CRandom16Impl impl(0x91F76E96);

  for (int i = 0; i < 100; i++) {
    cout << hex << impl.GetSeed() << dec << " ";
    cout << impl.Float() << endl;
  }
}

void readObjectList() {
  CStateManager manager(CStateManager::LOCATION);

  CObjectList list = manager.allObjs.deref();

  cout << "Size: " << list.count.read() << endl;
  int counter = 0;
  int id = list.firstID.read();
  cout << hex;
  while (id != 0xFFFF) {
    counter++;
    SObjectListEntry entry = list.list[id & 0x3FF];

    CEntity entity = entry.entity.deref();
//    if (entity.editorID.read() == 0x340286) {
    cout << "ID: " << entity.editorID.read() << endl;
    cout << "Address: " << entity.editorID.ptr() << endl;
    cout << "Name loc: " << entity.name.ptr() << endl;
    cout << "Name string ptr: " << entity.name.data.read() << endl;
    cout << "Name: " << entity.name.read_str() << endl;
    cout << "Unknowns: " << entity.name.unk2.read() << " | " << entity.name.unk3.read() << " | "
         << entity.name.unk4.read() << endl;
//      cout << "State flag: " << (uint32_t)entity.status.read() << endl;
//
//      cout << "State flag address: " << entity.status.ptr() << endl;
    cout << "------------------------------" << endl << endl;
//    }
    int newId = entry.next.read();
    if (id == newId) break;
    id = newId;
    if (counter > 0x3FF) break; //somethign went wrong
  }
  if (counter != list.count.read()) {
    cout << "Something is very wrong" << endl;
  }
  cout << dec;
}