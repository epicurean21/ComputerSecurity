#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define INF 9876543
#define BITMAPFILEHEADER 14 // BITMAP 파일 헤더 크기는 14 바이트
#define BITMAPINFOHEADER 40 // BITMAP 파일 인포 크기는 40 바이트
#define BITMAPHEADER 54 // 합이 54 바이트
#define RGBSIZE 8
char buffer[INF];

string content;
ifstream filein;
ofstream  fileout;
int fileSize = 0;
int imageSize = 0;
int main(int argc, char** argv) {
    // input이 e, input
    if (argv[1][0] == 'e') {
        filein.open("origin.bmp", ios::binary); // 2진단위로 읽기
        getline(cin, content);

        // file size;
        filein.seekg(0, ios::end);
        fileSize = filein.tellg();

        // bmp파일의 header를읽는다
        filein.seekg(0, ios::beg);
        char fileHeader[BITMAPFILEHEADER];
        char fileInfo[BITMAPINFOHEADER];
        filein.read(fileHeader, BITMAPFILEHEADER);
        filein.read(fileInfo, BITMAPINFOHEADER);

        // 헤더를 제외한 실제
        imageSize = fileSize - BITMAPHEADER;

        // buffer에 이미지를 이미지의 RGB를 읽는다
        filein.read(buffer, imageSize);
        int rgb = 0; // rgb 위치의 인덱스 활용
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
            rgb += RGBSIZE; // 8바이트씩 넘긴다
        }
        fileout.open("stego.bmp", ios::binary);
        fileout.write(fileHeader, BITMAPFILEHEADER);
        fileout.write(fileInfo, BITMAPINFOHEADER);
        fileout.write(buffer, imageSize);
    }
    else if (argv[1][0] == 'd') {
        filein.open("stego.bmp", ios::binary);

        // 파일 사이즈를 알아보자
        filein.seekg(0, ios::end);
        fileSize = filein.tellg();

        // BITMAP HEADER를 담아두자
        filein.seekg(0, ios::beg);
        char fileHeader[BITMAPFILEHEADER];
        char fileInfo[BITMAPINFOHEADER];
        filein.read(fileHeader, BITMAPFILEHEADER);
        filein.read(fileInfo, BITMAPINFOHEADER);

        imageSize = fileSize - BITMAPHEADER;
        filein.read(buffer, imageSize);
        int rgb = 0; // rgb 위치의 인덱스 활용

        // file size만큼만 돌린다
        while (rgb <= imageSize) {
            char readChar = 0;
            for (int i = rgb; i < rgb + RGBSIZE; i++) {
                if (rgb > imageSize)
                    break;
                int lsb = ((int)buffer[i] % 2); //현재 버퍼에서 가져오기
                lsb = (lsb << i - rgb); // 비트마스킹으로 뒤집힌 문자를 되돌리기 !
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