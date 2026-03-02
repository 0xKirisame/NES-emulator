#pragma once
#include <cstdint>
#include <array>

struct CPU {

  std::uint8_t A = 0;
  std::uint8_t X = 0;
  std::uint8_t Y = 0;
  std::uint16_t PC = 0;
  std::uint8_t SP = 0;
  std::uint8_t P = 0;

  std::array<uint8_t, 65536> memory;

  void setZN(uint8_t value);
  void setC(bool condition);
  void setV(bool condition);
  void setI(bool condition);
  void setD(bool condition);

  uint16_t fetchAddr();
  void push(uint8_t value);
  uint8_t pop();
  bool loadROM(std::string filename);

  std::uint8_t read(uint16_t addr);
  void write(uint16_t addr, uint8_t data);
  std::uint8_t fetch();
  void reset();
  void step();
};
