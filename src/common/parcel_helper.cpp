// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ParcelHelper.java

#include "parcel_helper.h"
#include <stdexcept>

namespace PinkReader {

bool ParcelHelper::readBoolean(DataReader& in) {
	return in.readByte() == 1;
}

std::string* ParcelHelper::readNullableString(DataReader& in) {
	bool isNull = readBoolean(in);
	if (isNull) {
		return nullptr;
	}
	return new std::string(in.readString());
}

int32_t* ParcelHelper::readNullableInt(DataReader& in) {
	bool isNull = readBoolean(in);
	if (isNull) {
		return nullptr;
	}
	return new int32_t(in.readInt());
}

int64_t* ParcelHelper::readNullableLong(DataReader& in) {
	bool isNull = readBoolean(in);
	if (isNull) {
		return nullptr;
	}
	return new int64_t(in.readLong());
}

bool* ParcelHelper::readNullableBoolean(DataReader& in) {
	bool isNull = readBoolean(in);
	if (isNull) {
		return nullptr;
	}
	return new bool(readBoolean(in));
}

void ParcelHelper::writeBoolean(DataWriter& writer, bool b) {
	writer.writeByte(b ? 1 : 0);
}

void ParcelHelper::writeNullableString(DataWriter& writer, const std::string* value) {
	if (value == nullptr) {
		writeBoolean(writer, false);
	} else {
		writeBoolean(writer, true);
		writer.writeString(*value);
	}
}

void ParcelHelper::writeNullableLong(DataWriter& writer, const int64_t* value) {
	if (value == nullptr) {
		writeBoolean(writer, false);
	} else {
		writeBoolean(writer, true);
		writer.writeLong(*value);
	}
}

void ParcelHelper::writeNullableBoolean(DataWriter& writer, const bool* value) {
	if (value == nullptr) {
		writeBoolean(writer, false);
	} else {
		writeBoolean(writer, true);
		writeBoolean(writer, *value);
	}
}

} // namespace PinkReader
