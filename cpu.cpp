#include "cpu.h"
#include <iostream>

void CPU::setZN(uint8_t value) {

  if (value==0) P = P | 0b00000010;
      else      P = P & 0b11111101;
      if (value & 0b10000000) P = P | 0b10000000;
      else                P = P & 0b01111111;
}

uint16_t CPU::fetchAddr() {

  uint16_t lo = fetch();
  uint16_t hi = fetch();
  return (hi << 8) | lo;
}

uint8_t CPU::read(uint16_t addr) {

  return memory[addr];
}

void CPU::write(uint16_t addr, uint8_t data) {

  memory[addr] = data;
}

uint8_t CPU::fetch() {

  return memory[PC++];
}

void CPU::reset(){

  A = 0;
  X = 0;
  Y = 0;

  SP = 0xFD;
  PC = read(0xFFFD) << 8 | read(0xFFFC); 
}

void CPU::step() {

  uint8_t opcode = fetch();
  
  switch (opcode) {

    case 0x4c:
      PC = fetchAddr();
      break;

    case 0xA9:
      A = fetch();
      setZN(A);
      break;

    case 0x8D: {
      write(fetchAddr(), A);
      break;
    }

    case 0xEA:
      break;

    default:
      std::cout << "unknown opcode" << std::endl;
  }
}
