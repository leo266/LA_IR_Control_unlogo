#include "pxt.h"
#include <map>
#include <vector>
#include "ReceiverIR.h"
using namespace pxt;
typedef vector<Action> vA;

enum class Pins{
  P0=  3,
  P1=  2,
  P2=  1,
  P3=  4,
  P4=  5,
  P5=  17,
  P6=  12,
  P7=  11,
  P8=  18,
  P9=  10,
  P10= 6,
  P11= 26,
  P12= 20,
  P13= 23,
  P14= 22,
  P15= 21,
  P16= 16,
  P19= 0,
  P20= 30
};

enum class RemoteButton {
      /*//CH_MINUS = 0x45,
	  UP = 0x46,
	  //CH_Add = 0x47,
	  LEFT = 0x44,
	  RIGHT = 0x43,
	  NUM3 = 0xd,
	  OK = 0x40,
	  NUM2 = 0x19,
	  //Minus = 0x7,
	  DOWN = 0x15,
	  //EQ = 0x9,
	  NUM1 = 0x16,
	  NUM4 = 0xc,
	  NUM5 = 0x18,
	  NUM6 = 0x5e,
	  NUM7 = 0x8,
	  NUM8 = 0x1c,
	  NUM9 = 0x5a,
	  STAR= 0x42,
	  NUM0 = 0x52,
	  GRID= 0x4A*/
	  
	  NUM1 = 0x45,
	  NUM2 = 0x46,
	  NUM3 = 0x47,
	  NUM4 = 0x44,
          NUM6 = 0x43,
	  GRID = 0xd,
	  NUM5 = 0x40,
	  NUM0 = 0x19,
	  NUM7 = 0x7,
	  NUM8 = 0x15,
	  NUM9 = 0x9,
	  STAR = 0x16,
	  //NUM1 = 0xc,
	  UP = 0x18,
	  //NUM3 = 0x5e,
	  LEFT = 0x8,
	  OK = 0x1c,
	  RIGHT = 0x5a,
	  //NUM7 = 0x42,
	  DOWN = 0x52,
          //NUM9 = 0x4A,
};

//% color=50 weight=19
//% icon="\uf1eb"
namespace Mbit_IR { 
  map<RemoteButton, vA> actions;
  map<RemoteButton, uint32_t> lastact;
  Timer tsb; 
  uint8_t buf[32];
  uint32_t now;
  ReceiverIR *rx;
  RemoteIR::Format fmt = RemoteIR::UNKNOWN;

  /**
  * button pushed.
  */
  //% blockId=ir_received_left_event
  //% block="on |%btn| button pressed"
  void onPressEvent(RemoteButton btn, Action body) {
    //if(actions.find(btn) == actions.end()) actions[btn] = new vector();
    actions[btn].push_back(body);
  }

  void cA(vA runner){for(int i=0;i<runner.size();i++){runAction0(runner[i]);} }

  void onReceivable(){
    int x = rx->getData(&fmt, buf, 32 * 8);
    if(actions.find((RemoteButton)buf[2]) == actions.end()) return;
    now = tsb.read_ms();
    if(now - lastact[(RemoteButton)buf[2]] < 100) return;
    lastact[(RemoteButton)buf[2]] = now;
    cA(actions[(RemoteButton)buf[2]]); 
  }

  void monitorIR(){
    while(1){
      while(rx->getState() != ReceiverIR::Received){ 
        uBit.sleep(50);
      }
      onReceivable();
    }
  }

  /**
  * initialises local variablesssss
  */
  //% blockId=ir_init
  //% block="connect ir receiver to %pin"
  void init(Pins pin){
    rx = new ReceiverIR((PinName)pin);
    tsb.start(); //interrupt timer for debounce
    create_fiber(monitorIR);
  }
}
