#include "playfair.h"
#include <cctype>

char mat[5][5];

void generateMatrix(std::string key) {
    int flag[26] = {0};
    int x = 0, y = 0;

    for (int i = 0; i < key.length(); i++) {
        if (key[i] == 'j') key[i] = 'i';

        if (flag[std::tolower(key[i]) - 'a'] == 0) {
            mat[x][y++] = std::tolower(key[i]);
            flag[std::tolower(key[i]) - 'a'] = 1;
        }
        if (y == 5) x++, y = 0;
    }

    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (ch == 'j') continue;

        if (flag[ch - 'a'] == 0) {
            mat[x][y++] = ch;
            flag[ch - 'a'] = 1;
        }
        if (y == 5) x++, y = 0;
    }
}

std::string formatMessage(std::string msg) {
    for (int i = 0; i < msg.length(); i++) {
        if (msg[i] == 'j') msg[i] = 'i';
    }

    for (int i = 1; i < msg.length(); i += 2) {
        if (std::tolower(msg[i - 1]) == std::tolower(msg[i])) msg.insert(i, "x");
    }

    if (msg.length() % 2 != 0) msg += "x";
    return msg;
}

struct Position {
    int row;
    int col;
};

Position getPosition(char c) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (c == mat[i][j]) {
                Position p = {i, j};
                return p;
            }
        }
    }
    return {-1, -1};
}

std::string encrypt(std::string message) {
    std::string ctext = "";
    for (int i = 0; i < message.length(); i += 2) {
        Position p1 = getPosition(std::tolower(message[i]));
        Position p2 = getPosition(std::tolower(message[i + 1]));
        int x1 = p1.row, y1 = p1.col;
        int x2 = p2.row, y2 = p2.col;

        if (x1 == x2) {
            ctext += mat[x1][(y1 + 1) % 5];
            ctext += mat[x2][(y2 + 1) % 5];
        } else if (y1 == y2) {
            ctext += mat[(x1 + 1) % 5][y1];
            ctext += mat[(x2 + 1) % 5][y2];
        } else {
            ctext += mat[x1][y2];
            ctext += mat[x2][y1];
        }
    }
    return ctext;
}

std::string decrypt(std::string message) {
    std::string ptext = "";
    for (int i = 0; i < message.length(); i += 2) {
        Position p1 = getPosition(std::tolower(message[i]));
        Position p2 = getPosition(std::tolower(message[i + 1]));
        int x1 = p1.row, y1 = p1.col;
        int x2 = p2.row, y2 = p2.col;

        if (x1 == x2) {
            ptext += mat[x1][(y1 - 1 + 5) % 5];
            ptext += mat[x2][(y2 - 1 + 5) % 5];
        } else if (y1 == y2) {
            ptext += mat[(x1 - 1 + 5) % 5][y1];
            ptext += mat[(x2 - 1 + 5) % 5][y2];
        } else {
            ptext += mat[x1][y2];
            ptext += mat[x2][y1];
        }
    }
    return ptext;
}
