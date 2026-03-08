plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "br.odb.nehe.lesson08"
    compileSdk = 36

    defaultConfig {
        applicationId = "br.odb.nehe.lesson08"
        minSdk = 22
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

tasks.register<Copy>( "copyAssets") {
    from(file("../../base.pfs"))
    into(file("src/main/assets"))
}

tasks {
    preBuild {
        dependsOn("copyAssets")
    }
}

dependencies {

}