plugins {
    id("com.android.application")
    kotlin("android") version "2.0.0" apply false
}

android {
    namespace = "org.pinkreader.app"
    compileSdk = 35

    defaultConfig {
        applicationId = "org.pinkreader.app"
        minSdk = 21
        targetSdk = 35
        versionCode = 1000000
        versionName = "1.0.0-alpha"
        ndk { abiFilters += listOf("arm64-v8a", "armeabi-v7a", "x86", "x86_64") }
    }

    buildTypes {
        release {
            isMinifyEnabled = true
            isShrinkResources = true
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
        debug { isDebuggable = true }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    externalNativeBuild {
        cmake {
            path = file("../CMakeLists.txt")
            version = "3.28.1"
        }
    }

    sourceSets {
        getByName("main") {
            manifest.srcFile("AndroidManifest.xml")
            java.srcDirs("src/main/java")
            res.srcDirs("res")
        }
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.13.1")
    implementation("androidx.appcompat:appcompat:1.7.0")
    implementation("com.google.android.material:material:1.13.0")
}
