#include "chip8.h"
#include <random>

/*
 * Reset all registers back to default
 */
void chip8::initialize(){
    for(int i = 0; i < 16; i++){
        stack[i] = 0;
        V[i] = 0;
    }
    SP = 0;

    I = 0;
    PC = 0x200; //Most Chip-8 programs start at location 0x200
    opcode = 0;

    delay = 0;
    sound = 0;
}

uint16_t chip8::nnn(uint16_t opcode){
    return (opcode & 0x0FFF);
}
uint8_t chip8::n(uint16_t opcode){
    return (opcode & 0x000F);
}
uint8_t chip8::x(uint16_t opcode){
    return ((opcode & 0x0F00) >> 8);
}
uint8_t chip8::y(uint16_t opcode){
    return ((opcode & 0x00F0) >> 4);
}
uint8_t chip8::kk(uint16_t opcode){
    return (opcode & 0x00FF);
}


void chip8::cycle(){
    //get opcode    
    opcode = mem[PC] << 8 | mem[PC + 1];
    PC += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F){
                case 0x0000: 
                    //00E0 - CLS
                    break;
                case 0x000E: 
                    //00EE - RET
                    break;
            }
        case 0x1000:
            //1nnn - JP addr
            PC = nnn(opcode);
            break;
        case 0x2000:
            //2nnn - CALL addr
            SP++;
            stack[SP] = PC;
            PC = nnn(opcode);
            break;
        case 0x3000:
            //3xkk - SE Vx, byte
            if (V[x(opcode)] == kk(opcode)) PC += 2;
            break;
        case 0x4000:
            //4xkk - SNE Vx, byte
            if (V[x(opcode)] != kk(opcode)) PC += 2;
            break;
        case 0x5000:
            //5xy0 - SE Vx, Vy
            if (V[x(opcode)] == V[y(opcode)]) PC += 2;
            break;
        case 0x6000:
            //6xkk - LD Vx, byte
            V[x(opcode)] = kk(opcode);
            break;
        case 0x7000:
            //7xkk - ADD Vx, byte
            V[x(opcode)] += kk(opcode);
            break;
        case 0x8000:
            switch (opcode & 0x000F){
                case 0x0000:
                    //8xy0 - LD Vx, Vy
                    V[x(opcode)] = V[y(opcode)];
                    break;
                case 0x0001:
                    //8xy1 - OR Vx, Vy
                    V[x(opcode)] |= V[y(opcode)];
                    break;
                case 0x0002:
                    //8xy2 - AND Vx, Vy
                    V[x(opcode)] &= V[y(opcode)];
                    break;
                case 0x0003:
                    //8xy3 - XOR Vx, Vy
                    V[x(opcode)] ^= V[y(opcode)];
                    break;
                case 0x0004:
                    //8xy4 - ADD Vx, Vy
                    V[0xF] = ((V[x(opcode)] + V[y(opcode)]) > 0xFF);
                    V[x(opcode)] += V[y(opcode)];
                    break;
                case 0x0005:
                    //8xy5 - SUB Vx, Vy
                    V[0xF] = (V[x(opcode)] > V[y(opcode)]);                    
                    V[x(opcode)] -= V[y(opcode)];
                    break;
                case 0x0006:
                    //8xy6 - SHR Vx {, Vy}
                    V[0xF] = (V[x(opcode)] & 1);
                    V[x(opcode)] >>= 1;
                    break;
                case 0x0007:
                    //8xy7 - SUBN Vx, Vy
                    V[0xF] = (V[y(opcode)] > V[x(opcode)]);                    
                    V[x(opcode)] = V[y(opcode)] - V[x(opcode)];
                    break;
                case 0x000E:
                    //8xyE - SHL Vx {, Vy}
                    V[0xF] = ((V[x(opcode)] & 0x80) >> 7);
                    V[x(opcode)] <<= 1;
                    break;
            }
            break;
       case 0x9000:
            //9xy0 - SNE Vx, Vy
            if (V[x(opcode)] != V[y(opcode)]) PC += 2;
            break;
       case 0xA000:
            //Annn - LD I, addr
            I = nnn(opcode);
            break;
       case 0xB000:
            //Bnnn - JP V0, addr
            PC = V[0] + nnn(opcode);
            break;
       case 0xC000: {
            //Cxkk - RND Vx, byte
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 255);
            V[x(opcode)] = distrib(gen) & kk(opcode);
            break;
       }
       case 0xD000:
            //Dxyn - DRW Vx, Vy, nibble
            break;
       case 0xE000:
            switch (opcode & 0x00FF){
                case 0x009E:
                    //Ex9E - SKP Vx
                    break;
                case 0x00A1:
                    //ExA1 - SKNP Vx
                    break;
            }
       case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0007:
                    //Fx07 - LD Vx, DT
                    break;
                case 0x000A:
                    //Fx0A - LD Vx, K
                    break;
                case 0x0015:
                    //Fx15 - LD DT, Vx
                    break;
                case 0x0018:
                    //Fx18 - LD ST, Vx
                    break;
                case 0x001E:
                    //Fx1E - ADD I, Vx
                    break;
                case 0x0029:
                    //Fx29 - LD F, Vx
                    break;
                case 0x0033:
                    //Fx33 - LD B, Vx
                    break;
                case 0x0055:
                    //Fx55 - LD [I], Vx
                    break;
                case 0x0065:
                    //Fx65 - LD Vx, [I]
                    break;
            }
    }

}
