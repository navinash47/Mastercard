#include "utility.h"
using namespace std;

TLV *parse_tlv(char *cmd)
{
    int curr = 0;
    TLV *head = NULL;
    // if (cmd[curr] == 8)
    // cout << cmd << endl;
    // cout << strlen(cmd) << ">" << curr << endl;
    while (strlen(cmd) > curr)
    {
        // cout << "curr:" << curr << endl;
        // cout << strlen(cmd) << ">" << curr << endl;
        char *tag_four = new char[5];
        char *tag_two = new char[5];

        tag_four[0] = cmd[curr];
        tag_four[1] = cmd[curr + 1];
        tag_four[2] = cmd[curr + 2];
        tag_four[3] = cmd[curr + 3];
        tag_four[4] = '\0';

        tag_two[0] = cmd[curr];
        tag_two[1] = cmd[curr + 1];
        tag_two[2] = '\0';

        if (search("tlv_csv.csv", tag_four))
        {
            cout << "tagfour" << tag_four << endl;
            curr += 6;
            TLV *node = get_node_tlv_quad(curr, cmd);
            node->child = parse_tlv(node->value);
            // cout << node->tag << endl;
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
        else if (search("tlv_csv.csv", tag_two))
        {
            curr += 4;
            TLV *node = get_node_tlv_double(curr, cmd);
            node->child = parse_tlv(node->value);
            // cout << node->tag << endl;
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

int Hextodec(char *hexVal)
{
    int len = strlen(hexVal);
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

char *substring(char *string, int position, int length)
{
    char *pointer = new char[length + 1];

    for (int c = 0; c < length; c++)
    {
        pointer[c] = string[position++];
    }

    pointer[length] = '\0';

    return pointer;
}

TLV *get_node_tlv_double(int curr, char *cmd)
{
    TLV *node = new TLV;
    char len[3];
    char tag[5];
    len[0] = cmd[curr - 2];
    len[1] = cmd[curr - 1];
    len[2] = '\0';

    node->length = Hextodec(len);
    node->tag = new char[5];
    node->value = new char[node->length];

    node->tag[0] = cmd[curr - 4];
    node->tag[1] = cmd[curr - 3];
    node->tag[2] = '\0';

    node->next = NULL;
    // cout << tag << endl;
    int string_len = node->length;
    node->value = substring(cmd, curr, 2 * string_len);

    return node;
}

TLV *get_node_tlv_quad(int curr, char *cmd)
{
    TLV *node = new TLV;
    char len[3];
    char tag[5];
    len[0] = cmd[curr - 2];
    len[1] = cmd[curr - 1];
    len[2] = '\0';

    node->length = Hextodec(len);
    node->tag = new char[5];
    node->value = new char[node->length];
    node->tag[0] = cmd[curr - 6];
    node->tag[1] = cmd[curr - 5];
    node->tag[2] = cmd[curr - 4];
    node->tag[3] = cmd[curr - 3];
    node->tag[4] = '\0';

    node->next = NULL;

    int string_len = node->length;
    node->value = substring(cmd, curr, 2 * string_len);

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
void create()
{
    // file pointer
    fstream fout;

    // opens an existing csv file or creates a new file.
    fout.open("tlv_csv.csv", ios::out | ios::app);

    cout << "Enter the details of tag:";

    int i, index;
    string tag;

    // Read the input
    for (i = 0; i < 20; i++)
    {

        cin >> index >> tag;

        // Insert the data to file
        fout << index << ", "
             << tag << "\n";
    }
}

bool search(string filename, char *tag)
{
    fstream fin;
    fin.open(filename, ios::in);
    vector<string> row;
    string temp,
        word, line;
    int count = 0;

    int i;
    string comp = "";
    for (i = 0; i < strlen(tag); i++)
    {
        comp = comp + tag[i];
    }
    // cout << "comp:" << comp << endl;

    while (fin >> temp)
    {
        // cout << temp << endl;
        row.clear();

        getline(fin, line);
        int j = 0, ind = 0;
        stringstream lineStream(temp);

        while (getline(lineStream, word, ','))
        {
            row.push_back(word);
        }

        if (row[1].compare(comp) == 0)
        {
            // Print the found data
            count = 1;
            cout << "index " << row[0] << " : \n";
            cout << "Tag: " << row[1] << "\n";
            break;
        }
    }
    if (count == 1)
    {
        return true;
    }
    return false;
}
string convertToString(char *a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++)
    {
        s = s + a[i];
    }
    return s;
}
