#ifndef CHIP8_H_
#define CHIP8_H_
#include <stdint.h>

class chip8 {
    public:
        chip8();
        ~chip8();
        void cycle();
        void load_rom();

    private:
        uint8_t mem[4096];  // memory
        uint16_t stack[16]; // stack
        uint16_t SP;        // stack pointer

        uint8_t V[16];      // general purpose registers

        uint16_t I;         // index register
        uint16_t PC;        // program counter
        uint16_t opcode;    // opcode

        uint8_t delay;      // delay register
        uint8_t sound;      // sound register

        void initialize(); 

        uint16_t nnn(uint16_t opcode);
        uint8_t n(uint16_t opcode);
        uint8_t x(uint16_t opcode);
        uint8_t y(uint16_t opcode);
        uint8_t kk(uint16_t opcode);

};

#endif
