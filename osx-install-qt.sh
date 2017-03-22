#qt installation based on https://github.com/benlau/qtci/blob/master/recipes/qt-5.7-osx

echo Downloading Qt
DOWNLOAD_URL=https://download.qt.io/archive/qt/5.7/5.7.0/qt-opensource-mac-x64-clang-5.7.0.dmg
INSTALLER=`basename $DOWNLOAD_URL`
INSTALLER_NAME=${INSTALLER%.*}
APPFILE=/Volumes/${INSTALLER_NAME}/${INSTALLER_NAME}.app/Contents/MacOS/${INSTALLER_NAME}
wget -c $DOWNLOAD_URL
hdiutil mount ${INSTALLER}
echo Installing Qt
wget -c https://raw.githubusercontent.com/benlau/qtci/master/bin/extract-qt-installer
chmod +x ./extract-qt-installer
./extract-qt-installer $APPFILE $PWD/Qt
export PATH=$PWD/Qt/5.7/clang_64/bin:$PATH
