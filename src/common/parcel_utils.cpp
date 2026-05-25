// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ParcelUtils.java

#include "parcel_utils.h"
#include "parcel_helper.h"

namespace PinkReader {

void ParcelUtils::writeNullableBoolean(DataWriter& writer, const bool* value) {
	if (value == nullptr) {
		writer.writeInt(0);
	} else if (*value) {
		writer.writeInt(1);
	} else {
		writer.writeInt(-1);
	}
}

bool* ParcelUtils::readNullableBoolean(DataReader& reader) {
	int32_t value = reader.readInt();

	switch (value) {
		case -1: return new bool(false);
		case 0:  return nullptr;
		case 1:  return new bool(true);
	}

	throw std::runtime_error("Invalid value " + std::to_string(value));
}

void ParcelUtils::writeNullableInt(DataWriter& writer, const int32_t* value) {
	if (value == nullptr) {
		writer.writeInt(0);
	} else {
		writer.writeInt(1);
		writer.writeInt(*value);
	}
}

int32_t* ParcelUtils::readNullableInt(DataReader& reader) {
	int32_t present = reader.readInt();

	if (present == 1) {
		return new int32_t(reader.readInt());
	} else {
		return nullptr;
	}
}

void ParcelUtils::writeNullableLong(DataWriter& writer, const int64_t* value) {
	if (value == nullptr) {
		writer.writeLong(0);
	} else {
		writer.writeLong(1);
		writer.writeLong(*value);
	}
}

int64_t* ParcelUtils::readNullableLong(DataReader& reader) {
	int64_t present = reader.readLong();

	if (present == 1) {
		return new int64_t(reader.readLong());
	} else {
		return nullptr;
	}
}

} // namespace PinkReader
