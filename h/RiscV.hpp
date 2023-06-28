#ifndef RISCV_HPP
#define RISCV_HPP

#include "../lib/hw.h"

class RiscV {
 public:

  // pop sstatus.spp and sstatus.spie bits (has to be a non inline function)
  static void popSppSpie();

  // read register scause
  static uint64 r_scause();

  // write register scause
  static void w_scause(uint64 scause);

  // read register sepc
  static uint64 r_sepc();

  // write register sepc
  static void w_sepc(uint64 sepc);

  // read register stvec
  static uint64 r_stvec();

  // write register stvec
  static void w_stvec(uint64 stvec);

  // read register stval
  static uint64 r_stval();

  // write register stval
  static void w_stval(uint64 stval);

  enum BitMaskSip {
    SIP_SSIE = (1 << 1),
    SIP_STIE = (1 << 5),
    SIP_SEIE = (1 << 9),
  };

  // mask set register sip
  static void ms_sip(uint64 mask);

  // mask clear register sip
  static void mc_sip(uint64 mask);

  // read register sip
  static uint64 r_sip();

  // write register sip
  static void w_sip(uint64 sip);

  enum BitMaskSstatus {
    SSTATUS_SIE = (1 << 1),
    SSTATUS_SPIE = (1 << 5),
    SSTATUS_SPP = (1 << 8),
  };

  // mask set register sstatus
  static void ms_sstatus(uint64 mask);

  // mask clear register sstatus
  static void mc_sstatus(uint64 mask);

  // read register sstatus
  static uint64 r_sstatus();

  // write register sstatus
  static void w_sstatus(uint64 sstatus);

  //read register a0
  static uint64 r_a0();

  //write register a0
  static void w_a0(uint64 val);

  //read register a1
  static uint64 r_a1();

  //write register a1
  static void w_a1(uint64 val);

  //read register a2
  static uint64 r_a2();

  //write register a2
  static void w_a2(uint64 val);

  //read register a3
  static uint64 r_a3();

  //write register a3
  static void w_a3(uint64 val);

  //read register a4
  static uint64 r_a4();

  //write register a0
  static void w_a4(uint64 val);

  static void ecall();
};

inline uint64 RiscV::r_scause() {
  volatile uint64 scause;
  __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
  return scause;
}

inline void RiscV::w_scause(uint64 scause) {
  __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::r_sepc() {
  uint64 volatile sepc;
  __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
  return sepc;
}

inline void RiscV::w_sepc(uint64 sepc) {
  __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::r_stvec() {
  uint64 volatile stvec;
  __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
  return stvec;
}

inline void RiscV::w_stvec(uint64 stvec) {
  __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::r_stval() {
  uint64 volatile stval;
  __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
  return stval;
}

inline void RiscV::w_stval(uint64 stval) {
  __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscV::ms_sip(uint64 mask) {
  __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask) {
  __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sip() {
  uint64 volatile sip;
  __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
  return sip;
}

inline void RiscV::w_sip(uint64 sip) {
  __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sstatus(uint64 mask) {
  __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask) {
  __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sstatus() {
  uint64 volatile sstatus;
  __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
  return sstatus;
}

inline void RiscV::w_sstatus(uint64 sstatus) {
  __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

inline uint64 RiscV::r_a0() {
  uint64 volatile val;
  __asm__ volatile ("mv %[dst], a0":[dst]"=r"(val));
  return val;
}

inline void RiscV::w_a0(uint64 val) {
  __asm__ volatile ("mv a0, %[src]": : [src]"r"(val));
}

inline uint64 RiscV::r_a1() {
  uint64 volatile val;
  __asm__ volatile ("mv %[dst], a1":[dst]"=r"(val));
  return val;
}

inline void RiscV::w_a1(uint64 val) {
  __asm__ volatile ("mv a1, %[src]": : [src]"r"(val));
}

inline uint64 RiscV::r_a2() {
  uint64 volatile val;
  __asm__ volatile ("mv %[dst], a2":[dst]"=r"(val));
  return val;
}

inline void RiscV::w_a2(uint64 val) {
  __asm__ volatile ("mv a2, %[src]": : [src]"r"(val));
}

inline uint64 RiscV::r_a3() {
  uint64 volatile val;
  __asm__ volatile ("mv %[dst], a3":[dst]"=r"(val));
  return val;
}

inline void RiscV::w_a3(uint64 val) {
  __asm__ volatile ("mv a3, %[src]": : [src]"r"(val));
}

inline uint64 RiscV::r_a4() {
  uint64 volatile val;
  __asm__ volatile ("mv %[dst], a4":[dst]"=r"(val));
  return val;
}

inline void RiscV::w_a4(uint64 val) {
  __asm__ volatile ("mv a4, %[src]": : [src]"r"(val));
}

inline void RiscV::ecall() {
  __asm__ volatile ("ecall");
}
#endif //RISCV_HPP
