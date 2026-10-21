#pragma once

#include <stdexcept>

class ImageProcessorError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class BmpError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};

class OpenFileError : public BmpError {
public:
    using BmpError::BmpError;
};

class WriteFileError : public BmpError {
public:
    using BmpError::BmpError;
};

class NotBmpError : public BmpError {
public:
    using BmpError::BmpError;
};

class UnsupportedBmpError : public BmpError {
public:
    using BmpError::BmpError;
};

class CorruptedBmpError : public BmpError {
public:
    using BmpError::BmpError;
};

class ArgumentError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};

class UsageError : public ArgumentError {
public:
    using ArgumentError::ArgumentError;
};

class UnknownFilterError : public ArgumentError {
public:
    using ArgumentError::ArgumentError;
};

class FilterParameterError : public ArgumentError {
public:
    using ArgumentError::ArgumentError;
};