// Origin: RedReader image/LegacySaveImageCallback.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

// Forward declarations
class BaseActivity;
class UriString;

// PermissionCallback interface for save image functionality
// Port of LegacySaveImageCallback which implements BaseActivity.PermissionCallback
class PermissionCallback {
public:
	virtual ~PermissionCallback() = default;
	virtual void OnPermissionGranted() = 0;
	virtual void OnPermissionDenied() = 0;
};

class LegacySaveImageCallback : public PermissionCallback {
private:
	BaseActivity* mActivity;
	UriString* mUri;

public:
	LegacySaveImageCallback(BaseActivity* activity, UriString* uri);

	void OnPermissionGranted() override;
	void OnPermissionDenied() override;
};

} // namespace PinkReader
