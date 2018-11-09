//
// Created by minggang on 2018/11/9.
//

#ifndef MOTAN_LUA_UTILS_H
#define MOTAN_LUA_UTILS_H

#include <stdlib.h>

extern int get_local_ip(char *ifname, char *ip);

extern int get_request_id_bytes(const char *request_id_str, char *rs_bytes);

extern char *itoa(u_int64_t value, char *result, int base);

extern int get_request_id(uint8_t bytes[8], char *request_id_str);

#endif //MOTAN_LUA_UTILS_H
