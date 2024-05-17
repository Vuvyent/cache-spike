#include "cache.h"
#include "common.h"
#include "simif.h"
#include "processor.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <cstdlib>
#include <string>


cache_t::cache_t(uint8_t sets, uint8_t lines, uint8_t line_size, std::string new_name, simif_t* new_sim)
{
 number_of_requests = 0;
 number_of_hits = 0;
 mode = true;
 name = new_name;
 number_of_sets = sets;
 number_of_lines = lines;
 number_of_elements = line_size;
 sim = new_sim;
 
 
 element_bits = 0;
 line_size-=(uint8_t) 1;
 //std::cout<<"line_size - 1 is: "<< (int) line_size<<"\n";
 element_mask = 1;
 while(line_size > 0)
 {
  element_bits++;
  line_size>>=1;
  if(line_size > 0)
  {
   element_mask<<=1;
   element_mask|=1;
  }
 }
 //std::cout<<"line size in bits: " << (int) element_bits << "\n" << "element mask: " << (int) element_mask <<"\n";
 
 
 sets-=1;
 set_bits = 0;
 set_mask = 1;
  while(sets > 0)
 {
  set_bits++;
  sets>>=1;
  if(sets>0)
  {
   set_mask<<=1;
   set_mask|=1;
  }
 }
 //std::cout<<"sets number in bits: " << (int) set_bits << "\n" << "set mask: " << (reg_t) set_mask <<"\n";
 
 tag_size_in_bytes = sizeof(reg_t) * 8 - element_bits - set_bits;
 tag_mask = 1;
 for(int i = 0; i<tag_size_in_bytes - 1; i++)//tag_size_in_bytes is tag size in bits at this point
 {
  tag_mask<<=1;
  tag_mask|=1;
 }
 if (tag_size_in_bytes%8!=0)
 {
  tag_size_in_bytes/=8;
  tag_size_in_bytes++;
 }
 else
 {
  tag_size_in_bytes/=8;
 }
 //std::cout<<"tag size in bytes is: " << (int) tag_size_in_bytes << "\n" << "tag mask is: " << (reg_t) tag_mask <<"\n";

 
 
 cache.resize(number_of_sets*number_of_lines*number_of_elements+number_of_sets*number_of_lines*tag_size_in_bytes); 
 age.resize(number_of_sets*number_of_lines, 0);
 dirty.resize(number_of_sets*number_of_lines, false);
 //std::cout<<"cache size is: " << cache.size() << "\n";
}


uint8_t* cache_t::search(reg_t addr, reg_t len, bool mode)//0 - load, 1 - store
{
 number_of_requests += 1;
 std::cout<<"cache search is called\n";
 std::cout<<"cache lines age is: ";
 for(std::size_t i = 0; i < age.size(); i++)
 {
  if(age[i]!=0)
  {
   age[i]+=1;
  }
  std::cout<<age[i]<<" ";
 }
 std::cout<<"\n";
 reg_t copy_addr = addr;
 reg_t element = copy_addr&element_mask;
 copy_addr>>=element_bits;
 reg_t set = copy_addr&set_mask;
 copy_addr>>=set_bits;
 std::vector<uint8_t> tag(tag_size_in_bytes);
 for (int i = tag_size_in_bytes - 1; i>=0;i--)
 {
  tag[i] = (uint8_t) copy_addr;
  copy_addr>>=8;
 }
 //std::cout<<"addr is: "<<addr <<"\nelement is: " << element << "\n" << "set is: " << set << "\n" << "tag is: ";
 for(int i = 0; i<tag_size_in_bytes; i++)
 {
  std::cout << (reg_t) tag[i]<<" ";
 }
 std::cout<<"\n";
 //how to do tag comparsion?
 
 int cnt_tag_bytes = 0;
 int begin = (number_of_elements + tag_size_in_bytes)*set*number_of_lines;
 //std::cout<<"begin in search is: "<< begin<<"\n";
 for(int i = 0; i < number_of_lines; i++)
 {
  for(int j=0;j<tag_size_in_bytes;j++)
  {
   int idx = begin + (number_of_elements + tag_size_in_bytes)*i + j;
   //std::cout<<"search idx " << i <<" "<< j <<" is: "<< idx<<" "<< "cache at idx is: " << (reg_t) cache[idx] << "\n";
   uint8_t tag_byte = cache[idx];
   if(tag_byte == tag[j])
   {
    cnt_tag_bytes++;
   }
  }
  if (cnt_tag_bytes == tag_size_in_bytes)
  {
   uint8_t* data = &cache[begin + (number_of_elements + tag_size_in_bytes)*i + tag_size_in_bytes + element];
   age[set*number_of_lines + i] = 1;
   std::cout<<"age if line is found in search: ";
   for(size_t i = 0; i<age.size();i++)
   {
    std::cout<<age[i]<<" ";
   }
   std::cout<<"\n";
   if(mode)
   {
    //std::cout<<"dirty is set";
    dirty[set*number_of_lines + i] = true;
   }
   std::cout<<"value in cache at given address is: ";
   for(int i = 0; i< len; i++)
   {
    std::cout << (reg_t) *(data+i) <<" ";
   }
   std::cout<<"\n";
   //std::cout<<"data adress is: "<< (reg_t) data<<"\n";
   //std::cout<<"data is found\n";
   number_of_hits += 1;
   std::cout<<"number of hits is "<<number_of_hits<<" out of total " << number_of_requests <<"\n";
   return data;
  }
  cnt_tag_bytes = 0;
 }
 std::cout<<"number of hits is "<<number_of_hits<<" out of total " << number_of_requests <<"\n";
 //std::cout<<"data is not found\n";
 return NULL;
}
bool cache_t::load_to_proc(uint8_t* where, reg_t addr, reg_t len)
{
 uint8_t* data = cache_t::search(addr, len, 0);
 
 if(data == NULL)
 {
  std::cout<<"\n\nwhole cache from load_to_proc " << this->name << " is: ";
  for (size_t i = 0; i<cache.size(); i++)
  {
   std::cout<<(reg_t) cache[i]<<" ";
   if(i%(number_of_elements+tag_size_in_bytes) == 0){
   std::cout<<"|| ";
   }
  } 
  std::cout<<"\n\n\n";
  
  //std::cout<<"load_to_proc data is NULL\n";
  return false;
 }
 else
 {
  //std::cout<<"load_to_proc data is: "<<(reg_t) data<<"\n";
  std::memcpy(where, data, len);
  
  std::cout<<"\n\nwhole cache from load_to_proc " << this->name << "  is: ";
  for (size_t i = 0; i<cache.size(); i++)
  {
   std::cout<<(reg_t) cache[i]<<" ";
   if(i%(number_of_elements+tag_size_in_bytes) == 0){
   std::cout<<"|| ";
   }
  } 
  std::cout<<"\n\n\n";
  
  return true;
 }
}

void cache_t::load_to_cache(reg_t addr, uint8_t* data)
{

 //std::cout<<"data at addr before load_to_cache:  ";
 //for(int i = 0;i<64;i++)
 //{
  //std::cout<<(reg_t)*(data+i)<<" ";
 //}
 //std::cout<<"\n";
 
 reg_t copy_addr = addr;
 reg_t element = copy_addr&element_mask;
 //std::cout<<"load_to_cache element number is: "<<element<<"\n";
 copy_addr>>=element_bits;
 reg_t set = copy_addr&set_mask;
 //std::cout<<"load_to_cache set number is: "<<set<<"\n";
 copy_addr>>=set_bits;
 int begin = (number_of_elements + tag_size_in_bytes)*set*number_of_lines;
 //std::cout<<"load_to_cache begin is: "<<begin<<"\n";
 
 //std::cout<<"data at addr before lru_line:  ";
 //for(int i = 0;i<64;i++)
 //{
 // std::cout<<(reg_t)*(data+i)<<" ";
 //}
 //std::cout<<"\n";
 
 uint8_t* lru_line;
 int max_age = 0;
 bool lru_dirty = false;
 bool is_evicted = true;
 int max_age_idx = 0;
 for(int i=0; i<number_of_lines;i++)
 {
  size_t age_idx = set*number_of_lines+i;
  //std::cout<<"age idx at set "<<set<<" line "<<i<<" is: "<< age_idx <<"\n"<<"age at age idx is: "<< age[age_idx]<<"\n";
  if(age[age_idx] == 0)
  {
   //std::cout<<"found not used cache line\n";
   lru_line = &cache[begin + (number_of_elements + tag_size_in_bytes)*i];
   age[age_idx] = 1;
   lru_dirty = false;
   is_evicted = false;
   max_age = 0;
   std::cout<<"age if line in load to cache is zero found: ";
   for(size_t i = 0; i<age.size();i++)
   {
    std::cout<<age[i]<<" ";
   }
   std::cout<<"\n";
   break;
   
  }
  if(age[age_idx] > max_age){
   max_age = age[age_idx];
   max_age_idx = age_idx;
   lru_line = &cache[begin + (number_of_elements + tag_size_in_bytes)*i];
   lru_dirty = dirty[age_idx];
  }
 }
 
 if(max_age > 0)
 {
  age[max_age_idx] = 1;
  std::cout<<"age if line in load to cache replaced: ";
   for(size_t i = 0; i<age.size();i++)
   {
    std::cout<<age[i]<<" ";
   }
   std::cout<<"\n";
 }
 
  //std::cout<<"data at addr after find lru_line:  ";
 //for(int i = 0;i<64;i++)
 //{
  //std::cout<<(reg_t)*(data+i)<<" ";
 //}
 //std::cout<<"\n";
 
 //if(is_evicted)
 //{
  //std::cout<<"line was evicted\n";
 //}
 //else
 //{
  //std::cout<<"free line is used\n";
 //}
 //std::cout<<"load_to_cache lru_line is: " <<(reg_t)lru_line<<" "<<(reg_t)*lru_line<<"\n";
 
 if(lru_dirty)
 {
  //std::cout<<"lru is dirty\n";
  uint8_t* addr_in_cache = lru_line + tag_size_in_bytes;
  //std::cout<<"load_to_cache addr_in_cache: "<<(reg_t)addr_in_cache<<" "<<(reg_t)*addr_in_cache<<"\n";
  reg_t addr_in_mem = 0;
  for(int i=0; i<tag_size_in_bytes; i++)
  {
   addr_in_mem += *(lru_line+i);
   if(i!=tag_size_in_bytes - 1)
   {
    addr_in_mem <<= 8;
   }
  }
  addr_in_mem <<= set_bits;
  addr_in_mem += set;
  addr_in_mem <<= element_bits;
  
  //std::cout<<"load_to_cache addr_in_mem is: "<<addr_in_mem<<"\n";
  //i tried this uint8_t* addr_in_mem_ptr = (uint8_t*) addr_in_mem;
  auto addr_in_mem_ptr = sim->addr_to_mem(addr_in_mem);
  store_from_cache(addr_in_cache,(uint8_t*) addr_in_mem_ptr);/////////////
 }
 
 std::vector<uint8_t> tag(tag_size_in_bytes);
 for (int i = tag_size_in_bytes - 1; i>=0;i--)
 {
  tag[i] = (uint8_t) copy_addr;
  copy_addr>>=8;
 }
 std::cout<<"tag is: ";
 for(std::size_t i =0;i<tag.size();i++)
 {
  std::cout<<(reg_t) tag[i]<<" "; 
 }
 //std::cout<<"\n";
 std::memcpy(lru_line, &tag[0], tag_size_in_bytes); //copy tag in cache
 
 
 copy_addr = addr;
 reg_t line_begin = copy_addr&(~element_mask);
 //std::cout<<"line begins is: "<<line_begin<<"\n";
 reg_t diff = addr - line_begin;
// std::cout<<"diff is: "<<diff<<"\n";
 //std::cout<<"data of line is: ";
 //for(int i = 0; i< number_of_elements; i++)
 //{
  //std::cout<<(reg_t)*(data+i)<<" ";
 //}
 //std::cout<<"\n";
 //std::cout<<"lru_line before memcpy is: ";
 //for(int i = 0; i< number_of_elements; i++)
 //{
  //std::cout<<(reg_t)*(lru_line+tag_size_in_bytes+i)<<" ";
 //}
 //std::cout<<"\n";
 std::memcpy(lru_line+tag_size_in_bytes, (uint8_t*) (data-diff), number_of_elements);//not sure
 //std::cout<<"lru_line after memcpy is: ";
 //for(int i = 0; i< number_of_elements; i++)
 //{
  //std::cout<<(reg_t)*(lru_line+tag_size_in_bytes+i)<<" ";
 //}
 //std::cout<<"\n";
 
 std::cout<<"\n\nwhole cache from load_to_cache " << this->name << " is: ";
 for (size_t i = 0; i<cache.size(); i++)
 {
  std::cout<<(reg_t) cache[i]<<" ";
  if(i%(number_of_elements+tag_size_in_bytes) == 0){
   std::cout<<"|| ";
  }
 } 
 std::cout<<"\n\n\n";
}


void cache_t::store_from_cache(uint8_t* addr_in_cache, uint8_t* addr_in_mem) //maybe need to use sim_t addr_to_mem
{
 std::memcpy(addr_in_mem, addr_in_cache, number_of_elements);
}


bool cache_t::store_from_proc(reg_t addr, const uint8_t* from, reg_t len) //how to do? ask about this
{
 uint8_t* data = cache_t::search(addr, len, 1);
 
 if(data == NULL)
 {
  std::cout<<"\n\nwhole cache from store_from_proc  " << this->name << " is: ";
  for (size_t i = 0; i<cache.size(); i++)
  {
   std::cout<<(reg_t) cache[i]<<" ";
   if(i%(number_of_elements+tag_size_in_bytes) == 0){
    std::cout<<"|| ";
   }
  } 
  std::cout<<"\n\n\n";
 
  //std::cout<<"store from proc data is NULL\n";
  return false;
 }
 else
 {
  //std::cout<<"store_from_proc data is: " << (reg_t) data<<"\n";
  std::memcpy((reg_t*) data, from, len);
  std::cout<<"\n\nwhole cache from store " << this->name << " is: ";
 for (size_t i = 0; i<cache.size(); i++)
 {
  std::cout<<(reg_t) cache[i]<<" ";
  if(i%(number_of_elements+tag_size_in_bytes) == 0){
   std::cout<<"|| ";
  }
 } 
  return true;
 }
}

std::string cache_t::get_name()
{
 return this->name;
}

bool cache_t::is_active()
{
 return mode;
}


bool cache_t::prefetch_on_miss(reg_t addr, uint8_t* from)
{
 reg_t copy_addr = addr;
 std::cout<<"addr in on miss is: "<<copy_addr<<"\n";
 reg_t line_begin = copy_addr&(~element_mask);
 reg_t diff = copy_addr - line_begin;
 std::cout<<"diff in on miss is: "<<diff<<"\n";
 reg_t bytes_to_next_line = (reg_t) 1<<element_bits;
 std::cout<<"bytes_to_next_line in on miss is: "<<bytes_to_next_line<<"\n";
 bytes_to_next_line -= diff;
 std::cout<<"bytes_to_next_line - diff in on miss is: "<<bytes_to_next_line<<"\n";
 uint8_t* copy_from_ptr = from;
 std::cout<<"copy_from_ptr is on miss is: "<<(reg_t) copy_from_ptr<<"\n";
 copy_from_ptr+=bytes_to_next_line;
 std::cout<<"copy_from_ptr+bytes_to_next_line is on miss is: "<<(reg_t) copy_from_ptr<<"\n";
 copy_addr += bytes_to_next_line;
 std::cout<<"copy_addr in on miss is: "<<copy_addr<<"\n";
 
 reg_t copy_addr2 = copy_addr; 
 reg_t set = copy_addr2&set_mask;
 copy_addr2>>=set_bits;
 std::vector<uint8_t> tag(tag_size_in_bytes);
 for (int i = tag_size_in_bytes - 1; i>=0;i--)
 {
  tag[i] = (uint8_t) copy_addr2;
  copy_addr2>>=8;
 }
 
 int cnt_tag_bytes = 0;
 int begin = (number_of_elements + tag_size_in_bytes)*set*number_of_lines;
 std::cout<<"begin in on_miss is: "<< begin<<"\n";
 for(int i = 0; i < number_of_lines; i++)
 {
  for(int j=0;j<tag_size_in_bytes;j++)
  {
   int idx = begin + (number_of_elements + tag_size_in_bytes)*i + j;
   std::cout<<"on_miss idx " << i <<" "<< j <<" is: "<< idx<<" "<< "cache at idx is: " << (reg_t) cache[idx] << "\n";
   uint8_t tag_byte = cache[idx];
   if(tag_byte == tag[j])
   {
    cnt_tag_bytes++;
   }
  }
  
  if (cnt_tag_bytes == tag_size_in_bytes)
  {
   std::cout<<"line already in cache\n";
   return false;
  }
  else
  {
   load_to_cache(copy_addr, copy_from_ptr);
   std::cout<<"line is prefetched\n";
   return true;
  }
 }
}
