#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>

using namespace std;

typedef vector<bool>            code_vector;
typedef map<char, code_vector>  table;

struct BYTE {
    unsigned b1: 1;
    unsigned b2: 1;
    unsigned b3: 1;
    unsigned b4: 1;
    unsigned b5: 1;
    unsigned b6: 1;
    unsigned b7: 1;
    unsigned b8: 1;
};

union bit_pack {
    char ch;
    BYTE bits;
};

struct Node {

    char            data;
    unsigned        freq;
    Node            *left, *right;

    Node(char data, unsigned freq) {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

struct compare {
    bool operator() (Node* l, Node* r) {
        return (l->freq > r->freq);
    }
};

code_vector string_to_bitvec(string packed) {
    code_vector result;

    for (string::iterator it = packed.begin(); it != packed.end(); ++it) {
        if(*it == 48) result.push_back(0);
        else result.push_back(1);
    }

    return result;
}

code_vector text_to_code(string text, table & codes) {
    code_vector result;

    for (string::iterator it = text.begin(); it != text.end(); ++it) {
        code_vector b = codes[*it];
        result.insert(result.end(), b.begin(), b.end());
    }

    return result;
}

void buildCodes(struct Node* root, string str, table* codes)
{

    if (!root)
        return;

    if (root->data != '$') {
        (*codes).insert( pair<char, code_vector>(root->data, string_to_bitvec(str)));
    }

    buildCodes(root->left, str + "0", codes);
    buildCodes(root->right, str + "1", codes);
}

code_vector encode(multimap<int, char> freqchart, string text) {
    struct Node *left, *right, *top;

    priority_queue<Node*, vector<Node*>, compare> minHeap;

    for (multimap<int, char>::iterator itr = freqchart.begin(); itr != freqchart.end(); ) {
        minHeap.push(new Node(itr->second, itr->first));
        ++itr;
    }

    while (minHeap.size() != 1) {

        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        top = new Node('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    table codes;
    buildCodes(minHeap.top(), "", &codes);

    return text_to_code(text, codes);
}

string code_vector_to_str(code_vector bool_text) {
    string compressed_text;

    int i;
    for (i = 0; i < bool_text.size() + 7; i = i + 8) {
        char pack = bool_text[i];

        for (int j = i + 1; j < i + 8; j++) {
            pack <<= 1;

            pack += bool_text[j];
        }

        compressed_text += pack;
    }

    return compressed_text;
}



int main()
{
    ifstream input("lorem.txt");
    ofstream outfile("compressed.data");

    map<char, int> freq;
    string text;

    if (input.is_open())
    {
        char c;
        while (input.get(c))
        {
            map<char, int>::iterator it = freq.find(c);
            if (it != freq.end())
            {
                freq[c] = freq[c] + 1;
            }
            else
            {
                freq[c] = 1;
            }
            text += c;
        };
        input.close();

        multimap<int, char> sorted;
        for (map<char, int>::iterator it = freq.begin(); it != freq.end(); it++)
        {
            sorted.insert(make_pair(it->second, it->first));
        }

        code_vector bool_text = encode(sorted, text);
        string compressed_text = code_vector_to_str(bool_text);

        copy(compressed_text.begin(), compressed_text.end(), ostreambuf_iterator<char>(outfile));
    }
    else
    {
        cout << "Unable to open file";
        return 1;
    }

    return 0;
}
