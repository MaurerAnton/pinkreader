// Origin: RedReader http/body/HTTPRequestBody.kt
#pragma once

#include <vector>
#include <functional>
#include <string>
#include "post_field.h"
#include "multipart_part.h"

namespace PinkReader {

// sealed interface HTTPRequestBody
// Nested subclasses defined outside the base class to avoid incomplete type issues
class HTTPRequestBody {
public:
	virtual ~HTTPRequestBody() = default;
};

class HTTPRequestBodyMultipart : public HTTPRequestBody {
private:
	std::vector<Part> mParts;

public:
	HTTPRequestBodyMultipart() = default;

	HTTPRequestBodyMultipart& AddPart(Part part) {
		mParts.push_back(std::move(part));
		return *this;
	}

	void ForEachPart(const std::function<void(const Part&)>& consumer) const {
		for (const auto& part : mParts) {
			consumer(part);
		}
	}
};

class HTTPRequestBodyPostFields : public HTTPRequestBody {
private:
	std::vector<PostField> mPostFields;

public:
	HTTPRequestBodyPostFields() = default;

	explicit HTTPRequestBodyPostFields(const std::vector<PostField>& postFields)
		: mPostFields(postFields) {}

	HTTPRequestBodyPostFields(std::initializer_list<PostField> fields)
		: mPostFields(fields) {}

	HTTPRequestBodyPostFields& AddField(PostField field) {
		mPostFields.push_back(std::move(field));
		return *this;
	}

	std::string EncodeFields() const {
		return PostField::EncodeList(mPostFields);
	}
};

} // namespace PinkReader
