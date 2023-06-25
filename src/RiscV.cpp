//
// Created by marko on 20.4.22..
//

#include "../h/RiscV.hpp"

void RiscV::popSppSpie() {
  __asm__ volatile ("csrw sepc, ra");
  __asm__ volatile ("sret");
}