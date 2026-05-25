/*
 * PinkReader - GPLv3
 * File: uri_string.h
 * Port of: org.quantumbadger.redreader.common.UriString.kt (Kotlin data class)
 *
 * Every field, method, companion object method from the original Kotlin file.
 * Original annotations: @Immutable, @Parcelize, data class, : Parcelable
 */

#pragma once

#include <QString>
#include <optional>
#include <string>

namespace PinkReader {

// Forward declaration for Android Uri equivalent
class PinkUri;

/**
 * @brief Port of org.quantumbadger.redreader.common.UriString
 *
 * Original: @Immutable @Parcelize data class UriString(
 *     @JvmField val value: String
 * ) : Parcelable
 *
 * Every constructor, method, and companion object method matches exactly.
 */
class UriString {
public:
	// Port of: data class constructor: UriString(val value: String)
	UriString() = default;
	explicit UriString(QString value) : m_value(std::move(value)) {}

	// Port of: @JvmField val value: String
	QString value() const { return m_value; }
	void setValue(const QString &v) { m_value = v; }

	// Port of: override fun toString() = value
	QString toString() const { return m_value; }

	// Port of: fun toUri() = Uri.parse(value)
	PinkUri toUri() const;

	// Port of: companion object

	// Port of: @JvmStatic fun fromNullable(value: String?) = value?.let { UriString(it) }
	static std::optional<UriString> fromNullable(const std::optional<QString> &value) {
		if (value.has_value()) {
			return UriString(value.value());
		}
		return std::nullopt;
	}

	// Port of: @JvmStatic fun from(value: Uri) = UriString(value.toString())
	static UriString from(const PinkUri &uri);

	// Port of: @JvmStatic fun from(value: Uri.Builder) = UriString(value.toString())
	// NOTE: Uri.Builder not yet ported; stub for now
	static UriString fromBuilder(const QString &builderString) {
		return UriString(builderString);
	}

	// Comparison operators (for data class equivalence)
	bool operator==(const UriString &other) const { return m_value == other.m_value; }
	bool operator!=(const UriString &other) const { return m_value != other.m_value; }

private:
	// Port of: @JvmField val value: String
	QString m_value;
};

} // namespace PinkReader
