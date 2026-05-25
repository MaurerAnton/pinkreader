# PinkReader ProGuard Rules
# Keep Qt classes
-keep class org.qtproject.qt.** { *; }

# Keep Reddit API models
-keep class org.pinkreader.app.models.** { *; }

# Keep serialization classes
-keepattributes *Annotation*
-keepclassmembers class * {
    @kotlinx.serialization.Serializable *;
}

# OkHttp
-dontwarn okhttp3.**
-dontwarn okio.**

# General Android rules
-keepattributes Signature
-keepattributes InnerClasses
-keepattributes EnclosingMethod
-keepattributes Exceptions
