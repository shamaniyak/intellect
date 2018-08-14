#CONFIG += build_translations
CONFIG += c++14

CONFIG(release, debug|release){
    #message(Release)
    BUILD_TYPE = release
}else{
    #message(Debug)
    BUILD_TYPE = debug
}

win32 {
    ARCH_DIR       = $${OUT_PWD}/../build/$${TARGET}
    ARCH_TYPE      = win32
}

#message("BUILD_DIR="$$BUILD_DIR)
#message("ARCH_DIR="$$ARCH_DIR)

BIN_DIR = $$PWD/bin
DEST_BINS = $${BIN_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}
BUILD_DIR = $${ARCH_DIR}/$${BUILD_TYPE}

message("BUILD_DIR="$$BUILD_DIR)

MOC_DIR        = $${BUILD_DIR}/moc
UI_DIR         = $${BUILD_DIR}/ui
UI_HEADERS_DIR = $${BUILD_DIR}/ui
UI_SOURCES_DIR = $${BUILD_DIR}/ui
OBJECTS_DIR    = $${BUILD_DIR}/obj
RCC_DIR        = $${BUILD_DIR}/rcc

#TRANSLATIONS_PATH = $$PWD/translations
