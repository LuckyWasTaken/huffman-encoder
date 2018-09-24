#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>

using namespace std;

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



void buildCodes(struct Node* root, string str, map<char, string>* codes)
{

    if (!root)
        return;

    if (root->data != '$') {
        (*codes).insert( pair<char, string>(root->data, str));
    }

    buildCodes(root->left, str + "0", codes);
    buildCodes(root->right, str + "1", codes);
}

void encode(multimap<int, char> freqchart) {
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

    map<char, string> codes;
    buildCodes(minHeap.top(), "", &codes);
}

int main()
{
    ifstream input("lorem.txt");
    map<char, int> freq;

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
        };
        input.close();

        multimap<int, char> sorted;
        for (map<char, int>::iterator it = freq.begin(); it != freq.end(); it++)
        {
            sorted.insert(make_pair(it->second, it->first));
        }

        encode(sorted);
    }
    else
    {
        cout << "Unable to open file";
        return 1;
    }

    return 0;
}