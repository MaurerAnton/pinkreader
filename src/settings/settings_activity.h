/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: settings_activity.h - Port of SettingsActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/settings/SettingsActivity.java
 *
 * Original: public class SettingsActivity extends ViewsBaseActivity
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class ViewsBaseActivity;

// Android framework stubs
class Bundle;

// RedReader settings fragment
class SettingsFragment;

/**
 * @brief Port of org.quantumbadger.redreader.settings.SettingsActivity
 *
 * Settings activity that hosts settings panels via SettingsFragment.
 * Every field, method, constant, and inner class ported exactly.
 */
class SettingsActivity : public QObject {
    Q_OBJECT

public:
    // ===== Constructor / Lifecycle =====
    explicit SettingsActivity(QObject *parent = nullptr);
    virtual ~SettingsActivity();

    // ===== Activity lifecycle (port of overrides) =====

    // Java lines 46-65: protected void onCreate(final Bundle savedInstanceState)
    void onCreate(Bundle *savedInstanceState);

    // ===== Public methods =====

    // Java lines 67-69: public void onPanelSelected(@NonNull final String panel)
    void onPanelSelected(const QString &panel);

private:
    // ===== Private methods =====

    // Java lines 32-44: private void launchFragment(@NonNull final String panel)
    void launchFragment(const QString &panel);

signals:
    // Qt signal for fragment transitions / options menu invalidations
    void panelLaunchRequested(const QString &panel);
    void optionsMenuInvalidated();
};

} // namespace PinkReader
