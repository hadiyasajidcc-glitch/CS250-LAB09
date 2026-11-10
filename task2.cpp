#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

struct WordNode {
    string word;
    int freq;
    WordNode* left;
    WordNode* right;
    WordNode(string w) {
        word = w;
        freq = 1;
        left = NULL;
        right = NULL;
    }
};

class WordBST {
    WordNode* root;
    int uniqueWords;
    int totalWords;

    WordNode* insertWord(WordNode* r, string w) {
        if (r == NULL) {
            uniqueWords++;
            return new WordNode(w);
        }
        if (w < r->word) r->left = insertWord(r->left, w);
        else if (w > r->word) r->right = insertWord(r->right, w);
        else r->freq++;
        return r;
    }

    void inorder(WordNode* r) {
        if (r == NULL) return;
        inorder(r->left);
        cout << r->word << ": " << r->freq << endl;
        inorder(r->right);
    }

    //put all nodes in a vector in alphabetical order
    void collect(WordNode* r, vector<WordNode*>& list) {
        if (r == NULL) return;
        collect(r->left, list);
        list.push_back(r);
        collect(r->right, list);
    }

    void destroy(WordNode* r) {
        if (r == NULL) return;
        destroy(r->left);
        destroy(r->right);
        delete r;
    }

public:
    WordBST() {
        root = NULL;
        uniqueWords = 0;
        totalWords = 0;
    }

    ~WordBST() {
        destroy(root);
    }

    void insert(string w) {
        totalWords++;
        root = insertWord(root, w);
    }

    //split the paragraph into lowercase words and insert them
    void addParagraph(string text) {
        string w = "";
        for (int i = 0; i < text.length(); i++) {
            char c = text[i];
            if (isalpha(c)) {
                w += tolower(c);
            } else {
                if (w != "") {
                    insert(w);
                    w = "";
                }
            }
        }
        if (w != "") insert(w);
    }

    void displayAlphabetical() {
        inorder(root);
    }

    void displayByFrequency() {
        vector<WordNode*> list;
        collect(root, list);

        //bubble sort, highest frequency first
        for (int i = 0; i < list.size() - 1; i++) {
            for (int j = 0; j < list.size() - i - 1; j++) {
                if (list[j]->freq < list[j + 1]->freq) {
                    WordNode* temp = list[j];
                    list[j] = list[j + 1];
                    list[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < list.size(); i++) {
            cout << list[i]->word << ": " << list[i]->freq << endl;
        }
    }

    void findMostFrequentWord() {
        vector<WordNode*> list;
        collect(root, list);

        int maxFreq = 0;
        for (int i = 0; i < list.size(); i++) {
            if (list[i]->freq > maxFreq) maxFreq = list[i]->freq;
        }

        cout << "Most frequent word(s), " << maxFreq << " times: ";
        for (int i = 0; i < list.size(); i++) {
            if (list[i]->freq == maxFreq) cout << list[i]->word << " ";
        }
        cout << endl;
    }

    void displaySummary() {
        cout << "Total unique words: " << uniqueWords << endl;
        cout << "Total word count: " << totalWords << endl;
    }
};

int main() {
    string paragraph = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

    WordBST tree;
    tree.addParagraph(paragraph);

    cout << "Alphabetical order:" << endl;
    tree.displayAlphabetical();

    cout << "By frequency:" << endl;
    tree.displayByFrequency();

    tree.findMostFrequentWord();
    tree.displaySummary();

    return 0;
}