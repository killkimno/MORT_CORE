# MORT_CORE

<img src="https://github.com/killkimno/MORT/blob/main/MORT_LOGO.png" width="90%"></img>

MORT 프로젝트에 필요한 MORT_CORE.DLL

### 받은 후 해야하는 작업 ###
라이브러리는 용량이 큰 관계로 커밋을 안 함
1. vcpkg 설치
2. 프로젝트에 포함된 vcpkg setting 에 있는 파일을 vcpkg에 설치
3. vcpkg install opencv:x64-windows-static-md
4. vcpkg install tesseract:x64-windows-static-md

### Tesseract 5.2 ###
https://github.com/LilyWangLL/vcpkg/tree/dev/LilyWang/issue16019

### MORT 1.239 대응 커밋
bd02323715552747f3e28a27418780691b4d0504
https://github.com/killkimno/MORT_CORE/commit/bd02323715552747f3e28a27418780691b4d0504

여기까지가 마지막 32비트 대응 DLL 그 다음부터는 64 비트 대응임
