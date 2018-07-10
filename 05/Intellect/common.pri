#CONFIG += build_translations
CONFIG += c++14

CONFIG(release, debug|release){
    #message(Release)
    BUILD_TYPE = release
}else{
    #message(Debug)
    BUILD_TYPE = debug
}

BUILD_DIR = $$PWD/../bin
win32 {
    ARCH_DIR       = $${OUT_PWD}/../build/$${TARGET}
    ARCH_TYPE      = win32
}

#message("BUILD_DIR="$$BUILD_DIR)
#message("ARCH_DIR="$$ARCH_DIR)

DEST_BINS = $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}

#message("DEST_BINS="$$DEST_BINS)

MOC_DIR        = $${ARCH_DIR}/$${BUILD_TYPE}/moc
UI_DIR         = $${ARCH_DIR}/$${BUILD_TYPE}/ui
UI_HEADERS_DIR = $${ARCH_DIR}/$${BUILD_TYPE}/ui
UI_SOURCES_DIR = $${ARCH_DIR}/$${BUILD_TYPE}/ui
OBJECTS_DIR    = $${ARCH_DIR}/$${BUILD_TYPE}/obj
RCC_DIR        = $${ARCH_DIR}/$${BUILD_TYPE}/rcc

#TRANSLATIONS_PATH = $$PWD/translations
