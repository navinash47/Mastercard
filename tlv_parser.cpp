#include <iostream>
using namespace std;
int main()
{
    char cmd[20000];
    TLV *tlv = new TLV;
    cout << "Enter tlv data : ";
    cin >> cmd;
    tlv = parse_tlv(cmd);
    print_node(tlv, 0);
    return 0;
}