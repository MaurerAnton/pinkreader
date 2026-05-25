// Origin: RedReader http/body/multipart/Part.kt
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <variant>

namespace PinkReader {

// sealed interface Part
// data class FormData(name, value)
// class FormDataBinary(name, value: ByteArray)

struct FormData {
	std::string name;
	std::string value;

	FormData(std::string name, std::string value)
		: name(std::move(name)), value(std::move(value)) {}
};

struct FormDataBinary {
	std::string name;
	std::vector<uint8_t> value;

	FormDataBinary(std::string name, std::vector<uint8_t> value)
		: name(std::move(name)), value(std::move(value)) {}
};

using Part = std::variant<FormData, FormDataBinary>;

} // namespace PinkReader
