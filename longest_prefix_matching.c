#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Structure to represent a node in the trie
typedef struct TrieNode {
    struct TrieNode *children[2]; // 0 and 1 for binary trie
    uint32_t next_hop;            // Next hop IP address or interface
} TrieNode;

// Structure to represent a routing table entry
typedef struct {
    uint32_t prefix;        // Network prefix (IP address)
    uint8_t prefix_length;  // Length of the prefix (in bits)
    uint32_t next_hop;      // Next hop IP address or interface
} RoutingTableEntry;

// Function to create a new trie node
TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node) {
        node->children[0] = NULL;
        node->children[1] = NULL;
        node->next_hop = 0;
    }
    return node;
}

// Function to insert a routing table entry into the trie
void insertIntoTrie(TrieNode* root, RoutingTableEntry entry) {
    TrieNode* current = root;

    // Iterate over each bit in the prefix
    for (int i = 31; i >= (32 - entry.prefix_length); --i) {
        int bit = (entry.prefix >> i) & 1;

        // Create a new node if the path doesn't exist
        if (!current->children[bit]) {
            current->children[bit] = createTrieNode();
        }

        // Move to the child node
        current = current->children[bit];
    }

    // Set the next hop in the leaf node
    current->next_hop = entry.next_hop;
}


// Function to perform longest prefix match using trie
uint32_t longest_prefix_match_trie(uint32_t ip_address, TrieNode* root) {
    TrieNode* current = root;
    uint32_t next_hop = 0;

    // Traverse the trie based on the IP address
    for (int i = 31; i >= 0; --i) {
        int bit = (ip_address >> i) & 1;

        // If a leaf node is found with a next hop, update next_hop
        if (current->next_hop != 0) {
            next_hop = current->next_hop;
        }

        // Move to the child node
        if (current->children[bit]) {
            current = current->children[bit];
        } else {
            // If no child node exists, return the next hop found so far
            break;
        }
    }

    return next_hop;
}

// Function to free the memory allocated for the trie
void freeTrie(TrieNode* root) {
    if (root) {
        for (int i = 0; i < 2; ++i) {
            if (root->children[i]) {
                freeTrie(root->children[i]);
            }
        }
        free(root);
    }
}
int main() {
    // Initialize the root of the trie
    TrieNode* root = createTrieNode();

    // Router needs a routing table
    RoutingTableEntry routing_table[] = {
        {0x0A000000, 24, 0x0A000001}, // Route for 10.0.0.0/24 with next hop 10.0.0.1
        {0x0A000100, 24, 0x0A000002}, // Route for 10.0.1.0/24 with next hop 10.0.0.2
        {0x0A010000, 16, 0x0A000003}, // Route for 10.1.0.0/16 with next hop 10.0.0.3
        {0x00000000, 0, 0x0A000004},   // Default route with next hop 10.0.0.4
    };
    size_t num_entries = sizeof(routing_table) / sizeof(routing_table[0]);

    // Insert each routing table entry into the trie
    for (size_t i = 0; i < num_entries; ++i) {
        insertIntoTrie(root, routing_table[i]);
    }

    // Example IP addresses to test the LPM function
    uint32_t ip_addresses[] = {0x0A000001, 0x0A000123, 0x0A010101, 0x0B000001};
    size_t num_ips = sizeof(ip_addresses) / sizeof(ip_addresses[0]);

    // Perform LPM for each IP address using the trie
    for (size_t i = 0; i < num_ips; ++i) {
        uint32_t next_hop = longest_prefix_match_trie(ip_addresses[i], root);
        printf("IP Address: %08X, Next Hop: %08X\n", ip_addresses[i], next_hop);
    }

    // Free the memory allocated for the trie
    freeTrie(root);

    return 0;
}
