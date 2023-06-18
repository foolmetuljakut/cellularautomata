#ifndef ca_bmp_hpp
#define ca_bmp_hpp

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace ca {

    template<unsigned BYTES_PER_PIXEL = 3>
    class bitmap {
        const int FILE_HEADER_SIZE = 14;
        const int INFO_HEADER_SIZE = 40;

        unsigned width, height, stride, paddingsize, widthInBytes;
        std::vector<std::vector<char>> data;

        void generateBitmapImage(std::string filename) {
            std::ofstream imageout(filename);
            imageout << createBitmapFileHeader(height, stride);
            imageout << createBitmapInfoHeader(height, width);
            
            std::vector<char> padding(paddingsize);
            for(unsigned p = 0; p < paddingsize; p++)
                    imageout << (char)0;
            for (unsigned j = 0; j < height; j++) {
                imageout.write(data[j].data(), width*BYTES_PER_PIXEL);
                imageout.write(padding.data(), padding.size());
            }

            imageout.close();
        }
        std::string createBitmapFileHeader(int height, int stride) {
            int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

            std::string fileHeader = {
                0,0,     /// signature
                0,0,0,0, /// image file size in bytes
                0,0,0,0, /// reserved
                0,0,0,0, /// start of pixel array
            };

            fileHeader[ 0] = (unsigned char)('B');
            fileHeader[ 1] = (unsigned char)('M');
            fileHeader[ 2] = (unsigned char)(fileSize      );
            fileHeader[ 3] = (unsigned char)(fileSize >>  8);
            fileHeader[ 4] = (unsigned char)(fileSize >> 16);
            fileHeader[ 5] = (unsigned char)(fileSize >> 24);
            fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

            return fileHeader;
        }
        std::string createBitmapInfoHeader(int height, int width) {
            std::string infoHeader = {
                0,0,0,0, /// header size
                0,0,0,0, /// image width
                0,0,0,0, /// image height
                0,0,     /// number of color planes
                0,0,     /// bits per pixel
                0,0,0,0, /// compression
                0,0,0,0, /// image size
                0,0,0,0, /// horizontal resolution
                0,0,0,0, /// vertical resolution
                0,0,0,0, /// colors in color table
                0,0,0,0, /// important color count
            };

            infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
            infoHeader[ 4] = (unsigned char)(width      );
            infoHeader[ 5] = (unsigned char)(width >>  8);
            infoHeader[ 6] = (unsigned char)(width >> 16);
            infoHeader[ 7] = (unsigned char)(width >> 24);
            infoHeader[ 8] = (unsigned char)(height      );
            infoHeader[ 9] = (unsigned char)(height >>  8);
            infoHeader[10] = (unsigned char)(height >> 16);
            infoHeader[11] = (unsigned char)(height >> 24);
            infoHeader[12] = (unsigned char)(1);
            infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

            return infoHeader;
        }
    public:
        bitmap(unsigned width, unsigned height) : width(width), height(height) {
            widthInBytes = width * BYTES_PER_PIXEL;
            for(unsigned j = 0; j < height; j++)
                data.emplace_back(widthInBytes);
            paddingsize = (4 - (widthInBytes) % 4) % 4;
            stride = widthInBytes + paddingsize;
        }
        char& at(unsigned i, unsigned j, unsigned rgbachannel) {
            return data[j][i * BYTES_PER_PIXEL + (BYTES_PER_PIXEL - 1 - rgbachannel)];
        }
        void save(std::string filename) {
            generateBitmapImage(filename);
        }
        template<unsigned bpp> friend std::ostream& operator<<(std::ostream& out, bitmap<bpp>& img);
    };

    template<unsigned bpp> std::ostream& operator<<(std::ostream& out, bitmap<bpp>& img) {
        return out;
    }
};

#endif //ca_bmp_hpp