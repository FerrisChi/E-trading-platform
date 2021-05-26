#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <cstring>
#include <iostream>

class Exception : public std::exception {
 public:
  explicit Exception(const std::string &message)
      : exception(), message(message) {}

  virtual ~Exception(void) throw() {}

  inline std::string getMessage(void) const { return this->message; }

 protected:
  std::string message;
};

class InvalidUserDataException : public Exception {
 public:
  InvalidUserDataException(const std::string &message) : Exception(message) {}
};

class InvalidCoordinatesException : public Exception {
 public:
  InvalidCoordinatesException(const std::string &message)
      : Exception(message) {}
};

#endif