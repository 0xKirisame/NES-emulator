#include <iostream>
#include "cpu.h"

int main() {

 CPU cpu;

 cpu.memory[0x0000] = 0xA9;
 cpu.memory[0x0001] = 0x42;
 cpu.memory[0x0002] = 0x8D;
 cpu.memory[0x0003] = 0x00;
 cpu.memory[0x0004] = 0x02;
 cpu.memory[0x0005] = 0x4c;
 cpu.memory[0x0006] = 0x00;
 cpu.memory[0x0007] = 0x00;

 
 cpu.memory[0xFFFC] = 0x00;
 cpu.memory[0xFFFD] = 0x00;

 cpu.reset();
  
 cpu.step(); std::cout << "PC: " << std::hex << cpu.PC << "\n";
 cpu.step(); std::cout << "PC: " << std::hex << cpu.PC << "\n";
 cpu.step(); std::cout << "PC: " << std::hex << cpu.PC << "\n";
 cpu.step(); std::cout << "PC: " << std::hex << cpu.PC << "\n";

 std::cout << std::hex << (int)cpu.memory[0x0200] << std::endl;

 return 0;

}
