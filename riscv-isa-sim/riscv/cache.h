#ifndef _RISCV_CACHE_H
#define _RISCV_CACHE_H

#include "memtracer.h"
#include "common.h"
#include "decode.h"
#include "simif.h"
#include "processor.h"
#include <cstring>
#include <string>
#include <map>
#include <cstdint>
#include <vector>
#include <utility>
#include <cstdlib>
#include <string>


class cache_t
{
 private:
  reg_t number_of_hits;
  reg_t number_of_requests;
  reg_t element_mask;
  reg_t set_mask;
  reg_t tag_mask;
  std::string name;
  uint8_t tag_size_in_bytes;
  uint8_t number_of_sets; 
  uint8_t number_of_elements;
  uint8_t number_of_lines;
  std::vector<uint8_t> cache; //how to realize cache?
  uint8_t element_bits;
  uint8_t set_bits;
  std::vector<int> age;
  std::vector<bool> dirty;
  void store_from_cache(uint8_t* addr_in_cache, uint8_t* addr_in_mem);
  uint8_t* search(reg_t addr, reg_t len, bool mode);
  simif_t* sim;
  bool mode;
 public:
  bool prefetch_on_miss(reg_t addr, uint8_t* where);
  cache_t(uint8_t sets, uint8_t lines, uint8_t line_size, std::string new_name, simif_t* new_sim);
  bool load_to_proc(uint8_t* where, reg_t addr, reg_t len);//how to control len?
  bool store_from_proc(reg_t addr, const uint8_t* from, reg_t len);
  void load_to_cache(reg_t addr, uint8_t* data);
  std::string get_name();
  bool is_active();
};

#endif
