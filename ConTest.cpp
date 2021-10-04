#include <iostream>
#include <cstdlib>
#include <fstream>
#include <windows.h>

int main(int argc, char** argv)
{
    using std::cin;
    
    long stat[256][256];
    long hist[256];
    double shannon = 0;
    const auto console = ::GetConsoleWindow();
    const auto context = ::GetDC(console);
    constexpr auto red = RGB(255, 0, 0);
    constexpr auto yellow = RGB(255, 255, 0);
    constexpr auto white = RGB(255, 255, 255);
    int length;
    long max = 0; 

    std::ifstream is(argv[1], std::ifstream::binary);

    for (int i = 0; i < 256; ++i) for (int j = 0; j < 256; j++)
        stat[i][j] = 0;    

    for (int i = 0; i < 256; ++i) 
        hist[i] = 0;


    if (is) {
        is.seekg(0, is.end);
        length = is.tellg();
        is.seekg(0, is.beg);

        byte old;
        byte next;

        char* buffer = new char[1];

        std::cout << "Reading " << length << " characters... ";
        is.read(buffer, 1);

        old = (byte)(*buffer);
        hist[old] = hist[old] + 1;

        for (int i = 1; i<length; i++) {
            is.read(buffer, 1);
            next = (byte)(*buffer);
            stat[old][next] = stat[old][next] + 1;
            hist[next] = hist[next] + 1;
            if (stat[old][next] > max) {
                max = stat[old][next];
            }
            old = next;
        }

        for (int i = 0; i < 256; i++) {
            double p = ((double)hist[i]) / ((double)length);
            shannon += (hist[i]==0)?0:(p * log2(p));
            
        }
        shannon = -shannon;

        if (is)
            std::cout << "all characters read successfully.";
        else
            std::cout << "error: only " << is.gcount() << " could be read";
        is.close();

        delete[] buffer;
    }    

    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; j++) {        
            printf("%d, ", stat[i][j]);
            stat[i][j] = log2(((double)stat[i][j]/(double)max)+1) *255.0;        
        }
        printf("\n");
    }

    printf("Shannon's entropy index H = %f\n", shannon);

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            ::SetPixel(context, i + 25, j + 25, RGB(stat[i][j] > 0 ? 255 : 0, stat[i][j] > 0 ? 255 : 0, stat[i][j] > 0 ? 255 : 0));
            ::SetPixel(context, i + 325, j + 25, RGB(stat[i][j], stat[i][j], stat[i][j]));
            //::SetPixel(context, i + 625, j + 25, (unsigned long)stat[i][j]*255*255);
        }
    }
    cin.get();
}
