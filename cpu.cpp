#include "cpu.h"
#include <iostream>
#include <fstream>
#include <cstring>

//set Zero Negative flags
void CPU::setZN(uint8_t value) {

  if (value==0) P = P | 0b00000010;
  else          P = P & 0b11111101;
  if (value & 0b10000000) P = P | 0b10000000;
  else                    P = P & 0b01111111;
}

//set Carry flag
void CPU::setC(bool condition) {

  if (condition) P = P | 0b00000001;
  else           P = P & 0b11111110;
}

//set Overflow flag
void CPU::setV(bool condition) {

  if (condition) P = P | 0b01000000;
  else           P = P & 0b10111111;
}

//set interrupt flag
void CPU::setI(bool condition) {

  if (condition) P = P | 0b00000100;
  else           P = P & 0b11111011;
}

//set Decimal flag
void CPU::setD(bool condition) {

  if (condition) P = P | 0b00001000;
  else           P = P & 0b11110111;
}

//helper to load ROM
bool CPU::loadROM(std::string filename) {

  //read binary file
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {return false;}

  uint8_t header[16];
  file.read((char*)header, 16);

  //check NES magic byte
  if (!(header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == 0x1A)) {
    return false;
  }

  //check for number of PRG banks
  //copy 16KB and mirror
  if (header[4] == 1) {
    file.read((char*)&memory[0x8000], 0x4000);
    std::memcpy(&memory[0xC000], &memory[0x8000], 0x4000);
  }
  //copy 32KB
  else {
    file.read((char*)&memory[0x8000], 0x4000);
    file.read((char*)&memory[0xC000], 0x4000);
  }


  //todo - add CHR bank for PPU

  return true;
}

//helper to fetch 16b addr
uint16_t CPU::fetchAddr() {

  uint16_t lo = fetch();
  uint16_t hi = fetch();
  return (hi << 8) | lo;
}

//helper to push in stack
void CPU::push(uint8_t value) {

  memory[0x0100 + SP--] = value;
}

//helper to pop stack
uint8_t CPU::pop() {

  return memory[0x0100 + ++SP];
}

uint8_t CPU::read(uint16_t addr) {

  //return memory
  if (addr <= 0x1FFF) {
      return memory[addr & 0x07FF];
  }
  //return PPU registers
  else if (addr <= 0x3FFF) {
    return 0;
  }
  //cartridge ROM
  else if (addr >= 0x8000) {
    return memory[addr];
  }
  //out of bound
  else {
    return 0;
  }
}

void CPU::write(uint16_t addr, uint8_t data) {

  //write memory
  if (addr <= 0x1FFF) {
    memory[addr& 0x07FF] = data;
  }
  //write PPU registers
  else if (addr <= 0x3FFF) {
    return ;
  }
  //cartridge ROM
  else if (addr >= 0x8000) {
    return ;
  }
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

    //BIT
    case 0x24: {
      uint8_t value = read(fetch());
      setZN(A & value);
      P = (P & 0b00111111) | (value & 0b11000000);
      break;
    }
  
    //PLP
    case 0x28:
      P = pop();
      break;

    //CMP
    case 0xC9: {
      uint8_t value = fetch();
      setC(A >= value);
      setZN(A - value);
      break;
    }

    //AND
    case 0x29:
      A &= fetch();
      setZN(A);
      break;

    //BVC
    case 0xF8:
      setD(true);
      break;

    //BEQ
    case 0x50:
      {
      int8_t offset = (int8_t)fetch();
      if (P & 0b00000010) {
        PC += offset;
      }
      break;
    }

    //BCC
    case 0x90:
      {
      int8_t offset = (int8_t)fetch();
      if (!(P & 0b00000001)) {
        PC += offset;
      }
      break;
    }

    //BCS
    case 0xB0:
      {
      int8_t offset = (int8_t)fetch();
      if (P & 0b00000001) {
        PC += offset;
      }
      break;
    }

    //SEC
    case 0x38:
      setC(true);
      break;

    //STX
    case 0x86:
      write(fetch(), X);
      break;

    //CPY
    case 0xC0: {
      uint8_t value = fetch();
      setC(Y >= value);
      setZN(Y - value);
      break;
    }

    //ROL
    case 0x2A: {
      uint8_t oldCarry = P & 0b00000001;
      setC(A & 0b10000000);
      A = (A << 1) | oldCarry;
      setZN(A);
      break;
    }

    //STX
    case 0x8E:
      write(fetchAddr(), X);
      break;

    //BPL
    case 0x10:
      {
      int8_t offset = (int8_t)fetch();
      if (!(P & 0b10000000)) {
        PC += offset;
      }
      break;
    }

    //TXS
    case 0x9A:
      SP = X;
      break;

    //CLC
    case 0x18:
      setC(false);
      break;

    //BVS
    case 0x70:
      {
      int8_t offset = (int8_t)fetch();
      if (P & 0b01000000) {
        PC += offset;
      }
      break;
    }

    //BMI
    case 0x30:
      {
      int8_t offset = (int8_t)fetch();
      if (P & 0b10000000) {
        PC += offset;
      }
      break;
    }

    //LDA
    case 0xAD:
      A = read(fetchAddr());
      setZN(A);
      break;

    //STA
    case 0x85:
      write(fetch(), A);
      break;

    //LDX
    case 0xA2:
      X = fetch();
      setZN(X);
      break;

    //CLD
    case 0xD8:
      setD(false);
      break;

    //SEI
    case 0x78:
      setI(true);
      break;

    //RTS
    case 0x60: {
      uint8_t lo = pop();
      uint8_t hi = pop();
      PC = ((hi << 8) | lo) + 1;
      break;
    }

   //JSR
    case 0x20: {
      uint16_t target = fetchAddr();
      PC--;
      push(PC >> 8);
      push(PC & 0xFF);
      PC = target;
      break;
    }
               
    //PHP
    case 0x08:
      push(P);
      break;

    //PLA
    case 0x68:
      A = pop();
      setZN(A);
      break;

    //PHA
    case 0x48:
      push(A);
      break;

    //ADC
    case 0x69: {
      uint8_t value = fetch();
      uint16_t result = A + value + (P & 0b00000001);
      setC(result > 0xFF);
      setV((~(A ^ value) & (A ^ result)) & 0x80);
      A = result & 0xFF;
      setZN(A);
      break;
    }

    //BNE
    case 0xD0: {
      int8_t offset = (int8_t)fetch();
      if (!(P & 0b00000010)) {
        PC += offset;
      }
      break;
    }

    //JMP
    case 0x4c:
      PC = fetchAddr();
      break;

    //LDA
    case 0xA9:
      A = fetch();
      setZN(A);
      break;

    //STA
    case 0x8D: {
      write(fetchAddr(), A);
      break;
    }

    //NOP
    case 0xEA:
      break;

    //err
    default:
      std::cout << "unknown opcode: 0x" << std::hex << (int)opcode 
              << " at PC: 0x" << (PC-1) << "\n";
      return;
  }
}
