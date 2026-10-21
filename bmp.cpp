#include "errors.h"
#include "bmp.h"

#include <cstdint>
#include <fstream>
#include <vector>
#include <filesystem>

namespace {
    constexpr uint16_t KSignature = 0x4D42; // "BM"
    constexpr uint32_t FileHeaderSize = 14;
    constexpr uint32_t InfoHeaderSize = 40;
    constexpr uint16_t BitsPerPixel = 24;

    template <typename T>
    T ReadNumber(std::ifstream &in) {
        unsigned char bytes[sizeof(T)];
        if (!in.read(reinterpret_cast<char *>(bytes), sizeof(T))) {
            throw CorruptedBmpError("BMP файл слишком маленькой длины");
        }
        T result = 0;
        for (size_t i = 0; i < sizeof(T); ++i) {
            result |= static_cast<T>(static_cast<T>(bytes[i]) << (8 * i));
        }
        return result;
    }

    template <typename T>
    void WriteNumber(std::ofstream &out, T value) {
        for (size_t i = 0; i < sizeof(T); ++i) {
            out.put(static_cast<char>((value >> (8 * i)) & 0xFF));
        }
    }

    // Длина строки пикселей в байтах
    size_t RowSize(size_t width) {
        return (width * 3 + 3) / 4 * 4;
    }

    float ByteToColor(unsigned char byte) {
        return static_cast<float>(byte) / 255.0f;
    }

    unsigned char ColorToByte(float value) {
        if (value <= 0.0f) {
            return 0;
        }
        if (value >= 1.0f) {
            return 255;
        }
        return static_cast<unsigned char>(value * 255.0f + 0.5f);
    }
} //namespace

Image ReadBmp(const std::string &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw OpenFileError("Не получилось открыть файл: " + path);
    }
    const uint64_t file_size = std::filesystem::file_size(path);
    if (ReadNumber<uint16_t>(in) != KSignature) {
        throw NotBmpError("Не BMP файл: " + path);
    }
    ReadNumber<uint32_t>(in); // размер
    ReadNumber<uint32_t>(in);
    const uint32_t start = ReadNumber<uint32_t>(in);
    const uint32_t header_size = ReadNumber<uint32_t>(in);
    const int32_t raw_width = static_cast<int32_t>(ReadNumber<uint32_t>(in));
    const int32_t raw_height = static_cast<int32_t>(ReadNumber<uint32_t>(in));
    ReadNumber<uint16_t>(in);
    const uint16_t bits_per_pixel = ReadNumber<uint16_t>(in);
    const uint32_t compression = ReadNumber<uint32_t>(in);
    if (header_size != InfoHeaderSize) {
        throw UnsupportedBmpError("Неправильный BMP header");
    }
    if (bits_per_pixel != BitsPerPixel) {
        throw UnsupportedBmpError("Не 24-битный BMP");
    }
    if (compression != 0) {
        throw UnsupportedBmpError("BMP без сжатия");
    }
    if (raw_width <= 0 || raw_height == 0) {
        throw CorruptedBmpError("Неправильное разрешение картинки");
    }
    const bool top_down = raw_height < 0;
    const size_t width = static_cast<size_t>(raw_width);
    const size_t height = static_cast<size_t>(top_down ? -static_cast<int64_t>(raw_height) : raw_height);
    const size_t row_size = RowSize(width);
    if (start > file_size || (file_size - start) / row_size < height) {
        throw CorruptedBmpError("Размеры картинки неверные");
    }
    Image image(width, height);
    in.seekg(start);
    std::vector<unsigned char> row(row_size);
    for (size_t i = 0; i < height; ++i) {
        if (!in.read(reinterpret_cast<char *>(&row[0]), static_cast<std::streamsize>(row_size))) {
            throw CorruptedBmpError("Не получилось считать ряд");
        }
        const size_t y = top_down ? i : height - 1 - i;
        for (size_t x = 0; x < width; ++x) {
            Color &color = image.At(x, y);
            color.b = ByteToColor(row[x * 3]);
            color.g = ByteToColor(row[x * 3 + 1]);
            color.r = ByteToColor(row[x * 3 + 2]);
        }
    }
    return image;
}

void WriteBmp(const Image &image, const std::string &path) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw WriteFileError("Не получилось открыть файл для записи: " + path);
    }
    const size_t width = image.Width();
    const size_t height = image.Height();
    const size_t row_size = RowSize(width);
    const auto pixels_size = static_cast<uint32_t>(row_size * height);

    WriteNumber<uint16_t>(out, KSignature);
    WriteNumber<uint32_t>(out, FileHeaderSize + InfoHeaderSize + pixels_size); //размер
    WriteNumber<uint32_t>(out, 0);
    WriteNumber<uint32_t>(out, FileHeaderSize + InfoHeaderSize);

    WriteNumber<uint32_t>(out, InfoHeaderSize);
    WriteNumber<uint32_t>(out, static_cast<uint32_t>(width));
    WriteNumber<uint32_t>(out, static_cast<uint32_t>(height));
    WriteNumber<uint16_t>(out, 1);
    WriteNumber<uint16_t>(out, BitsPerPixel);
    WriteNumber<uint32_t>(out, 0);
    WriteNumber<uint32_t>(out, pixels_size);
    WriteNumber<uint32_t>(out, 0);
    WriteNumber<uint32_t>(out, 0);
    WriteNumber<uint32_t>(out, 0);
    WriteNumber<uint32_t>(out, 0);
    std::vector<unsigned char> row(row_size, 0);
    for (size_t i = 0; i < height; ++i) {
        const size_t y = height - 1 - i;
        for (size_t x = 0; x < width; ++x) {
            const Color &color = image.At(x, y);
            row[x * 3] = ColorToByte(color.b);
            row[x * 3 + 1] = ColorToByte(color.g);
            row[x * 3 + 2] = ColorToByte(color.r);
        }
        out.write(reinterpret_cast<const char *>(row.data()), static_cast<std::streamsize>(row_size));
    }
    if (!out) {
        throw WriteFileError("Не получилось записать файл: " + path);
    }
}