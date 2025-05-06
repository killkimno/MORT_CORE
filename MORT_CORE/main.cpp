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
extern "C" __declspec(dllimport)int ProcessNHocr(int imgWidth, int imgheight, uchar * data, wstring * resultString, int channel);
tesseract::TessBaseAPI api;
MainCore* myMainCore = new MainCore();
Mat* screenImg = new Mat();

int captureCount = 0;
bool isUseQuick = false;
struct resultDB
{
	std::wstring original;
	std::wstring translation;
};
void GetDBText(resultDB* result);
extern std::wstring utfStringToWstring(std::string originalString);
extern std::wstring stringToWstring(std::string originalString);


void GetImg(int captureIndex, int* locationX, int* locationY)
{
	myMainCore->getScreen(screenImg, captureIndex, locationX, locationY);
}

void getImg(int captureIndex)
{
	myMainCore->getScreen(screenImg, captureIndex);
}

void GetImgWithData(int captureIndex, uint8_t* data, int widht, int height, int positionX, int positionY, bool getOriginal)
{
	myMainCore->GetImgWithData(screenImg, captureIndex, data, widht, height, positionX, positionY, getOriginal);
}

bool isError = false;

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


void getText(resultDB* result)
{
	api.Clear();
	std::string text;
	std::wstring wText;
	int nhocrResult = 1;


	if (myMainCore->GetIsUseNHocr())
	{
		std::wcout << std::endl << L" Start Get nhocr" << std::endl;
		myMainCore->debugStruct.debug = std::to_wstring(screenImg->channels());
		nhocrResult = ProcessNHocr(screenImg->size().width, screenImg->size().height, screenImg->data, &wText, screenImg->channels());

		if (nhocrResult == -1)
		{
			wText = L"error nhocr";
		}
	}
	else
	{
		
		clock_t start, end;
		start = clock();
		std::wcout << std::endl << L" Start Get Tessract" << std::endl;

		api.SetImage((uchar*)screenImg->data, screenImg->size().width, screenImg->size().height, screenImg->channels(), screenImg->step1());

	


		//cout << "start! : " << std::endl;
		isError = false;
		GetTesserctText();
		end = clock();

		double  time = (end - start);
		std::wcout << std::endl << L" end Tessract" << time << std::endl;
		if (!isError)
		{
			char* out = api.GetUTF8Text();
			text = out;

			wText = utfStringToWstring(text);
			std::wcout << std::endl << L" Get Tessract Success" << wText << std::endl;

		}
		else

		{
			text = "에러 : Tesseract OCR을 사용할 수 없습니다. 문제가 계속 발생시 MORT 1.239V 를 사용해 주세요 ";
			wText = L"에러 : Tesseract OCR을 사용할 수 없습니다. 문제가 계속 발생시 MORT 1.239V 를 사용해 주세요 ";
		}
	}

	std::wcout << std::endl << L" Remove Space?" << std::endl;
	if (myMainCore->GetIsRemoveSpace())
	{
		myMainCore->ReplaceAll(wText, L" ", L"");
	}



	std::wcout << std::endl << L" Check Spell?" << std::endl;
	if (myMainCore->getUseCheckSpellingFlag() == true)
	{
		std::wcout << std::endl << L" start Check Spell?" << std::endl;
		bool isReplaceFlag = false;

		wText = myMainCore->checkSpelling(wText, &isReplaceFlag, L"\n");

	}
	else
	{
		std::wcout << std::endl << L" start none Check Spell?" << std::endl;
		std::vector<std::wstring> text = myMainCore->StringSplite(wText, L"\n", -1);
		wText = L"";
		for (int i = 0; i < text.size(); i++)
		{
			wText = wText + text[i] + L"\r\n";
		}
	}


	result->original = wText;


	std::wcout << std::endl << L" Get DB?" << std::endl;
	GetDBText(result);

	std::wcout << std::endl << L" img release?" << std::endl;
	screenImg->release();
	std::wcout << std::endl << L" img release done?" << std::endl;
}

void GetDBText(resultDB* result)
{
	bool isFound = false;
	clock_t start, end;

	start = clock();
	if (myMainCore->getUseDBFlag() == true)
	{

		bool isMultiDB = myMainCore->GetIsUsePartialDB();


		if (isMultiDB)
		{//멀티에서는 분할해서 다시 검색한다.

			isFound = false;
			result->translation = myMainCore->getTranslation(result->original, isFound);

			if (!isFound)
			{
				bool isFoundSomething = false;

				std::wstring transResult;
				std::vector<std::wstring> textList = myMainCore->StringSplite(result->original, L"\r\n", 9);

				std::wcout << std::endl << L" ----------------" << textList.size() << std::endl;
				//std::wcout << std::endl << result->original << std::endl;
				int limit = textList.size();

				//8줄 이상 넘어가면 막아버린다.
				if (limit >= 8)
				{
					limit = 8;
				}


				int startIndex = limit - 1;
				int index = 0;


				for (; startIndex > -1; startIndex--)
				{
					//std::wcout << std::endl << L"Start : " << startIndex;
					std::wstring text;
					for (; index < startIndex; index++)
					{
						//std::wcout << std::endl << L" index : " << index << std::endl;

						if (index == -1)
						{
							for (int i = 0; i <= startIndex; i++)
							{
								text = text + textList[i] + L" ";

							}
						}
						else
						{
							text = textList[startIndex - index] + L" " + text;
						}

						//std::wcout << std::endl << L"text : " << text;

						std::wstring dbResult = myMainCore->getTranslation(text, isFound);

						if (isFound)
						{
							//std::wcout << std::endl << L"isFound : " << isFound;

							isFoundSomething = true;
							transResult = dbResult + L"\r\n" + transResult;

							if (index == -1)
							{
								startIndex = 0;
							}
							else
							{
								startIndex = startIndex - (index);
							}

							//std::wcout << std::endl << L" Result StartIndex :  : " << startIndex;

							break;

						}

						if (index == -1)
						{
							text = L"";
						}

					}

					index = -1;
				}

				if (isFoundSomething)
				{
					result->translation = transResult;
				}

				textList.clear();
			}


		}
		else
		{
			result->translation = myMainCore->getTranslation(result->original, isFound);
		}
	}
	else
	{
		result->translation = L"";
	}

	end = clock();

	double  time = (end - start);

	//std::wcout << std::endl << "OCR : " << result->original << std::endl;

	std::cout << std::endl << "Found : " << isFound << " DB Time : " << time << std::endl;
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
setUseDB(bool isUseDBFlag, bool isUsePartialDB, char* dbFileText)
{
	myMainCore->setUseDB(isUseDBFlag, isUsePartialDB, dbFileText);
}


extern "C" __declspec(dllexport)void
setAdvencedImgOption(bool isUseRGBFlag, bool isUseHSVFlag, bool isUseErodeFlag, float imgZoomSize, bool isUseThreshold, int thresholdValue)
{
	myMainCore->setAdvencedImgOption(isUseRGBFlag, isUseHSVFlag, isUseErodeFlag, imgZoomSize, isUseThreshold, thresholdValue);
}
extern "C" __declspec(dllexport)void
setUseCheckSpelling(bool isUseCheckSpellingFlag, bool isMatchingWord, char* dicFileTxt)
{
	myMainCore->setUseCheckSpellingFlag(isUseCheckSpellingFlag, isMatchingWord, dicFileTxt);
}

/// <summary>
/// 교정사전 추가 교정 횟수
/// </summary>
/// <param name="reProcessCount"></param>
extern "C" __declspec(dllexport)void
SetReCheckSpellingCount(int reProcessCount)
{
	myMainCore->SetReCheckSpellingCount(reProcessCount);
}


extern "C" __declspec(dllexport)void
SetIsUseJpn(bool _isUseJpn)
{
	myMainCore->setIsUseJpnFlag(_isUseJpn);
}


extern "C" __declspec(dllexport)void
setTessdata(char* tessData, bool isUseJpnFlag)
{

	isError = false;

	api.Clear();
	api.End();
	//api.Init(".\\tessdata", tessData, tesseract::OEM_TESSERACT_ONLY);		//레가시 모드
	api.Init(".\\tessdata", tessData, tesseract::OEM_LSTM_ONLY);				//고급 모드
	api.SetPageSegMode(tesseract::PSM_AUTO);
	myMainCore->setIsUseJpnFlag(isUseJpnFlag);
	if (isUseJpnFlag == false)
	{
		api.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmonpqrstuvwxyz1234567890.,`'&?!()- ");
	}
	else
	{
		api.SetVariable("tessedit_char_whitelist", "");
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
initOcr() {

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

}


#pragma region ::::::::::: OCR 색 그룹 사용 여부 설정 :::::::::::

extern "C" __declspec(dllexport)void
ClearOcrColorSet() {
	myMainCore->ClearOcrColorSet();
}

extern "C" __declspec(dllexport)void
AddOcrColorSet(int colorList[], int size) {

	myMainCore->AddOcrColorSet(colorList, size);
}

#pragma endregion


extern "C" __declspec(dllexport)void
setCutPoint(int newX[], int newY[], int newX2[], int newY2[], int size) {
	captureCount = size;
	myMainCore->setCutPoint(newX, newY, newX2, newY2, size);

}


extern "C" __declspec(dllexport)void
SetExceptPoint(int newX[], int newY[], int newX2[], int newY2[], int size) {
	myMainCore->SetExceptPoint(newX, newY, newX2, newY2, size);

}




extern "C" __declspec(dllexport)void
processOcr(wchar_t resultOriginal[], wchar_t resultTranslation[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	resultDB getDB;
	std::wstring out;

	std::wstring ocrResult = L"";
	std::wstring translationResult = L"";
	if (captureCount == 1)
	{
		getImg(0);
		getText(&getDB);

		std::wcout << std::endl << L" Done img and text" << std::endl;

		ocrResult = ocrResult + getDB.original + L"\r\n";
		translationResult = translationResult + getDB.translation;

		std::wcout << std::endl << L" Copy" << std::endl;
	}
	else
	{
		for (int i = 0; i < captureCount; i++)
		{
			getImg(i);
			getText(&getDB);

			if (myMainCore->GetIsShowOCRIndex())
			{
				stringstream s;
				s << i + 1;
				std::string converted(s.str());
				std::wstring wConverted = stringToWstring(converted);
				ocrResult = ocrResult + wConverted + L": " + getDB.original;
				if (getDB.translation.compare(L"not thing") != 0)
				{
					translationResult = translationResult + wConverted + L" : " + getDB.translation + L"\r\n";
				}
				s.clear();
			}
			else
			{
				if (!getDB.original.empty())
				{
					ocrResult = ocrResult + L"- " + getDB.original;
					if (getDB.translation.compare(L"not thing") != 0)
					{
						translationResult = translationResult + L"- " + getDB.translation + L"\r\n";
					}
				}
			}

			if (i + 1 < captureCount)
			{
				ocrResult = ocrResult + L"\t" + L"\r\n";
			}

		}
	}

	std::wcout << std::endl << L" Copy1" << std::endl;
	std::wcscpy(resultOriginal, ocrResult.c_str());
	std::wcout << std::endl << L" Copy2" << std::endl;
	std::wcscpy(resultTranslation, translationResult.c_str());

	std::wcout << std::endl << L" Copy Done" << std::endl;
}


extern "C" __declspec(dllexport)void
processOcrWithData(wchar_t resultOriginal[], wchar_t resultTranslation[], int width, int height, int positionX, int positionY, uint8_t * data) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	resultDB getDB;
	std::wstring out;

	std::wstring ocrResult = L"";
	std::wstring translationResult = L"";
	if (captureCount == 1)
	{
		GetImgWithData(0, data, width, height, positionX, positionY, false);
		getText(&getDB);

		ocrResult = ocrResult + getDB.original + L"\r\n";
		translationResult = translationResult + getDB.translation;
	}
	else
	{
		for (int i = 0; i < captureCount; i++)
		{
			GetImgWithData(i, data, width, height, positionX, positionY, false);
			getText(&getDB);

			if (myMainCore->GetIsShowOCRIndex())
			{
				stringstream s;
				s << i + 1;
				std::string converted(s.str());
				std::wstring wConverted = stringToWstring(converted);
				ocrResult = ocrResult + wConverted + L": " + getDB.original;
				if (getDB.translation.compare(L"not thing") != 0)
				{
					translationResult = translationResult + wConverted + L" : " + getDB.translation + L"\r\n";
				}
				s.clear();
			}
			else
			{
				if (!getDB.original.empty())
				{
					ocrResult = ocrResult + L"- " + getDB.original;
					if (getDB.translation.compare(L"not thing") != 0)
					{
						translationResult = translationResult + L"- " + getDB.translation + L"\r\n";
					}
				}
			}

			if (i + 1 < captureCount)
			{
				ocrResult = ocrResult + L"\t" + L"\r\n";
			}

		}
	}
	std::wcscpy(resultOriginal, ocrResult.c_str());
	std::wcscpy(resultTranslation, translationResult.c_str());
}


//이미지 결과 가져오기.
extern "C" __declspec(dllexport)uchar *
processGetImgData(int caputreIndex, int* x, int* y, int* channels, int* locationX, int* locationY) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (captureCount >= caputreIndex + 1)
	{
		GetImg(caputreIndex, locationX, locationY);
		//data = screenImg->data;
		*y = screenImg->size().height;
		*x = screenImg->size().width;
		*channels = screenImg->channels();

		uchar* data = (uchar*)LocalAlloc(LPTR, screenImg->size().height * screenImg->size().width * screenImg->channels());
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



//이미지 결과 가져오기.
extern "C" __declspec(dllexport)uchar *
ProcessGetImgDataFromByte(int caputreIndex, int width, int height, int positionX, int positionY, uint8_t * data, int* x, int* y, int* channels, bool getOriginal) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//std::cout << "ssss"  << (int)(data[0]) << "/"<< (int) data[1] << "/" << (int)data[2] << "/" << (int)data[3];
	if (captureCount >= caputreIndex + 1)
	{

		GetImgWithData(caputreIndex, data, width, height, positionX, positionY, getOriginal);

		//data = screenImg->data;
		*y = screenImg->size().height;
		*x = screenImg->size().width;
		*channels = screenImg->channels();

		uchar* data = (uchar*)LocalAlloc(LPTR, screenImg->size().height * screenImg->size().width * screenImg->channels());
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


}



//스펠 체크.
extern "C" __declspec(dllexport)void
ProcessGetSpellingCheck(wchar_t resultOriginal[], bool isUseMatchWordDic) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::wstring out;

	std::wstring wText = resultOriginal;

	bool isReplaceFlag = false;
	wText = myMainCore->checkSpelling(wText, &isReplaceFlag, L"\r\n");

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


