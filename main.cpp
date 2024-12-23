#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

enum Type { WARNING, ERROR, FATAL_ERROR, UNKNOWN_MESSAGE };

class LogMessage {
 public:

  LogMessage* next_ = nullptr;

  Type enum_;

  std::string message_;

  virtual Type type() const final { return enum_; }

  virtual void setnextHandler(LogMessage* next_handler) final {
    next_ = next_handler;
  }

  virtual void messageAnalysis(const std::string message, Type enum_) = 0;

  virtual ~LogMessage() = default;
};

class Warning : public LogMessage {
 public:

  void messageAnalysis(const std::string message, Type _enum) override {
    enum_ = _enum;
    if (type() == Type::WARNING) {
      std::cout  << message << std::endl;
    } else if (next_ != nullptr) {
      next_->messageAnalysis(message, enum_);
    } else {
      throw std::exception("pointer is nullptr\n");
    }
  }
};

class unknownMessage : public LogMessage {
 public:

  void messageAnalysis(const std::string message, Type _enum) override {
    enum_ = _enum;
    if (type() == Type::UNKNOWN_MESSAGE) {
      throw std::exception("Unknown message\n");
    } else if (next_ != nullptr) {
      next_->messageAnalysis(message, enum_);
    } else {
      throw std::exception("pointer is nullptr\n");
    }
  }
};

class Error : public LogMessage {
  std::string path_;

 public:
  Error(const std::string path) : path_(path) {}
  void messageAnalysis(const std::string message, Type _enum) override {
    enum_ = _enum;
    if (type() == Type::ERROR) {
      std::ofstream file(path_);
      if (file.is_open()) {
        file << message << std::endl;
        file.close();
      } else {
        std::cout << "file is not open\n";
      }

    } else if (next_ != nullptr) {
      next_->messageAnalysis(message, enum_);
    } else {
      throw std::exception("pointer is nullptr\n");
    }
  }
};

class fatalError : public LogMessage {
 public:

  void messageAnalysis(const std::string message, Type _enum) override {
    enum_ = _enum;
    if (type() == Type::FATAL_ERROR) {
      throw std::exception("Fatal error, program is stop\n");
    } else if (next_ != nullptr) {
      next_->messageAnalysis(message, enum_);
    } else {
     throw std::exception("pointer is nullptr\n");
    }
  }
};

int main() {

    LogMessage* w_obj = new Warning;
    LogMessage* fatal_obj = new fatalError;
    LogMessage* error_obj = new Error("text.txt");
    LogMessage* unknown_obj = new unknownMessage;

    try {
      w_obj->setnextHandler(fatal_obj);
      fatal_obj->setnextHandler(error_obj);
      error_obj->setnextHandler(unknown_obj);
      w_obj->messageAnalysis("Warning", Type::WARNING);
      delete w_obj;
      delete fatal_obj;
      delete error_obj;
      delete unknown_obj;
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
}
