#!/usr/bin/env python3
"""
PinkReader build and deploy script for Android.
Requires: Qt6 for Android, Android NDK, Android SDK, JDK
"""

import os
import sys
import subprocess
import argparse
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.resolve()
BUILD_DIR = PROJECT_ROOT / "build-android"

def run(cmd, **kwargs):
    print(f"  RUN: {' '.join(cmd)}")
    subprocess.run(cmd, check=True, **kwargs)

def build_android(args):
    """Build for Android using CMake + Qt."""
    qt_path = os.environ.get("QT6_ANDROID", os.environ.get("QTDIR", ""))
    ndk_path = os.environ.get("ANDROID_NDK_ROOT", os.environ.get("ANDROID_NDK", ""))
    sdk_path = os.environ.get("ANDROID_SDK_ROOT", os.environ.get("ANDROID_HOME", ""))
    abi = args.abi or "arm64-v8a"
    
    if not qt_path:
        print("ERROR: Set QT6_ANDROID or QTDIR to Qt6 Android install path")
        sys.exit(1)
    if not ndk_path:
        print("ERROR: Set ANDROID_NDK_ROOT to Android NDK path")
        sys.exit(1)
    if not sdk_path:
        print("ERROR: Set ANDROID_SDK_ROOT to Android SDK path")
        sys.exit(1)
    
    cmake = Path(qt_path) / "bin" / "qt-cmake"
    if not cmake.exists():
        cmake = "cmake"
    
    build_dir = BUILD_DIR / abi
    build_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"=== Building PinkReader for {abi} ===")
    
    run([
        str(cmake),
        "-DCMAKE_BUILD_TYPE=Release" if not args.debug else "-DCMAKE_BUILD_TYPE=Debug",
        f"-DANDROID_ABI={abi}",
        f"-DANDROID_NDK={ndk_path}",
        f"-DANDROID_SDK_ROOT={sdk_path}",
        f"-DQT_HOST_PATH={qt_path}",
        "-S", str(PROJECT_ROOT),
        "-B", str(build_dir),
        "-GNinja" if args.ninja else "",
    ])
    
    run(["cmake", "--build", str(build_dir), "--parallel", str(args.jobs)])
    
    print(f"=== Build complete: {build_dir} ===")

def deploy_android(args):
    """Deploy APK to device via ADB."""
    abi = args.abi or "arm64-v8a"
    build_dir = BUILD_DIR / abi
    
    apk_path = build_dir / "android-build" / "pinkreader.apk"
    
    if not apk_path.exists():
        print(f"APK not found: {apk_path}")
        print("Did you run build first?")
        sys.exit(1)
    
    run(["adb", "install", "-r", str(apk_path)])
    run(["adb", "shell", "am", "start", "-n", "io.pinkreader/org.qtproject.qt.android.bindings.QtActivity"])

def clean(args):
    """Remove build artifacts."""
    import shutil
    if BUILD_DIR.exists():
        shutil.rmtree(BUILD_DIR)
    print("Build artifacts cleaned.")

def main():
    parser = argparse.ArgumentParser(description="PinkReader build tool")
    subparsers = parser.add_subparsers(dest="command")
    
    build_parser = subparsers.add_parser("build", help="Build for Android")
    build_parser.add_argument("--abi", default="arm64-v8a", choices=["arm64-v8a", "armeabi-v7a", "x86_64", "x86"])
    build_parser.add_argument("--debug", action="store_true")
    build_parser.add_argument("--ninja", action="store_true")
    build_parser.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    
    deploy_parser = subparsers.add_parser("deploy", help="Deploy to device via ADB")
    deploy_parser.add_argument("--abi", default="arm64-v8a")
    
    subparsers.add_parser("clean", help="Clean build directories")
    
    args = parser.parse_args()
    
    commands = {
        "build": build_android,
        "deploy": deploy_android,
        "clean": clean,
    }
    
    if args.command in commands:
        commands[args.command](args)
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
