// Example sketch which shows how to display some patterns
// on a 64x32 LED matrix
//

#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"


#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 64     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 2      // Total number of panels chained one to another
 
//MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8



void setup() {
  Serial.begin(115000);
  // Module configuration
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );

  mxconfig.gpio.e = 32;
  mxconfig.clkphase = false;
  mxconfig.driver = HUB75_I2S_CFG::FM6124;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(90); //0-255
  dma_display->clearScreen();
  dma_display->fillScreen(dma_display->color444(0, 0, 0));

  //drawText(0);

}
void rect1(int x0,int y0,int len,int wd, int r, int g, int b)
{
  dma_display->drawRect(x0,y0,wd,len,dma_display->color444(r, g, b));
}

void line(int x0, int y0, int x2, int y2, int r, int g, int b)
{
  dma_display->drawLine(x0, y0, x2, y2, dma_display->color444(r, g, b));
}

void trapezium(int xlu,int ylu,int xru,int yru, int xld, int yld, int xrd, int yrd, int r, int g, int b)
{
  line(xlu, ylu, xru, yru, r, g, b);
  line(xru, yru, xrd, yrd, r, g, b);
  line(xld, yld, xrd, yrd, r, g, b);
  line(xlu, ylu, xld, yld, r, g, b);
}

void alu(int x, int y, int r, int g, int b)
{
    line(x, y, x+4, y+4, r, g, b);
    line(x+4, y+4, x+4, y+8, r, g, b);
    line(x+4, y+8, x, y+12, r, g, b);
    line(x, y+12, x, y+8, r, g, b);
    line(x, y+8, x+2, y+6, r, g, b);
    line(x+2, y+6, x, y+4, r, g, b);
    line(x, y+4, x, y, r, g, b);
}

void mux(int x, int y, int r, int g, int b)
{
  dma_display->drawLine(x, y, x+2, y+1, dma_display->color444(r, g, b));
  dma_display->drawLine(x+2, y+1, x+2, y+6, dma_display->color444(r, g, b));
  dma_display->drawLine(x, y+7, x+2, y+6, dma_display->color444(r, g, b));
  dma_display->drawLine(x, y, x, y+7, dma_display->color444(r, g, b));
  
  //trapezium(x, y, x+2, y+1, x, y+7, x+2, y+6);
}


void ctrlUnit(int r, int g, int b){ rect1(39, 3, 16, 7, r, g, b);} //Control unit}
void cond(int r, int g, int b){ line(32, 5, 39, 5, r, g, b);} //Cond
void op(int r, int g, int b){ line(32, 8, 39, 8, r, g, b);} //Op
void funct(int r, int g, int b){ line(32, 11, 39, 11, r, g, b);} //Funct
void rd(int r, int g, int b){ line(32, 14, 39, 14, r, g, b);} //Rd

void aluFlags(int r, int g, int b)
{
   line(36, 16, 39, 16, r, g, b); //ALU-flags
   line(36, 16, 36, 20, r, g, b); //ALU-flags
   line(36, 20, 91, 20, r, g, b); //ALU-flags
   line(91, 21, 91, 32, r, g, b); //ALU-flags
   line(91, 32, 89, 32, r, g, b); //ALU-flags
}
void clk(int r, int g, int b) {rect1(10, 27, 7, 4, r, g, b);}
void pc2clk(int r, int g, int b)
{
   line(9, 30, 10, 30, r, g, b); //PC to Clock
}
void PCplus4Alu(int r, int g, int b){alu(17, 44, r, g, b);} //(PC + 4) ALU
void clk2PCplus4Alu(int r, int g, int b)
{
    line(17, 46, 15, 46, r, g, b); //Clock output to (PC + 4) ALU up input
    line(15, 30, 15, 46, r, g, b); //Clock output to (PC + 4) ALU up input
}
void clk2instMem(int r, int g, int b)
{
  line(14, 30, 16, 30, r, g, b); //Clock to Instruction Memory
}
void instMem(int r, int g, int b)
{
  rect1(17, 24, 16, 13, r, g, b); //Clock to Instruction Memory
}
void instMem2bus(int r, int g, int b)
{
   line(30, 33, 31, 33, r, g, b); //Instruction memory to Bus
}
void bus(int r, int g, int b)
{
  line(31, 4, 31, 57, r, g, b); //Bus
}

void muxA1(int r, int g, int b)
{
  trapezium(38, 23, 40, 25, 38, 29, 40, 27, r, g, b); //A1 MUX
}

void muxA2(int r, int g, int b)
{
  trapezium(41, 30, 43, 32, 41, 36, 43, 34, r, g, b); //A2 MUX
}

void bus2muxA1(int r, int g, int b){line(37, 24, 32, 24, r, g, b);} //Bus 2 MUX A1 up
void bus2muxA2up(int r, int g, int b){line(40, 31, 32, 31, r, g, b);}//Bus 2 MUX A2 up
void bus2reg(int r, int g, int b)
{
    line(31, 38, 50, 38, r, g, b);
    line(40, 35, 38, 35, r, g, b); //MUX A2 down
    line(38, 36, 38, 38, r, g, b); 
}
void muxA12reg(int r, int g, int b){ line(40, 26, 50, 26, r, g, b); }
void muxA22reg(int r, int g, int b){line(43, 33, 50, 33, r, g, b);}
void regSrc2muxA1(int r, int g, int b)
{
  line(42, 18, 42, 22, r, g, b);
  line(39, 24, 39, 22, r, g, b); //RegSrc shorted to MUX A1 selection
  line(39, 22, 41, 22, r, g, b); //RegSrc shorted to MUX A1 selection
}
void regSrc2muxA2(int r, int g, int b){line(42, 31, 42, 19, r, g, b);}
void reg(int r, int g, int b){ rect1(50, 24, 24, 17, r, g, b);} //Register file
void regWrite(int r, int g, int b)
{
    line(58, 17, 58, 23, r, g, b); //RegWrite
    line(46, 17, 58, 17, r, g, b); 
}
void reg2alu(int r, int g, int b){line(67, 29, 85, 29, r, g, b);}
void muxAfterReg(int r, int g, int b){trapezium(76, 34, 78, 36, 76, 40, 78, 38, r, g, b);}
void mux2alu(int r, int g, int b){line(85, 37, 78, 37, r, g, b);} 
void extend(int r, int g, int b){ trapezium(52, 56, 57, 51, 52, 59, 57, 59, r, g, b);}
void bus2extend(int r, int g, int b){ line(31, 57, 52, 57, r, g, b);} //Bus to Extend
void immsrc2extend(int r, int g, int b)
{
    line(55, 50, 55, 53, r, g, b); //ImmSrc
    line(68, 50, 55, 50, r, g, b); //ImmSrc
    line(68, 15, 68, 50, r, g, b); //ImmSrc
    line(45, 15, 68, 15, r, g, b); //
}

void alu2dataMem_dataMem(int r, int g, int b)
{
  line(89, 33, 97, 33, r, g, b); //ALU to Data memory
   rect1(97, 24, 24, 14, r, g, b); //Data memory
}
void dataMux2PCMux(int r, int g, int b)
{
    line(2, 28, 6, 28, r, g, b); //MUX up input connected to output of MUX after Data Memory
    line(2, 29, 2, 61, r, g, b); //MUX up input connected to output of MUX after Data Memory
    line(2, 62, 123, 62, r, g, b); //MUX up input connected to output of MUX after Data Memory
    line(123, 61, 123, 37, r, g, b); //MUX up input connected to output of MUX after Data Memory
    line(122, 37, 119, 37, r, g, b); //MUX up input connected to output of MUX after Data Memory

}
void dataMux2wd3(int r, int g, int b)
{
    line(122, 37, 119, 37, r, g, b);
    line(123, 61, 123, 37, r, g, b);
    line(48, 62, 123, 62, r, g, b);
    line(48, 61, 48, 42, r, g, b);
    line(48, 42, 50, 42, r, g, b);
    
}
void PCplus82PCMux(int r, int g, int b)
{
    line(4, 32, 6, 32, r, g, b); //MUX down input connected to output of (PC + 😎 ALU
    line(4, 33, 4, 60, r, g, b); //MUX down input connected to output of (PC + 😎 ALU
    line(4, 60, 46, 60, r, g, b); //MUX down input connected to output of (PC + 😎 ALU 
    line(46, 59, 46, 46, r, g, b); //MUX down input connected to output of (PC + 😎 ALU
}
void PCSrc(int r, int g, int b)
{
    line(7, 2, 7, 28, r, g, b); //PCSrc
    line(7, 1, 47, 1, r, g, b); //PCSrc
    line(47, 2, 47, 4, r, g, b); //PCSrc
    line(46, 4, 45, 4, r, g, b); //PCSrc
}
void PCMux(int r, int g, int b){trapezium(6, 27, 8, 29, 6, 33, 8, 31, r, g, b);} //MUX
void dataMemAlu(int r, int g, int b){ alu(85, 27, r, g, b);} //Rightmost ALU
void memWrite(int r, int g, int b)
{
    line(104, 9, 104, 24, r, g, b); //MemWrite
    line(45, 9, 104, 9, r, g, b); //MemWrite
}
void mem2reg(int r, int g, int b)
{
    line(45, 7, 118, 7, r, g, b); //MemtoReg
    line(118, 7, 118, 35, r, g, b); //MemtoReg
}
void muxAfterDataMem(int r, int g, int b){ trapezium(117, 34, 119, 36, 117, 40, 119, 38, r, g, b);} //MUX afte Data memory
void reg2dataMem(int r, int g, int b)
{
    line(70, 35, 70, 42, r, g, b); //Register file output to Data memory input
    line(70, 42, 97, 42, r, g, b); //Register file output to Data memory input
}
void alub4dataMem2muxAfterDataMem(int r, int g, int b)
{
    line(94, 33, 94, 50, r, g, b); //ALU output to MUX after data memory input
    line(94, 50, 113, 50, r, g, b); //ALU output to MUX after data memory input
    line(113, 50, 113, 39, r, g, b); //ALU output to MUX after data memory input
    line(113, 39, 117, 39, r, g, b); //ALU output to MUX after data memory input
}
void dataMem2mux(int r, int g, int b){ line(117, 35, 111, 35, r, g, b);} //Data memory to MUX up input
void aluSrc(int r, int g, int b)
{
    line(45, 13, 77, 13, r, g, b); //ALUSrc
    line(77, 13, 77, 35, r, g, b); //ALUSrc
}
void aluCtrl(int r, int g, int b)
{
    line(45, 11, 87, 11, r, g, b); //ALUControl
    line(87, 11, 87, 29, r, g, b); //ALUControl
}
void mux2extend(int r, int g, int b)
{
    line(72, 39, 72, 56, r, g, b); //MUX down input connected to Extend (ExtImm)
    line(72, 56, 57, 56, r, g, b); //MUX down input connected to Extend (ExtImm)
    line(72, 39, 75, 39, r, g, b); //MUX down input connected to Extend (ExtImm)
}
void reg2mux(int r, int g, int b){line(75, 35, 67, 35, r, g, b);}
void pcPlus8alu(int r, int g, int b){alu(38, 40, r, g, b);}
void pcPlus8alu2reg(int r, int g, int b){line(42, 46, 50, 46, r, g, b);}
void pcPlus42pcPlus8(int r, int g, int b){line(21, 50, 38, 50, r, g, b);}
    
void initialSetup()
{
  //Control Unit
    ctrlUnit(0,15,0);
    cond(0,15,0);
    op(0,15,0);
    funct(0,15,0);
    rd(0,15,0);
    aluFlags(0,15,0);
    
    //Clock
    clk(0,15,0); //Chotto rect
    pc2clk(0,15,0);//PC to clock
    
    //alu(17, 44, 15, 0, 0); //(PC + 4) ALU
    PCplus4Alu(0,15,0);
    clk2PCplus4Alu(0,15,0); //Clock output to (PC + 4) ALU up input
    line(17, 54, 15, 54, 0, 15, 0); //(PC + 4) ALU down input

    
    //MUX and PC
 
    dataMux2PCMux(0,15,0);
    PCplus82PCMux(0,15,0);
    PCSrc(0,15,0);
    PCMux(0,15,0);
 


    //Instruction Memory
    clk2instMem(0, 15, 0); //Clock to Instruction Memory
    instMem(0, 15, 0); //Instruction Memory
    
    bus(0,15,0); //Bus
    //Register File
    instMem2bus(0,15,0); //Instruction memory to Bus
    reg(0,15,0); //Register file
    
    
    muxA1(0,15,0); //A1 MUX
    muxA2(0,15,0); //A2 MUX
    
    
    regSrc2muxA2(0,15,0); //RegSrc to A2 Mux
    regSrc2muxA1(0,15,0); //RegSrc shorted to MUX A1 selection
    
    
    
    bus2muxA1(0,15,0); //Bus 2 MUX A1 up
    line(37, 28, 36, 28, 0, 15, 0); //MUX A1 down
    
  
   
    bus2muxA2up(0,15,0); //Bus 2 MUX A2 up
   
  
    muxA12reg(0,15,0);//MUX A1 to Register file
    muxA22reg(0,15,0);//MUX A2 to Register file
    bus2reg(0,15,0); //Bus to Register file
    

   //PC+8 ALU
    pcPlus8alu(0,15,0); //(PC + 8) ALU
    pcPlus8alu2reg(0,15,0); //ALU to Register file
    line(38, 42, 36, 42, 0, 15, 0); //ALU input up
    pcPlus42pcPlus8(0,15,0); //(PC + 8) ALU input down connected to (PC + 4) ALU
    regWrite(0,15,0);//RegWrite
    

    //ALU
    dataMemAlu(0,15,0);
    reg2alu(0,15,0);// ALU up input
    mux2alu(0,15,0);//ALU down input connected to MUX
    muxAfterReg(0,15,0); //MUX before ALU
    reg2mux(0,15,0);//MUX up input
    //line(72, 39, 67, 39, 15, 0, 0); //MUX down input
    mux2extend(0,15,0);
    aluSrc(0,15,0);
    aluCtrl(0,15,0);

    //Extend
    extend(0,15,0);
    immsrc2extend(0,15,0); //ImmSrc
    bus2extend(0,15,0);
    

    //Data Memory
    alu2dataMem_dataMem(0, 15, 0);// ALU to Data Memory and Data Memory
    memWrite(0,15,0);
    muxAfterDataMem(0,15,0);
    mem2reg(0,15,0);
    reg2dataMem(0,15,0);
    alub4dataMem2muxAfterDataMem(0,15,0);
    dataMem2mux(0,15,0);

    dataMux2wd3(0,15,0);
    
}
void LDR()
{
  clk(15,0,0);
    clk2instMem(15,0,0);
    delay(4000);
    clk(0,15,0);
    
    instMem(15,0,0);
    instMem2bus(15,0,0);
    delay(4000);
    instMem2bus(0,15,0);
    clk2instMem(0,15,0);
    instMem(0,15,0);

    ctrlUnit(15,0,0);
    op(15,0,0);
    bus(15,0,0);
    regSrc2muxA1(15,0,0);
    muxA1(15,0,0);
    delay(4000);
    op(0,15,0);
    bus(0,15,0);
    regSrc2muxA1(0,15,0);
    muxA1(0,15,0);
    ctrlUnit(0,15,0);
  
    muxA12reg(15,0,0);
    reg(15,0,0);
    reg2alu(15,0,0);// to off
    
    delay(4000);
    muxA12reg(0,15,0);
    reg(0,15,0);


    ctrlUnit(15,0,0);
    immsrc2extend(15,0,0); //ImmSrc
    delay(2000);
    extend(15,0,0);
    delay(2000);
    mux2extend(15,0,0);
    muxAfterReg(15,0,0);//to off
    delay(2000);
    mux2alu(15,0,0);
    dataMemAlu(15,0,0);
    delay(4000);
    immsrc2extend(0,15,0); //ImmSrc
    extend(0,15,0);
    mux2extend(0,15,0);
    mux2alu(0,15,0);
    dataMemAlu(0,15,0);
    reg2alu(0,15,0);
    muxAfterReg(0,15,0);

    
    alu2dataMem_dataMem(15,0,0);
    mem2reg(15,0,0);
    dataMem2mux(15,0,0);
    muxAfterDataMem(15,0,0);
    delay(4000);
    alu2dataMem_dataMem(0,15,0);
    mem2reg(0,15,0);
    dataMem2mux(0,15,0);
    muxAfterDataMem(0,15,0);
    ctrlUnit(0,15,0);
    

    dataMux2wd3(15,0,0);  
}
void loop() {


    
    
    initialSetup();
    LDR();
    
    delay(4000);
    
    
   
}
