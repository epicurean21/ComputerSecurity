#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define INF 9876543
#define BITMAPFILEHEADER 14 // BITMAP ���� ��� ũ��� 14 ����Ʈ
#define BITMAPINFOHEADER 40 // BITMAP ���� ���� ũ��� 40 ����Ʈ
#define BITMAPHEADER 54 // ���� 54 ����Ʈ
#define RGBSIZE 8
char buffer[INF];

string content;
ifstream filein;
ofstream  fileout;
int fileSize = 0;
int imageSize = 0;
int main(int argc, char** argv) {
    // input�� e, input
    if (argv[1][0] == 'e') {
        filein.open("origin.bmp", ios::binary); // 2�������� �б�
        getline(cin, content);

        // file size;
        filein.seekg(0, ios::end);
        fileSize = filein.tellg();

        // bmp������ header���д´�
        filein.seekg(0, ios::beg);
        char fileHeader[BITMAPFILEHEADER];
        char fileInfo[BITMAPINFOHEADER];
        filein.read(fileHeader, BITMAPFILEHEADER);
        filein.read(fileInfo, BITMAPINFOHEADER);

        // ����� ������ ����
        imageSize = fileSize - BITMAPHEADER;

        // buffer�� �̹����� �̹����� RGB�� �д´�
        filein.read(buffer, imageSize);
        int rgb = 0; // rgb ��ġ�� �ε��� Ȱ��
        for (int i = 0; i < content.size() + RGBSIZE; i++) {
            int curBit = (int)content[i];
            if (i == content.size()) {
                int lastChar = '\0';
                for (int j = rgb; j < rgb + RGBSIZE; j++) {
                    int lsb = lastChar % 2;
                    buffer[j] -= ((int)buffer[j] % 2);
                    buffer[j] += (int)lsb;
                    lastChar /= 2;
                }
                break;
            }
            for (int j = rgb; j < rgb + RGBSIZE; j++) {
                int lsb = curBit % 2;
                buffer[j] -= ((int)buffer[j] % 2);
                buffer[j] += (int)lsb;
                curBit /= 2;
            }
            rgb += RGBSIZE; // 8����Ʈ�� �ѱ��
        }
        fileout.open("stego.bmp", ios::binary);
        fileout.write(fileHeader, BITMAPFILEHEADER);
        fileout.write(fileInfo, BITMAPINFOHEADER);
        fileout.write(buffer, imageSize);
    }
    else if (argv[1][0] == 'd') {
        filein.open("stego.bmp", ios::binary);

        // ���� ����� �˾ƺ���
        filein.seekg(0, ios::end);
        fileSize = filein.tellg();

        // BITMAP HEADER�� ��Ƶ���
        filein.seekg(0, ios::beg);
        char fileHeader[BITMAPFILEHEADER];
        char fileInfo[BITMAPINFOHEADER];
        filein.read(fileHeader, BITMAPFILEHEADER);
        filein.read(fileInfo, BITMAPINFOHEADER);

        imageSize = fileSize - BITMAPHEADER;
        filein.read(buffer, imageSize);
        int rgb = 0; // rgb ��ġ�� �ε��� Ȱ��

        // file size��ŭ�� ������
        while (rgb <= imageSize) {
            char readChar = 0;
            for (int i = rgb; i < rgb + RGBSIZE; i++) {
                if (rgb > imageSize)
                    break;
                int lsb = ((int)buffer[i] % 2); //���� ���ۿ��� ��������
                lsb = (lsb << i - rgb); // ��Ʈ����ŷ���� ������ ���ڸ� �ǵ����� !
                readChar += (int)lsb;
            }
            if (rgb > imageSize)
                break;
            rgb += RGBSIZE;
            if (readChar == '\0')
                break;
            content.push_back(readChar);
        }
        cout << content << "\n";
    }

    return 0;
}