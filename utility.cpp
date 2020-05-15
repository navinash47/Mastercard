#include <string>
#include <iostream>
#include <fstream>
#include <sstream> // std::stringstream
using namespace std;
typedef struct _tlv
{

    int length;
    string tag;
    string value;
    struct _tlv *child;
    struct _tlv *next;
} TLV;

//functions
TLV *get_node_tlv_quad(int curr, string cmd, int length, string tag_four);
TLV *get_node_tlv_double(int curr, string cmd, int length, string tag_two);
TLV *parse_tlv(string cmd);
int Hextodec(string hexVal);
void print_node(TLV *node, int level);
bool search(string tag);

string rapdu[] = {"42", "4F", "57", "50", "5A", "5F20", "5F24", "5F25", "5F28", "5F2A", "5F2D", "5F30", "5F34", "5F36", "5F50", "5F54", "5F55", "5F53",
                  "5F56",
                  "5F57",
                  "61",
                  "6F",
                  "70",
                  "71",
                  "72",
                  "73",
                  "77",
                  "80",
                  "81",
                  "82",
                  "83",
                  "84",
                  "86",
                  "87",
                  "88",
                  "89",
                  "8A",
                  "8C",
                  "8D",
                  "8E",
                  "8F",
                  "90",
                  "91",
                  "92",
                  "93",
                  "94",
                  "95",
                  "97",
                  "98",
                  "99",
                  "9A",
                  "9B",
                  "9C",
                  "9D",
                  "9F01",
                  "9F02",
                  "9F03",
                  "9F04",
                  "9F05",
                  "9F06",
                  "9F07",
                  "9F08",
                  "9F09",
                  "9F0B",
                  "9F0D",
                  "9F0E",
                  "9F0F",
                  "9F10",
                  "9F11",
                  "9F12",
                  "9F13",
                  "9F14",
                  "9F15",
                  "9F16",
                  "9F17",
                  "9F18",
                  "9F1A",
                  "9F1B",
                  "9F1C",
                  "9F1D",
                  "9F1E",
                  "9F1F",
                  "9F20",
                  "9F21",
                  "9F22",
                  "9F23",
                  "9F26",
                  "9F27",
                  "9F2D",
                  "9F2E",
                  "9F2F",
                  "9F32",
                  "9F33",
                  "9F34",
                  "9F35",
                  "9F36",
                  "9F37",
                  "9F38",
                  "9F39",
                  "9F3A",
                  "9F3B",
                  "9F3C",
                  "9F3D",
                  "9F40",
                  "9F41",
                  "9F42",
                  "9F43",
                  "9F44",
                  "9F45",
                  "9F46",
                  "9F47",
                  "9F48",
                  "9F49",
                  "9F4A",
                  "9F4B",
                  "9F4C",
                  "9F4D",
                  "9F4E",
                  "9F4F",
                  "A5",
                  "BF0C"};
using namespace std;

TLV *parse_tlv(string cmd)
{
    int curr = 0;
    TLV *head = NULL;
    while (cmd.length() > curr)
    {
        string tag_four, tag_two, len;
        int Lval;

        tag_four = cmd.substr(curr, 4);
        tag_two = cmd.substr(curr, 2);

        if (search(tag_four))
        {

            // cout << "tagfour" << tag_four << endl;
            curr += 4;
            if (cmd[curr] == '8' && cmd[curr + 1] == '1')
            {
                curr += 2;
                len = cmd.substr(curr, 2);
                Lval = Hextodec(len);
                curr += 2;
            }
            else if (cmd[curr] == '8' && cmd[curr + 1] == '2')
            {
                curr += 2;
                len = cmd.substr(curr, 4);
                Lval = Hextodec(len);
                curr += 4;
            }
            else
            {
                len = cmd.substr(curr, 2);
                Lval = Hextodec(len);
                curr += 2;
            }
            TLV *node = get_node_tlv_quad(curr, cmd, Lval, tag_four);
            node->child = parse_tlv(node->value);
            // cout << node->value << endl;
            TLV *last = head;

            if (head == NULL)
            {
                head = node;
            }
            else
            {
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = node;
            }

            curr += node->length;
            curr += node->length;
        }
        else if (search(tag_two))
        {
            curr += 2;
            if (cmd[curr] == '8' && cmd[curr + 1] == '1')
            {
                curr += 2;
                len = cmd.substr(curr, 2);
                Lval = Hextodec(len);
                curr += 2;
            }
            else if (cmd[curr] == '8' && cmd[curr + 1] == '2')
            {
                curr += 2;
                len = cmd.substr(curr, 4);
                Lval = Hextodec(len);
                curr += 4;
            }
            else
            {
                len = cmd.substr(curr, 2);
                Lval = Hextodec(len);
                curr += 2;
            }

            TLV *node = get_node_tlv_double(curr, cmd, Lval, tag_two);
            node->child = parse_tlv(node->value);
            // cout << node->value << endl;
            TLV *last = head;

            if (head == NULL)
            {
                head = node;
            }
            else
            {
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = node;
            }

            curr += node->length;
            curr += node->length;
        }
        else
        {
            break;
        }
    }
    return head;
}

int Hextodec(string hexVal)
{
    int len = hexVal.length();
    int base = 1;
    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        if (hexVal[i] >= '0' && hexVal[i] <= '9')
        {
            dec_val += (hexVal[i] - 48) * base;
            base = base * 16;
        }

        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
        {
            dec_val += (hexVal[i] - 55) * base;
            base = base * 16;
        }
    }

    return dec_val;
}

TLV *get_node_tlv_double(int curr, string cmd, int length, string tag_two)
{
    TLV *node = new TLV;
    node->length = length;
    node->tag = tag_two;
    node->next = NULL;
    node->value = cmd.substr(curr, 2 * length);

    return node;
}

TLV *get_node_tlv_quad(int curr, string cmd, int length, string tag_four)
{
    TLV *node = new TLV;
    node->length = length;
    node->tag = tag_four;
    node->next = NULL;
    node->value = cmd.substr(curr, 2 * length);

    return node;
}

void print_node(TLV *node, int level)
{
    TLV *last = node;
    if (last == NULL)
        return;
    cout << "\n";
    for (int i = 0; i < level; i++)
    {
        cout << "->";
    }
    while (last != NULL)
    {
        cout << "Tag:" << last->tag;
        cout << ", Length:" << last->length;
        // cout << ", Value:" << last->value << ";\t";
        print_node(last->child, level + 1);
        last = last->next;
    }
    cout << endl;
}

bool search(string tag)
{
    bool flg = false;
    // cout << "rapdusize:" << sizeof(rapdu) / sizeof(rapdu[0]) << endl;
    for (int i = 0; i < sizeof(rapdu) / sizeof(rapdu[0]); i++)
    {
        if (tag.compare(rapdu[i]) == 0)
        {
            flg = true;
            // cout << "Tag: " << rapdu[i] << "\n";
            break;
        }
    }
    return flg;
}
