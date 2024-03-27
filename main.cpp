#include <windows.h>
#include <commctrl.h>
#include <string>
#include "playfair.h"

#define ID_EDITBOX_PLAINTEXT    1
#define ID_EDITBOX_KEY          2
#define ID_BUTTON_ENCRYPT       3
#define ID_EDITBOX_CIPHERTEXT   4
#define ID_BUTTON_DECRYPT       5
#define ID_EDITBOX_DECRYPTTEXT  6

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "PlayfairCipher";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Failed to register window class", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindowEx(0, "PlayfairCipher", "Playfair Cipher",
                          WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          500, 300, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndPlaintext, hwndKey, hwndCiphertext, hwndDecrypttext;
    static HWND hwndEncryptBtn, hwndDecryptBtn;

    switch (msg) {
        case WM_CREATE: {
            CreateWindow("STATIC", "Plaintext:", WS_VISIBLE | WS_CHILD,
                         10, 10, 80, 20, hwnd, NULL, NULL, NULL);
            hwndPlaintext = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                         100, 10, 200, 20, hwnd, (HMENU)ID_EDITBOX_PLAINTEXT, NULL, NULL);

            CreateWindow("STATIC", "Key:", WS_VISIBLE | WS_CHILD,
                         10, 40, 80, 20, hwnd, NULL, NULL, NULL);
            hwndKey = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                   100, 40, 200, 20, hwnd, (HMENU)ID_EDITBOX_KEY, NULL, NULL);

            hwndEncryptBtn = CreateWindow("BUTTON", "Encrypt", WS_VISIBLE | WS_CHILD,
                                          100, 70, 100, 30, hwnd, (HMENU)ID_BUTTON_ENCRYPT, NULL, NULL);

            CreateWindow("STATIC", "Ciphertext:", WS_VISIBLE | WS_CHILD,
                         10, 110, 80, 20, hwnd, NULL, NULL, NULL);
            hwndCiphertext = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
                                          100, 110, 200, 20, hwnd, (HMENU)ID_EDITBOX_CIPHERTEXT, NULL, NULL);

            hwndDecryptBtn = CreateWindow("BUTTON", "Decrypt", WS_VISIBLE | WS_CHILD,
                                          100, 140, 100, 30, hwnd, (HMENU)ID_BUTTON_DECRYPT, NULL, NULL);

            CreateWindow("STATIC", "Decrypted Text:", WS_VISIBLE | WS_CHILD,
                         10, 180, 90, 20, hwnd, NULL, NULL, NULL);
            hwndDecrypttext = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
                                           100, 180, 200, 20, hwnd, (HMENU)ID_EDITBOX_DECRYPTTEXT, NULL, NULL);
            break;
        }

        case WM_COMMAND: {
	    switch (LOWORD(wParam)) {
	        case ID_BUTTON_ENCRYPT: {
	            char plaintext[256];
	            char key[256];
	
	            GetWindowText(hwndPlaintext, plaintext, sizeof(plaintext));
	            GetWindowText(hwndKey, key, sizeof(key));
	
	            generateMatrix(std::string(key));
	            std::string formatted = formatMessage(std::string(plaintext));
	            std::string ciphertext = encrypt(formatted);
	
	            SetWindowText(hwndCiphertext, ciphertext.c_str());
	            break;
	        }
	
	        case ID_BUTTON_DECRYPT: {
	            char ciphertext[256];
	            char key[256];
	
	            GetWindowText(hwndCiphertext, ciphertext, sizeof(ciphertext));
	            GetWindowText(hwndKey, key, sizeof(key));
	
	            generateMatrix(std::string(key));
	            std::string decrypted = decrypt(std::string(ciphertext));
	
	            SetWindowText(hwndDecrypttext, decrypted.c_str());
	            break;
	        }
	    }
	    break;
	}

        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
