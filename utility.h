#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdexcept> // std::runtime_error
#include <sstream>   // std::stringstream
using namespace std;
typedef struct _tlv
{

    int length;
    char *tag;
    char *value;
    struct _tlv *child;
    struct _tlv *next;
} TLV;
TLV *get_node_tlv_quad(int curr, char *cmd);
TLV *get_node_tlv_double(int curr, char *cmd);

TLV *parse_tlv(char *cmd);
int Hextodec(char *hexVal);
char *substring(char *string, int position, int length);
void print_node(TLV *node, int level);
void append(TLV **head_ref, TLV *node);
void create();
bool search(string filename, char *tag);

#endif