/*
ayoungprogrammer.blogspot.com
 
Part 1: Extracting contours from text
 
*/
 
#include <iostream>
#include "MainCore.h"
#include <Windows.h>
 
#ifdef _CH_
#pragma package <opencv>
#endif
 
#ifndef _EiC
#include "opencv.hpp"
#include "highgui.hpp"
#include "ml.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <baseapi.h>
#include <allheaders.h>

#include <process.h>
#include <mmsystem.h>

#endif



using namespace std;
using namespace cv;
extern "C" __declspec(dllimport)void InitNHOCR();
extern "C" __declspec(dllimport)int ProcessNHocr(int imgWidth, int imgheight, uchar* data, wstring* resultString, int channel);
tesseract::TessBaseAPI api;
MainCore *myMainCore = new MainCore();
Mat* screenImg = new Mat();

int captureCount = 0;
bool isUseQuick = false;
struct resultDB
{
	std::wstring original;
	std::wstring translation;
};
void GetDBText(resultDB *result);
extern std::wstring utfStringToWstring(std::string originalString);
extern std::wstring stringToWstring(std::string originalString);


	
void getImg(int captureIndex)
{
	myMainCore->getScreen(screenImg, captureIndex);
}

bool isError = false;
std::string legacyTesData;

void GetTesserctText()
{
	__try {
	
		api.Recognize(0);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		isError = true;
		//cout << "Error! : " << ex.what();
	}
}


void getText(resultDB *result)
{
		api.Clear();
		std::string text;
		std::wstring wText;
		int nhocrResult = 1;

		
		if (myMainCore->GetIsUseNHocr())
		{
			myMainCore->debugStruct.debug = std::to_wstring(screenImg->channels());
			nhocrResult = ProcessNHocr(screenImg->size().width, screenImg->size().height, screenImg->data, &wText, screenImg->channels());

			if (nhocrResult == -1)
			{
				wText = L"error nhocr";
			}
		}
		else
		{ 

			
			api.SetImage((uchar*)screenImg->data, screenImg->size().width, screenImg->size().height, screenImg->channels(), screenImg->step1());
			
			cout << "start! : " << std::endl;
			isError = false;
			GetTesserctText();

			if (!isError)
			{
				char* out = api.GetUTF8Text();
				text = out;
				wText = utfStringToWstring(text);
			}
			else

			{
				cout << "Error! : " << std::endl;
				
				api.Clear();
				api.End();
				api.Init(".\\tessdata", legacyTesData.c_str(), tesseract::OEM_TESSERACT_ONLY);		//레가시 모드
				//api.Init(".\\tessdata", currentTesData, tesseract::OEM_LSTM_ONLY);				//고급 모드
				api.SetPageSegMode(tesseract::PSM_AUTO);
				
				if (myMainCore->GetIsUseJpnFlag() == false)
				{
					api.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmonpqrstuvwxyz1234567890.,`'&?!()- ");
				}
				else
				{
					api.SetVariable("tessedit_char_whitelist", "");
				}

				api.SetImage((uchar*)screenImg->data, screenImg->size().width, screenImg->size().height, screenImg->channels(), screenImg->step1());

				GetTesserctText();

				char* out = api.GetUTF8Text();
				text = out;
				wText = utfStringToWstring(text);

				wText = L"에러 : Tesseract OCR 에 문제가 발생하여 레가시로 전환합니다. " + wText;
				
			}			
		}

		if (myMainCore->GetIsRemoveSpace())
		{
			myMainCore->ReplaceAll(wText, L" ", L"");
		}
		
		 
		 if(myMainCore->getUseCheckSpellingFlag() == true)
		 {			 
			 for(int i = 0; i < 2 ; i++)
			 {
				 bool isReplaceFlag = false;
				 wText = myMainCore->checkSpelling(wText , &isReplaceFlag);

				 if(isReplaceFlag == false)
				 {
					 break;
				 }
			 }			
		 }

		 result->original = wText;
		 GetDBText(result);

		 
		 screenImg->release();
}

void GetDBText(resultDB *result)
{
	if (myMainCore->getUseDBFlag() == true)
	{
		result->translation = myMainCore->getTranslation(result->original);
	}
	else
	{
		result->translation = L"";
	}
}

extern "C" __declspec(dllexport)void
SetIsDebugMode(bool isDebug, bool isShowReplace, bool isSaveCapture, bool isSaveCaptureResult)
{
	myMainCore->debugMode.isActive = isDebug;
	myMainCore->debugMode.isShowReplace = isShowReplace;
	myMainCore->debugMode.isSaveCapture = isSaveCapture;
	myMainCore->debugMode.isSaveCaptureResult = isSaveCaptureResult;
}


extern "C" __declspec(dllexport)void
	setFiducialValue(int r[], int g[], int b[], int s1[], int s2[], int v1[], int v2[], int size)
{
	myMainCore->setFiducialValue(r, g, b, s1, s2, v1, v2, size);
}

extern "C" __declspec(dllexport)void
SetIsStringUpper(bool isUpper)
{
	myMainCore->SetIsStringUpper(isUpper);
}

extern "C" __declspec(dllexport)void
SetIsActiveWindow(bool isActiveWindow)
{
	myMainCore->SetIsActiveWindow(isActiveWindow);
}

 extern "C" __declspec(dllexport)void
	  SetIsUseNHocr(bool isUseNHocr)
 {
	 myMainCore->SetIsUseNHocr(isUseNHocr);
 }

   extern "C" __declspec(dllexport)void
	   setUseDB(bool isUseDBFlag, char *dbFileText)
 {
	 myMainCore->setUseDB(isUseDBFlag, dbFileText);
 }
      extern "C" __declspec(dllexport)void
	   setAdvencedImgOption(bool isUseRGBFlag, bool isUseHSVFlag, bool isUseErodeFlag, float imgZoomSize)
 {
	 myMainCore->setAdvencedImgOption(isUseRGBFlag, isUseHSVFlag, isUseErodeFlag,  imgZoomSize);
 }
   extern "C" __declspec(dllexport)void
	   setUseCheckSpelling(bool isUseCheckSpellingFlag, bool isMatchingWord, char *dicFileTxt)
 {
	 myMainCore->setUseCheckSpellingFlag(isUseCheckSpellingFlag, isMatchingWord, dicFileTxt);
 }

   extern "C" __declspec(dllexport)void
	   SetIsUseJpn(bool _isUseJpn)
   {
	   myMainCore->setIsUseJpnFlag(_isUseJpn);
   }
   

 extern "C" __declspec(dllexport)void
	 setTessdata(char *tessData, bool isUseJpnFlag)
 {


	isError = false;
	

	api.Clear();
	api.End();
	//api.Init(".\\tessdata", tessData, tesseract::OEM_TESSERACT_ONLY);		//레가시 모드
	api.Init(".\\tessdata",tessData, tesseract::OEM_LSTM_ONLY);				//고급 모드
	api.SetPageSegMode(tesseract::PSM_AUTO);
	myMainCore->setIsUseJpnFlag(isUseJpnFlag);
	if(isUseJpnFlag == false)
	{
		api.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmonpqrstuvwxyz1234567890.,`'&?!()- ");
	}
	else
	{
		api.SetVariable("tessedit_char_whitelist", "");
	}

	if (strcmp(tessData, "eng_fast") == 0)
	{
		legacyTesData.assign("eng_legacy");
	}
	else if (strcmp(tessData, "jpn_fast") == 0)
	{
		legacyTesData.assign("jpn_legacy");
	}
	else
	{
		legacyTesData.assign(strcat(tessData, "_legacy"));
	}

	
		
 }
 
  extern "C" __declspec(dllexport)void
	 setErode()
 {
	 myMainCore->setErode(true);
  }

  extern "C" __declspec(dllexport)void
	  SetRemoveSpace(bool isRemove)
  {
	  myMainCore->SetRemoveSpace(isRemove);
  }

  //OCR 인덱스 번호 출력
  extern "C" __declspec(dllexport)void
	  SetShowOCRIndex(bool isShow)
  {
	  myMainCore->SetShowOCRIndex(isShow);
  }



  
 extern "C" __declspec(dllexport)void
	 initOcr(){
	
	isError = false;
	std::wcout << "init nhocr" << std::endl;
	InitNHOCR();
	std::wcout << "init setting" << std::endl;
	myMainCore->init();
	
	api.Clear();
	api.End();

	std::wcout << "init tessract ocr " << std::endl;

	api.Init(".\\tessdata", "eng", tesseract::OEM_LSTM_ONLY);
	

	std::wcout << "init tessract set page mode " << std::endl;
	api.SetPageSegMode(tesseract::PSM_AUTO);
	std::wcout << "init tessract white list " << std::endl;
	api.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmonpqrstuvwxyz1234567890.,'&?! ");
	std::wcout << "init complete " << std::endl;

	legacyTesData = "eng_legacy";

 }
 

#pragma region ::::::::::: OCR 색 그룹 사용 여부 설정 :::::::::::

 extern "C" __declspec(dllexport)void
	 ClearOcrColorSet(){
	 myMainCore->ClearOcrColorSet();
 }

 extern "C" __declspec(dllexport)void
	 AddOcrColorSet(int colorList[], int size){

	 myMainCore->AddOcrColorSet(colorList, size);
 }

#pragma endregion


 extern "C" __declspec(dllexport)void
	 setCutPoint(int newX[], int newY[], int newX2[], int newY2[], int size){
		captureCount = size;
		myMainCore->setCutPoint(newX,newY,newX2,newY2, size);
 
}


 extern "C" __declspec(dllexport)void
	 SetExceptPoint(int newX[], int newY[], int newX2[], int newY2[], int size) {
	 myMainCore->SetExceptPoint(newX, newY, newX2, newY2, size);

 }



    extern "C" __declspec(dllexport)void
	 processOcr(wchar_t resultOriginal[], wchar_t resultTranslation[]){

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		resultDB getDB;
		std::wstring out;

		std::wstring ocrResult = L"";
		std::wstring translationResult = L"";
		if(captureCount == 1)
		{
				getImg(0);
				getText(&getDB);
				
				ocrResult = ocrResult + getDB.original + L"\r\n";
				translationResult = translationResult + getDB.translation;
		}
		else
		{
			for(int i=0; i < captureCount; i++)
			{
				getImg(i);
				getText(&getDB);

				if (myMainCore->GetIsShowOCRIndex())
				{
					stringstream s;
					s << i + 1;
					std::string converted(s.str());
					std::wstring wConverted = stringToWstring(converted);
					ocrResult = ocrResult + wConverted + L": " + getDB.original + L"\r\n";
					if (getDB.translation.compare(L"not thing") != 0)
					{
						translationResult = translationResult + wConverted + L": " + getDB.translation + L"\r\n";
					}
					s.clear();
				}
				else
				{
					if (!getDB.original.empty())
					{
						ocrResult = ocrResult + L"- " + getDB.original + L"\r\n";
						if (getDB.translation.compare(L"not thing") != 0)
						{
							translationResult = translationResult + L"- " + getDB.translation + L"\r\n";
						}
					}					
				}
			
			}
		}
		//testestst
		//ocrResult = ocrResult + L"utf : " + stringToWstring(std::to_string(MainCore::utf));


		//std::wstring debug = std::to_wstring(myMainCore->debugStruct.clientCoordinateX) + L" / " + std::to_wstring(myMainCore->debugStruct.clientCoordinateY);	//for debug

		std::wcscpy(resultOriginal, ocrResult.c_str());
		//std::wcscat(resultOriginal, myMainCore->debugStruct.debug.c_str());	//for debug
		std::wcscpy(resultTranslation , translationResult.c_str());
}

	//이미지 결과 가져오기.
	extern "C" __declspec(dllexport)uchar*
		processGetImgData(int caputreIndex, int *x, int *y, int *channels) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		
		if (captureCount >= caputreIndex + 1)
		{
			getImg(caputreIndex);

			//data = screenImg->data;
			*y = screenImg->size().height;
			*x = screenImg->size().width;
			*channels = screenImg->channels();

			uchar * data = (uchar*)LocalAlloc(LPTR, screenImg->size().height * screenImg->size().width * screenImg->channels());
			int max = screenImg->size().height * screenImg->size().width * screenImg->channels();
			for (int i = 0; i < max; i++)
			{
				data[i] = screenImg->data[i];
			}
			//LocalFree(data);
			screenImg->release();
			return data;
		}
		else

		{
			return NULL;
		}
	
		//myMainCore->debugStruct.debug = std::to_wstring(screenImg->channels());
		//nhocrResult = ProcessNHocr(screenImg->size().width, screenImg->size().height, screenImg->data, &wText, screenImg->channels());
		
	}

	//스펠 체크.
	extern "C" __declspec(dllexport)void
		ProcessGetSpellingCheck(wchar_t resultOriginal[], bool isUseMatchWordDic) {

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		std::wstring out;

		std::wstring wText = resultOriginal;
	
		bool isReplaceFlag = false;
		if (!isUseMatchWordDic)
		{
			wText = myMainCore->GetLetterSpellingCheck(wText, &isReplaceFlag);
		}
		else

		{
			wText = myMainCore->GetMatchingSpellingCheck(wText, &isReplaceFlag);
		}
	

		std::wcscpy(resultOriginal, wText.c_str());
	}

	

	//TODO : 작업중.
	//DB만 가져오기.
	//TEXT를 받아오고 반환해야 함.
	extern "C" __declspec(dllexport)void
		ProcessGetDBText(wchar_t resultOriginal[], wchar_t resultTranslation[]) {

		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		resultDB getDB;
		std::wstring out;

		std::wstring ocrResult = resultOriginal;
		std::wstring translationResult = L"";
		getDB.original = ocrResult;
		GetDBText(&getDB);

		//ocrResult = ocrResult + getDB.original + L"\r\n";
		translationResult = translationResult + getDB.translation;

		std::wcscpy(resultTranslation, translationResult.c_str());
	}


