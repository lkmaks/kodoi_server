#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <QByteArray>


#define SERIALIZE(...)         \
  template <typename Archive>  \
  void serialize(Archive& a) { \
    a(__VA_ARGS__);            \
  };



struct Archives {
    using InputArchive = cereal::JSONInputArchive;
    using OutputArchive = cereal::JSONOutputArchive;
    using BinaryInputArchive = cereal::BinaryInputArchive;
    using BinaryOutputArchive = cereal::BinaryOutputArchive;
};


template <typename T>
QByteArray Serialize(const T& object) {
    std::stringstream output;
    {
        Archives::OutputArchive oarchive(output);
        oarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    auto str = output.str();
    return QByteArray(str.c_str(), str.size());
}


template <typename T>
T Deserialize(const QByteArray& bytes) {
    std::string str = bytes.toStdString();

    T object;

    std::stringstream input(str);
    {
        Archives::InputArchive iarchive(input);
        iarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    return object;
}



template <typename T>
QByteArray SerializeBinary(const T& object) {
    std::stringstream output;
    {
        Archives::BinaryOutputArchive oarchive(output);
        oarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    auto str = output.str();
    return QByteArray(str.c_str(), str.size());
}


template <typename T>
T DeserializeBinary(const QByteArray& bytes) {
    std::string str = bytes.toStdString();

    T object;

    std::stringstream input(str);
    {
        Archives::BinaryInputArchive iarchive(input);
        iarchive(object);
    }  // archive goes out of scope, ensuring all contents are flushed

    return object;
}



#endif // HELPERS_H
