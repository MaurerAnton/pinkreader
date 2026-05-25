// Origin: org/quantumbadger/redreader/fragments/ErrorPropertiesDialog.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

class RRError;
class Context;
class BaseActivity;
class LinearLayout;
class MaterialAlertDialogBuilder;
class AppCompatActivity;

class ErrorPropertiesDialog {
public:
    static std::unique_ptr<ErrorPropertiesDialog> newInstance(const RRError& error);

protected:
    std::string getTitle(Context& context);
    void prepare(BaseActivity& context, LinearLayout& items);
    void interceptBuilder(MaterialAlertDialogBuilder& builder);

private:
    explicit ErrorPropertiesDialog(const RRError& error);

    AppCompatActivity* mContext = nullptr;
    const RRError& mError;
};

} // namespace PinkReader
