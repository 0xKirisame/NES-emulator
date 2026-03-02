#include <iostream>
#include "cpu.h"

int main(int argc, char* argv[]) {

  if (argc < 2) {
      std::cout << "Usage: ./nes-emu <rom.nes>\n";
      return 1;
  }
    
  CPU cpu;

  if (!cpu.loadROM(argv[1])) {
      std::cout << "Failed to load ROM\n";
      return 1;
  }
  std::cout << "ROM loaded\n";

  cpu.memory[0xFFFC] = 0x00;  // force PC to start at 0xC000
  cpu.memory[0xFFFD] = 0xC0;

  cpu.reset();
  std::cout << "PC after reset: " << std::hex << cpu.PC << "\n";
    
  // run 1000 steps and see what happens
  for (int i = 0; i < 10000; i++) {
      cpu.step();
  }
  
  std::cout << "Test result at 0x02: " << std::hex << (int)cpu.memory[0x02] << "\n";
  std::cout << "PC: " << std::hex << cpu.PC << "\n";
  std::cout << "A: " << std::hex << (int)cpu.A << "\n";


 return 0;

}
