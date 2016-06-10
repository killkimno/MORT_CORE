REM Create un-numbered, shorter NTFS hardlinks for Leptonica related library files
REM
REM static libraries
fsutil hardlink create giflib-static-mtdll-debug.lib giflib416-static-mtdll-debug.lib
fsutil hardlink create giflib416d.lib giflib416-static-mtdll-debug.lib
fsutil hardlink create giflibd.lib giflib416-static-mtdll-debug.lib

fsutil hardlink create giflib-static-mtdll.lib giflib416-static-mtdll.lib
fsutil hardlink create giflib416.lib giflib416-static-mtdll.lib
fsutil hardlink create giflib.lib giflib416-static-mtdll.lib

fsutil hardlink create libjpeg-static-mtdll-debug.lib libjpeg8c-static-mtdll-debug.lib
fsutil hardlink create libjpeg8cd.lib libjpeg8c-static-mtdll-debug.lib
fsutil hardlink create libjpegd.lib libjpeg8c-static-mtdll-debug.lib

fsutil hardlink create libjpeg-static-mtdll.lib libjpeg8c-static-mtdll.lib
fsutil hardlink create libjpeg8c.lib libjpeg8c-static-mtdll.lib
fsutil hardlink create libjpeg.lib libjpeg8c-static-mtdll.lib

fsutil hardlink create libpng-static-mtdll-debug.lib libpng143-static-mtdll-debug.lib
fsutil hardlink create libpng143d.lib libpng143-static-mtdll-debug.lib
fsutil hardlink create libpngd.lib libpng143-static-mtdll-debug.lib

fsutil hardlink create libpng-static-mtdll.lib libpng143-static-mtdll.lib
fsutil hardlink create libpng143.lib libpng143-static-mtdll.lib
fsutil hardlink create libpng.lib libpng143-static-mtdll.lib

fsutil hardlink create libtiff-static-mtdll-debug.lib libtiff394-static-mtdll-debug.lib
fsutil hardlink create libtiff394d.lib libtiff394-static-mtdll-debug.lib
fsutil hardlink create libtiffd.lib libtiff394-static-mtdll-debug.lib

fsutil hardlink create libtiff-static-mtdll.lib libtiff394-static-mtdll.lib
fsutil hardlink create libtiff394.lib libtiff394-static-mtdll.lib
fsutil hardlink create libtiff.lib libtiff394-static-mtdll.lib

fsutil hardlink create zlibd-static-mtdll-debug.lib zlib125-static-mtdll-debug.lib
fsutil hardlink create zlib125d.lib zlib125-static-mtdll-debug.lib
fsutil hardlink create zlibd.lib zlib125-static-mtdll-debug.lib

fsutil hardlink create zlib-static-mtdll.lib zlib125-static-mtdll.lib
fsutil hardlink create zlib125.lib zlib125-static-mtdll.lib
fsutil hardlink create zlib.lib zlib125-static-mtdll.lib

fsutil hardlink create leptonlib-static-mtdll-debug.lib liblept168-static-mtdll-debug.lib
fsutil hardlink create liblept168d-static.lib liblept168-static-mtdll-debug.lib
fsutil hardlink create libleptd-static.lib liblept168-static-mtdll-debug.lib

fsutil hardlink create leptonlib-static-mtdll.lib liblept168-static-mtdll-debug.lib
fsutil hardlink create liblept168-static.lib liblept168-static-mtdll-debug.lib
fsutil hardlink create liblept-static.lib liblept168-static-mtdll-debug.lib

REM dynamic libraries
IF EXIST liblept168d.dll (
fsutil hardlink create leptonlibd.dll liblept168d.dll
fsutil hardlink create libleptd.dll liblept168d.dll
)

IF EXIST liblept168d.lib (
fsutil hardlink create leptonlibd.lib liblept168d.lib
fsutil hardlink create libleptd.lib liblept168d.lib
)

fsutil hardlink create leptonlib.dll liblept168.dll
fsutil hardlink create liblept.dll liblept168.dll

fsutil hardlink create leptonlib.lib liblept168.lib
fsutil hardlink create liblept.lib liblept168.lib


