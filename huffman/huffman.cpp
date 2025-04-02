#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <sstream> // faster string handling

using namespace std;

class huffman_coder
{
private:
    struct node
    {
        char data; // character stored in the node
        int cost;  // frequency of the character
        node *left, *right;

        // constructor to initialize node
        node(char d, int c) : data(d), cost(c), left(nullptr), right(nullptr) {}
    };

    struct compare
    {
        // custom comparator for priority queue (min-heap)
        bool operator()(node *a, node *b)
        {
            return a->cost > b->cost; // lower cost = higher priority
        }
    };

    unordered_map<char, string> encoder; // char → binary code
    unordered_map<string, char> decoder; // binary code → char
    node *root;                          // root of huffman tree

    // recursively generate huffman codes
    void init_encoder_decoder(node *n, string prefix)
    {
        if (!n)
            return;

        // if it's a leaf node, assign encoding
        if (!n->left && !n->right)
        {
            encoder[n->data] = prefix;
            decoder[prefix] = n->data;
        }

        // recurse for left (0) and right (1) subtrees
        init_encoder_decoder(n->left, prefix + "0");
        init_encoder_decoder(n->right, prefix + "1");
    }

    // recursively free memory allocated for huffman tree
    void free_memory(node *n)
    {
        if (!n)
            return;
        free_memory(n->left);
        free_memory(n->right);
        delete n;
    }

public:
    // constructor: builds huffman tree and initializes encoding/decoding maps
    huffman_coder(const string &feeder)
    {
        unordered_map<char, int> freq_map;

        // count frequency of each character
        for (char c : feeder)
        {
            freq_map[c]++;
        }

        // priority queue (min-heap) for building huffman tree
        priority_queue<node *, vector<node *>, compare> min_heap;
        for (auto &entry : freq_map)
        {
            min_heap.push(new node(entry.first, entry.second));
        }

        // build huffman tree by combining two lowest frequency nodes iteratively
        while (min_heap.size() > 1)
        {
            node *first = min_heap.top();
            min_heap.pop();
            node *second = min_heap.top();
            min_heap.pop();

            // create new internal node with combined frequency
            node *new_node = new node('\0', first->cost + second->cost);
            new_node->left = first;
            new_node->right = second;

            min_heap.push(new_node);
        }

        // root of the huffman tree
        root = min_heap.top();
        init_encoder_decoder(root, ""); // generate encoding/decoding mappings
    }

    // destructor: cleans up memory
    ~huffman_coder()
    {
        free_memory(root);
    }

    // encode a given string using huffman encoding
    string encode(const string &source)
    {
        ostringstream encoded_stream;
        for (char c : source)
        {
            encoded_stream << encoder[c];
        }
        return encoded_stream.str();
    }

    // decode a huffman-encoded string back to the original text
    string decode(const string &coded_string)
    {
        string key;
        ostringstream decoded_stream;
        for (char c : coded_string)
        {
            key += c;
            auto it = decoder.find(key);
            if (it != decoder.end())
            {
                decoded_stream << it->second;
                key.clear();
            }
        }
        return decoded_stream.str();
    }
};

// driver code to test huffman_coder
int main()
{
    string text = "hey pratap"; // sample input text

    huffman_coder coder(text); // create huffman coder instance

    string encoded = coder.encode(text); // encode input text
    cout << "encoded: " << encoded << endl;

    string decoded = coder.decode(encoded); // decode back to original text
    cout << "decoded: " << decoded << endl;

    return 0;
}

/*
    compile & run:
        a. g++ huffman.cpp -o huffman.exe
        b. ./huffman.exe
*/
